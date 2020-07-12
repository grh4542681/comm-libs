/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:03
 * @file     : parser_json.h
 * @brief    : Parser json format header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PERSER_JSON_H__
#define __PERSER_JSON_H__

#include <vector>
#include <map>
#include <time.h>

#include "parser_return.h"
#include "parser.h"
#include "mempool.h"
#include "mutex/thread_rw_lock.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"

#define MAXJSONFILESIZE (1024*1024)

namespace parser {

/**
* @brief - Type of json field.
*/
typedef enum class _JsonType : int {
    STRING = 1, ///< string.
    INT,        ///< int.
    DOUBLE,     ///< double.
    BOOL,       ///< bool.
    ARRAY,      ///< array.
    OBJECT,     ///< object.
    ONULL,      ///< NULL.
}JsonType;

class ParserJson;
/**
* @brief - Json tree element class.
*/
class ParserJsonObject {
public:
    ParserJsonObject();
    ParserJsonObject(ParserJson*, rapidjson::Value*);
    ParserJsonObject(const parser::ParserJsonObject&);
    ParserJsonObject(const parser::ParserJsonObject&&);
    ~ParserJsonObject();

    bool isAvailable();
    bool hasError();
    ParserRet getLastRet();

    bool isString();
    bool isInt();
    bool isLong();
    bool isDouble();
    bool isNull();
    bool isBool();
    bool isArray();
    bool isObject();
    
    ParserRet getString(char* cache, unsigned int cache_size, struct timespec* overtime = NULL);
    ParserRet getInt(int* cache, struct timespec* overtime = NULL);
    ParserRet getLong(long* cache, struct timespec* overtime = NULL);
    ParserRet getDouble(double* cache, struct timespec* overtime = NULL);
    ParserRet getBool(bool* cache, struct timespec* overtime = NULL);
    ParserRet getArray(std::vector<ParserJsonObject>* cache, struct timespec* overtime = NULL);
    ParserRet getObject(std::map<std::string, ParserJsonObject>* cache, struct timespec* overtime = NULL);

    ParserJsonObject& setString(char* cache, unsigned int cache_size, struct timespec* overtime = NULL);
    ParserJsonObject& setInt(int cache, struct timespec* overtime = NULL);
    ParserJsonObject& setLong(long cache, struct timespec* overtime = NULL);
    ParserJsonObject& setDouble(double cache, struct timespec* overtime = NULL);
    ParserJsonObject& setNull(struct timespec* overtime = NULL);
    ParserJsonObject& setBool(bool cache, struct timespec* overtime = NULL);
    ParserJsonObject& setArray(struct timespec* overtime = NULL);
    ParserJsonObject& setObject(struct timespec* overtime = NULL);

    ParserRet arraySize(unsigned int* size, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(const char* value, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(const char* value, unsigned int len, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(int value, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(long value, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(double value, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(bool value, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(JsonType type, unsigned int size,  struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(ParserJsonObject&& jsontree, struct timespec* overtime = NULL);
    ParserJsonObject& arrayPush(ParserJsonObject& jsontree, struct timespec* overtime = NULL);

    ParserJsonObject& arrayPop(struct timespec* overtime = NULL);
    ParserJsonObject& arrayClear(struct timespec* overtime = NULL);

    bool hasObject(const char* key, struct timespec* overtime = NULL);
    ParserRet objectSize(unsigned int* size, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, const std::string& value, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, const char* value, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, const char* value, unsigned int len, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, int value, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, long value, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, double value, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, bool value, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, JsonType type, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, ParserJsonObject&& jsontree, struct timespec* overtime = NULL);
    ParserJsonObject& objectAdd(const char* key, ParserJsonObject& jsontree, struct timespec* overtime = NULL);

    ParserJsonObject& objectDel(const char* key, struct timespec* overtime = NULL);
    ParserJsonObject& objectClear(struct timespec* overtime = NULL);

    ParserJsonObject Vfind(const std::string& path);
    ParserJsonObject Vfind(const char* path);
    ParserJsonObject& Hfind(const char* path);

    ParserJsonObject operator[](int index);
    ParserJsonObject operator[](const char* name);
    ParserJsonObject& operator=(const ParserJsonObject& other);
    ParserJsonObject& operator=(const ParserJsonObject&& other);
private:
    bool init_flag_;
    rapidjson::Value* rpj_value_;
    ParserJson* pj_center_;
    ParserRet last_ret_;

private:
    void _setret(ParserRet ret) {
        this->last_ret_ = ret;
    }
};

/**
* @brief - Main class of parser json.
*/
class ParserJson : public Parser {
public:
    friend class ParserJsonObject;

public:
    ParserJsonObject root;  ////< root element.

public:
    ParserJson();
    ~ParserJson();

    //Inherited from class Parser.
    ParserRet LoadString(std::string str);
    ParserRet LoadFile(file::File& file);
    ParserRet LoadFile(std::string str);

    ParserRet StoreString(std::string& str);
    ParserRet StoreFile(file::File& file);
    ParserRet StoreFile(std::string str);

    ParserRet ParserJsonFile(const char* filename);
    ParserRet ParserJsonString(const char* jsonstring);
    ParserRet StorageJsonFile(const char* filename, bool pretty = false);
    ParserRet StorageJsonString(std::string& jsonstring, bool pretty = false);
    ParserRet StorageJsonString(char* jsonstring, unsigned int len, bool pretty = false);

    void setThreadSafe(bool flag);
    bool getThreadSafe();

    void setNonBlock(bool flag);
    bool getNonBlock();

    ParserRet RLock(struct timespec* overtime);
    ParserRet WLock(struct timespec* overtime);
    ParserRet UnLock();

    ParserJsonObject find(const std::string& path);
    ParserJsonObject find(const char* path);
private:
    bool thread_safe_flag_;
    mempool::Mempool* mempool_;
    thread::mutex::ThreadRWLock rwlock_;

    rapidjson::Document doc_;
};

}//namespace parser end

#endif
