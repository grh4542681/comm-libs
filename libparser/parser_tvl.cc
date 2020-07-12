#include "parser_tvl.h"

#define MAX_TVL_ELEMENT (9999)

namespace parser {

ParserTvl::ParserTvl() { }
ParserTvl::~ParserTvl()
{
}

ParserRet ParserTvl::LoadString(std::string str)
{
    int tvl_len = 0;
    int tvl_size = 0;
    int tvl_offset = 0;

    if (str.size() <= 4) {
        return ParserRet::PARSER_EFORMAT;
    }

    tvl_vec_.clear();

    sscanf(str.substr(0, 4).c_str(), "%d", &tvl_size);
    tvl_offset += 4;
    for (int loop = 0; loop < tvl_size; loop++) {
        tvl_len = 0;
        sscanf(str.substr(tvl_offset, 4).c_str(), "%d", &tvl_len);
        tvl_offset += 4;
        tvl_vec_.push_back(str.substr(tvl_offset, tvl_len));
        tvl_offset += tvl_len;
    }
    return ParserRet::SUCCESS;
}
ParserRet ParserTvl::LoadFile(file::File& file)
{
    return ParserRet::ENONSUPPORT;
}
ParserRet ParserTvl::LoadFile(std::string str)
{
    return ParserRet::ENONSUPPORT;
}

ParserRet ParserTvl::StoreString(std::string& str)
{
    str.clear();
    char slen[5];
    memset(slen, 0, sizeof(slen));
    sprintf(slen, "%04lu", tvl_vec_.size());
    str += slen;

    for (auto it : tvl_vec_) {
        memset(slen, 0, sizeof(slen));
        sprintf(slen, "%04lu", it.size());
        printf("--len %s\n",slen);
        str += slen;
        str += it;
    }
    return ParserRet::SUCCESS;
}
ParserRet ParserTvl::StoreFile(file::File& file)
{
    return ParserRet::ENONSUPPORT;

}
ParserRet ParserTvl::StoreFile(std::string str)
{
    return ParserRet::ENONSUPPORT;
}

ssize_t ParserTvl::GetTvlSize()
{
    return (tvl_vec_.size());
}

ParserTvl& ParserTvl::PushHead(ParserTvl& other)
{
    if ((tvl_vec_.size() + other.GetTvlSize()) > MAX_TVL_ELEMENT) {
        PARSER_ERROR("The number of serializable TVL elements reaches the upper limit.");
        return *this;
    }
    tvl_vec_.insert(tvl_vec_.begin(), other.tvl_vec_.begin(), other.tvl_vec_.end());
    return *this;
}
ParserTvl& ParserTvl::PushHead(ParserTvlObject& other)
{
    if (tvl_vec_.size() >= MAX_TVL_ELEMENT) {
        PARSER_ERROR("The number of serializable TVL elements reaches the upper limit.");
        return *this;
    }
    std::string stvl;
    stvl.clear();
    other.BuildTvlString(&stvl);
    tvl_vec_.insert(tvl_vec_.begin(), stvl);
    return *this;
}
std::string ParserTvl::PopHead()
{
    if (tvl_vec_.size() == 0) {
        return "";
    }
    std::string stmp = tvl_vec_.front();
    tvl_vec_.erase(tvl_vec_.begin());
    return stmp;
}

ParserTvl& ParserTvl::PushBack(ParserTvl& other)
{
    if ((tvl_vec_.size() + other.GetTvlSize()) > MAX_TVL_ELEMENT) {
        PARSER_ERROR("The number of serializable TVL elements reaches the upper limit.");
        return *this;
    }
    tvl_vec_.insert(tvl_vec_.end(), other.tvl_vec_.begin(), other.tvl_vec_.end());
    return *this;
}
ParserTvl& ParserTvl::PushBack(ParserTvlObject& other)
{
    if (tvl_vec_.size() >= MAX_TVL_ELEMENT) {
        PARSER_ERROR("The number of serializable TVL elements reaches the upper limit.");
        return *this;
    }
    std::string stvl;
    stvl.clear();
    other.BuildTvlString(&stvl);
    tvl_vec_.push_back(stvl);
    return *this;
}
std::string ParserTvl::PopBack()
{
    if (tvl_vec_.size() == 0) {
        return "";
    }
    std::string stmp = tvl_vec_.back();
    tvl_vec_.pop_back();
    return stmp;
}

}
