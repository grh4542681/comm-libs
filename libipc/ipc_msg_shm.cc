#include <string.h>

#include "msg_shm.h"

#define MSG_SHM_MUTEX_INDEX (0)
#define MSG_SHM_COUNT_INDEX (1)
#define MSG_SHM_ALIGN_SIZE (8)

namespace infra::ipc::msg {

MsgShm::MsgShm()
{

}

MsgShm::MsgShm(std::string path) : Msg(path)
{
    if (!path.empty()) {
        shm_ = shm::ShmPosix(path + ".shm");
        sem_ = sem::SemPosix(path + ".sem");
    }
}

MsgShm::~MsgShm()
{
}

size_t MsgShm::GetMsgNum()
{
    if (p_shm_head_) {
        return p_shm_head_->msg_num;
    }
    return 0;
}

size_t MsgShm::GetFreeSize()
{
    if (p_shm_head_) {
        return p_shm_head_->free_size;
    }
    return 0;
}

IpcRet MsgShm::GetRet()
{
    return ret_;
}

IpcRet MsgShm::Create(size_t size, mode_t mode)
{
    IpcRet ret = IpcRet::SUCCESS;
    ret = shm_.Create(_align(size, MSG_SHM_ALIGN_SIZE), mode);
    if (ret != IpcRet::SUCCESS) {
        return ret;
    }

    ret = sem_.Create(2, mode);
    if (ret != IpcRet::SUCCESS) {
        shm_.Destroy();
        return ret;
    }

    ret = shm_.Open(IpcMode::READ_WRITE);
    if (ret == IpcRet::SUCCESS) {
        p_shm_head_ = reinterpret_cast<MsgShmHead*>(shm_.GetHeadPtr());
        p_shm_head_->msg_num = 0;
        p_shm_head_->free_size = shm_.GetSize() - sizeof(MsgShmHead);
        p_shm_head_->data_start = sizeof(MsgShmHead);
        p_shm_head_->data_end = shm_.GetSize();
        p_shm_head_->msg_head =  p_shm_head_->data_start;
        p_shm_head_->msg_tail = p_shm_head_->data_start;
    } else {
        shm_.Destroy();
        sem_.Destroy();
        return ret;
    }
    shm_.Close();

    ret = sem_.Open(IpcMode::READ_WRITE);
    if (ret == IpcRet::SUCCESS) {
        sem_.V(MSG_SHM_MUTEX_INDEX);
    } else {
        shm_.Destroy();
        sem_.Destroy();
        return ret;
    }
    sem_.Close();

    return ret;
}

IpcRet MsgShm::Destroy()
{
    shm_.Destroy();
    sem_.Destroy();
    p_shm_head_ = NULL;
    return IpcRet::SUCCESS;
}

IpcRet MsgShm::Open(IpcMode mode)
{
    IpcRet ret = IpcRet::SUCCESS;
    ret = shm_.Open(mode);
    if (ret != IpcRet::SUCCESS) {
        return ret;
    }

    ret = sem_.Open(mode);
    if (ret != IpcRet::SUCCESS) {
        shm_.Close();
        return ret;
    }

    p_shm_head_ = reinterpret_cast<MsgShmHead*>(shm_.GetHeadPtr());
    init_flag_ = true;

    return ret;
}

IpcRet MsgShm::Close()
{
    shm_.Close();
    sem_.Close();
    p_shm_head_ = NULL;
    init_flag_ = false;
    return IpcRet::SUCCESS;
}

size_t MsgShm::Recv(void* data, size_t data_len, timer::Time* overtime)
{
    IpcRet ret;
    if (!init_flag_ || !p_shm_head_) {
        ret_ = IpcRet::EINIT;
        return 0;
    }
    if (!data || (data_len == 0)) {
        ret_ = IpcRet::EBADARGS;
        return 0;
    }

    ret = sem_.P(MSG_SHM_COUNT_INDEX, overtime);
    if (ret != IpcRet::SUCCESS) {
        ret_ = ret;
        return 0;
    }
    ret = sem_.P(MSG_SHM_MUTEX_INDEX, overtime);
    if (ret != IpcRet::SUCCESS) {
        ret_ = ret;
        sem_.V(MSG_SHM_COUNT_INDEX);
        return 0;
    }

    //pointer conv
    char* data_start = reinterpret_cast<char*>(p_shm_head_) + p_shm_head_->data_start;
    char* msg_head = reinterpret_cast<char*>(p_shm_head_) + p_shm_head_->msg_head;

    size_t tmp_len = 0;
    if ((p_shm_head_->data_end - p_shm_head_->msg_head) >= sizeof(size_t)) {
        tmp_len = *reinterpret_cast<size_t*>(msg_head);
        if (_align(data_len, MSG_SHM_ALIGN_SIZE) < tmp_len) {
            ret_ = IpcRet::MSG_ENOSPACE;
            sem_.V(MSG_SHM_COUNT_INDEX);
            sem_.V(MSG_SHM_MUTEX_INDEX);
            return 0;
        }
        p_shm_head_->msg_head += sizeof(size_t);
        p_shm_head_->msg_head = (p_shm_head_->msg_head == p_shm_head_->data_end) ? p_shm_head_->data_start : p_shm_head_->msg_head;
    } else {
        memcpy(&tmp_len, msg_head, (p_shm_head_->data_end - p_shm_head_->msg_head));
        memcpy(&tmp_len, data_start, sizeof(size_t) - (p_shm_head_->data_end - p_shm_head_->msg_head));
        if (_align(data_len, MSG_SHM_ALIGN_SIZE) < tmp_len) {
            ret_ = IpcRet::MSG_ENOSPACE;
            sem_.V(MSG_SHM_COUNT_INDEX);
            sem_.V(MSG_SHM_MUTEX_INDEX);
            return 0;
        }
        p_shm_head_->msg_head = p_shm_head_->data_start + (sizeof(size_t) - (p_shm_head_->data_end - p_shm_head_->msg_head));
    }

    msg_head = reinterpret_cast<char*>(p_shm_head_) + p_shm_head_->msg_head;

    if ((p_shm_head_->data_end - p_shm_head_->msg_head) >= tmp_len) {
        memcpy(data, msg_head, tmp_len);
        p_shm_head_->msg_head += tmp_len;
        p_shm_head_->msg_head = (p_shm_head_->msg_head == p_shm_head_->data_end) ? p_shm_head_->data_start : p_shm_head_->msg_head;
    } else {
        memcpy(data, msg_head, (p_shm_head_->data_end - p_shm_head_->msg_head));
        p_shm_head_->msg_head = p_shm_head_->data_start;
        memcpy(data, msg_head, tmp_len - (p_shm_head_->data_end - p_shm_head_->msg_head));
        p_shm_head_->msg_head += (tmp_len - (p_shm_head_->data_end - p_shm_head_->msg_head));
    }

    p_shm_head_->msg_num--;
    p_shm_head_->free_size += (tmp_len + sizeof(size_t));
    sem_.V(MSG_SHM_MUTEX_INDEX);
    return tmp_len;
}

size_t MsgShm::Send(void* data, size_t data_len, timer::Time* overtime)
{
    IpcRet ret;
    if (!init_flag_ || !p_shm_head_) {
        ret_ = IpcRet::EINIT;
        return 0;
    }
    if (!data || (data_len == 0)) {
        ret_ = IpcRet::EBADARGS;
        return 0;
    }

    data_len = _align(data_len, MSG_SHM_ALIGN_SIZE);

    if (p_shm_head_->free_size < data_len + sizeof(size_t)) {
        ret_ = IpcRet::MSG_ENOSPACE;
        return 0;
    }
    ret = sem_.P(MSG_SHM_MUTEX_INDEX, overtime);
    if (ret != IpcRet::SUCCESS) {
        ret_ = ret;
        return 0;
    }

    char* data_start = reinterpret_cast<char*>(p_shm_head_) + p_shm_head_->data_start;
    char* msg_tail = reinterpret_cast<char*>(p_shm_head_) + p_shm_head_->msg_tail;

    if (p_shm_head_->msg_head <= p_shm_head_->msg_tail) {
        size_t tmp_len = p_shm_head_->data_end - p_shm_head_->msg_tail;
        if (tmp_len >= sizeof(size_t)) {
            *reinterpret_cast<size_t*>(msg_tail) = data_len;
            p_shm_head_->msg_tail += sizeof(size_t);
            tmp_len  -= sizeof(size_t);
            p_shm_head_->msg_tail = (p_shm_head_->msg_tail == p_shm_head_->data_end) ? p_shm_head_->data_start : p_shm_head_->msg_tail;
        } else {
            memcpy(msg_tail, &data_len, tmp_len);
            p_shm_head_->msg_tail = p_shm_head_->data_start;
            memcpy(data_start, reinterpret_cast<char*>(&data_len) + tmp_len, sizeof(size_t) - tmp_len);
            p_shm_head_->msg_tail += (sizeof(size_t) - tmp_len);
            tmp_len = 0;
        }

        msg_tail = reinterpret_cast<char*>(p_shm_head_) + p_shm_head_->msg_tail;

        if (tmp_len >= data_len) {
            memcpy(msg_tail, data, data_len);
            p_shm_head_->msg_tail += data_len;
            p_shm_head_->msg_tail = (p_shm_head_->msg_tail == p_shm_head_->data_end) ? p_shm_head_->data_start : p_shm_head_->msg_tail;
        } else {
            memcpy(msg_tail, data, tmp_len);
            p_shm_head_->msg_tail = p_shm_head_->data_start;
            memcpy(data_start, reinterpret_cast<char*>(data) + tmp_len, data_len - tmp_len);
            p_shm_head_->msg_tail += data_len - tmp_len;
        }
    } else {
        *reinterpret_cast<size_t*>(msg_tail) = data_len;
        p_shm_head_->msg_tail += sizeof(size_t);
        msg_tail = reinterpret_cast<char*>(p_shm_head_) + p_shm_head_->msg_tail;
        memcpy(msg_tail, data, data_len);
        p_shm_head_->msg_tail += data_len;
        p_shm_head_->msg_tail = (p_shm_head_->msg_tail == p_shm_head_->data_end) ? p_shm_head_->data_start : p_shm_head_->msg_tail;
    }

    p_shm_head_->free_size -= (data_len + sizeof(size_t));
    p_shm_head_->msg_num++;
    sem_.V(MSG_SHM_COUNT_INDEX);
    sem_.V(MSG_SHM_MUTEX_INDEX);

    return data_len;
}

size_t MsgShm::_align(size_t raw_size, size_t align_size) {
    return (raw_size + align_size - 1) &~ (align_size - 1);
}

}
