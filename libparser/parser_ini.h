/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:03
 * @file     : parser_ini.h
 * @brief    : Parser ini format header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PARSER_INI_H__
#define __PARSER_INI_H__

#include <map>

#include "parser_return.h"
#include "mempool.h"

namespace infra::parser {

/**
* @brief - Parser *.ini config file class.
*/
class ParserIni{
public:
    typedef std::map<std::string, std::string> Section; ///< Section map.

    /**
    * @brief ParserIni - Construct function.
    *
    * @param [filename] - Config file name.
    */
    ParserIni(std::string filename);
    /**
    * @brief ~ParserIni - Destruct fucntion.
    */
    ~ParserIni();

    /**
    * @brief Load - Load configuration file contents into memory.
    *
    * @returns  ParserRet.
    */
    ParserRet Load();
    /**
    * @brief Reload - Reload the contents of the new configuration file into memory.
    *
    * @param [filename] - New config file name.
    *
    * @returns  ParserRet.
    */
    ParserRet Reload(std::string filename);
    /**
    * @brief Storage - Save the in-memory configuration to a file.
    *
    * @param [filename] - Output file name.
    *
    * @returns  ParserRet.
    */
    ParserRet Storage(std::string filename);
    /**
    * @brief Free - Release the in-memory configuration.
    *
    * @returns  ParserRet.
    */
    ParserRet Free();
    /**
    * @brief Print - Print the in-memory configuration.
    *
    * @returns  
    */
    ParserRet Print();

    /**
    * @brief getConfig - Get designated configuration value.
    *
    * @param [section] - Section name.
    * @param [item] - Item name.
    *
    * @returns  value (string type).
    */
    std::string getConfig(std::string section, std::string item);
    /**
    * @brief setConfig - Set designated configuration value into memory.
    *
    * @param [section] - Section name.
    * @param [item] - Item name.
    * @param [value] - Configuration valu.
    *
    * @returns  ParserRet.
    */
    ParserRet setConfig(std::string section, std::string item, std::string value);

private:
    bool init_flag_;
    mempool::Mempool* mempool_;
    std::string filename_;
    std::map<std::string, Section*> conftree_;

    ParserRet _load();
    ParserRet _free();
};

}//namespace infra::parser end

#endif
