#ifndef __PARSER_YAML_H__
#define __PARSER_YAML_H__

#include "yaml-cpp/yaml.h"

#include "parser_log.h"
#include "parser.h"

namespace parser {

class ParserYamlObject {
public:
    ParserYamlObject();
    ParserYamlObject(const YAML::Node& node);
    ParserYamlObject(const ParserYamlObject& other);
    ~ParserYamlObject();

    ParserYamlObject& operator=(const ParserYamlObject& other);
    ParserYamlObject operator[](const std::string index);
    ParserYamlObject operator[](const int index);

    bool HasError();
    bool IsVector();

    ParserYamlObject Find(std::string index);
    template < typename D > ParserRet GetData(D* data) {
        try {
            *data = node_.as<D>();
        } catch (YAML::Exception& e) {
            PARSER_ERROR("Get data from node error : %s", e.what());
            return ParserRet::ERROR;
        } catch (...) {
            PARSER_ERROR("Get data from node error : Unknow exception.");
            return ParserRet::ERROR;
        }
        return ParserRet::SUCCESS;
    }
private:
    YAML::Node node_;
    ParserRet ret_;

    void _set_ret(const ParserRet& ret);
};

class ParserYaml : public Parser{
public:
    ParserYaml();
    ~ParserYaml();

    ParserRet LoadString(std::string str);
    ParserRet LoadFile(file::File& file);
    ParserRet LoadFile(std::string str);

    ParserYamlObject& GetDoc();
private:
    ParserYamlObject root_;
};

}

#endif
