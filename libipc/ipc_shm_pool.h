#ifndef __SHM_POOL_H__
#define __SHM_POOL_H__

#include <string>

#include "shm.h"

namespace infra::ipc::shm {

template < typename S > class ShmPool {
public:
    ShmPool() : shm_() {
        static_assert(std::is_base_of<Shm, S>::value, "Must be a subclass that inherits from Shm.");
    }
    ShmPool(std::string path) : shm_(path) {
        static_assert(std::is_base_of<Shm, S>::value, "Must be a subclass that inherits from Shm.");
    }
    ~ShmPool() {}

    IpcRet Create(size_t shm_size, mode_t mode) {
        IpcRet ret = shm_.Create(shm_size, mode);
        if (ret != IpcRet::SUCCESS) {
            return ret;
        }
        ret = shm_.Open(IpcMode::READ_WRITE);
        if (ret == IpcRet::SUCCESS) {
           
        }
        ret = shm_.Close();
        if (ret == IpcRet::SUCCESS) {

        }
        cursize_ = shm_size;
        return ret;
    }

    IpcRet Destroy() {
        IpcRet ret = shm_.Destroy();
        if (ret == IpcRet::SUCCESS) {

        }
        return ret;
    }

    IpcRet Open(IpcMode mode) {
        IpcRet ret = shm_.Open(mode);
        if (ret == IpcRet::SUCCESS) {

        }
        return ret;
    }
    
    IpcRet Close() {
        IpcRet ret = shm_.Close();
        if (ret == IpcRet::SUCCESS) {

        }
        return ret;
    }

    void* Malloc(size_t size){
        return _malloc(size);
    }
    template < typename T, typename ... Args> T* Malloc(Args&& ... args){
        T* ptr = (T*)Malloc(sizeof(T));
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<Args>(args)...));
    }

    template < typename T, typename ... Args> T* Reset(T* ptr, Args&& ... args){
        ptr->~T();
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<Args>(args)...));
    }

    void Free(void* ptr){
        _free(ptr);
    }
    template < typename T > void Free(T* ptr){
        ptr->~T();
        _free(ptr);
    }

    template < typename T, typename ... Args> static T* Construct(void* ptr, Args&& ... args){
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<Args>(args)...));
    }

    template < typename T > static void* Destruct(T* ptr){
        ptr->~T();
        return ptr;
    }

private:
    S shm_;
    size_t cursize_ = 0;

    void* _malloc(size_t size) {

    }
    void _free(void* ptr) {

    }
};

}

#endif