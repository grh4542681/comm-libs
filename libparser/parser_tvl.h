#ifndef __PARSER_TVL_H__
#define __PARSER_TVL_H__

#include <vector>
#include "parser_log.h"
#include "parser_return.h"
#include "parser.h"

namespace parser {

class ParserTvlObject {
public:
    ParserTvlObject() { }
    virtual ~ParserTvlObject() { }
    virtual ParserRet BuildTvlString(std::string* str) = 0;
    virtual ParserRet ParseTvlString(const std::string& str) = 0;
};

class ParserTvl : public Parser {
public:
    ParserTvl();
    ~ParserTvl();

    //Inherited from class Parser
    ParserRet LoadString(std::string str);
    ParserRet LoadFile(file::File& file);
    ParserRet LoadFile(std::string str);
    
    ParserRet StoreString(std::string& str);
    ParserRet StoreFile(file::File& file);
    ParserRet StoreFile(std::string str);

    ssize_t GetTvlSize();
    ParserTvl& PushHead(ParserTvl& other);
    ParserTvl& PushHead(ParserTvlObject& other);
    std::string PopHead();
    ParserTvl& PushBack(ParserTvl& other);
    ParserTvl& PushBack(ParserTvlObject& other);
    std::string PopBack();
private:
    std::vector<std::string> tvl_vec_;
};

}

#endif
