#include "string/string_util.h"

#include "parser_log.h"
#include "parser_yaml.h"

namespace infra::parser {

ParserYamlObject::ParserYamlObject()
{
    ret_ = ParserRet::SUCCESS;
}
ParserYamlObject::ParserYamlObject(const YAML::Node& node)
{
    node_ = node;
    ret_ = ParserRet::SUCCESS;
}
ParserYamlObject::ParserYamlObject(const ParserYamlObject& other)
{
    node_ = other.node_;
    ret_ = other.ret_;
}

ParserYamlObject::~ParserYamlObject() { }

ParserYamlObject& ParserYamlObject::operator=(const ParserYamlObject& other)
{
    node_ = other.node_;
    ret_ = other.ret_;
    return *this;
}

ParserYamlObject ParserYamlObject::operator[](const std::string index)
{
//    std::string path = index;
//    std::vector<std::string> path_vec;
//    util::String::Split(path, path_vec);
//
//    YAML::Node empty_node;
//    YAML::Node tmp_node = node_;
//    for (auto it : path_vec) {
//        try {
//            tmp_node = tmp_node[it];
//        } catch (YAML::InvalidNode& e) {
//            PARSER_ERROR("Find object %s error : %s", it.c_str(), e.what());
//            _set_ret(ParserRet::PARSER_ENOTFOUND);
//            return ParserYamlObject(empty_node);
//        } catch (...) {
//            _set_ret(ParserRet::ERROR);
//            return ParserYamlObject(empty_node);
//        }
//    }
//    return ParserYamlObject(tmp_node);
    YAML::Node tmp_node;
    try {
        tmp_node = node_[index];
    } catch (YAML::Exception& e) {
        PARSER_ERROR("Find object %s error : %s", index.c_str(), e.what());
        _set_ret(ParserRet::PARSER_ENOTFOUND);
        return ParserYamlObject(tmp_node);
    } catch (...) {
        _set_ret(ParserRet::ERROR);
        return ParserYamlObject(tmp_node);
    }
    return ParserYamlObject(tmp_node);
}

ParserYamlObject ParserYamlObject::operator[](const int index)
{
    YAML::Node empty_node;
    YAML::Node tmp_node;
    try {
        tmp_node = node_[index];
    } catch (YAML::InvalidNode& e) {
        PARSER_ERROR("Find object array index %d error : %s", index, e.what());
        _set_ret(ParserRet::PARSER_ENOTFOUND);
        return ParserYamlObject(empty_node);
    } catch (...) {
        _set_ret(ParserRet::ERROR);
        return ParserYamlObject(empty_node);
    }
    return tmp_node;
}

bool ParserYamlObject::HasError()
{
    return (ret_ != ParserRet::SUCCESS);
}

bool ParserYamlObject::IsVector()
{
    return node_.IsSequence();
}

ParserYamlObject ParserYamlObject::Find(std::string index)
{
 //   std::string path = index;
 //   std::vector<std::string> path_vec;
 //   util::String::Split(path, "/", path_vec);

 //   YAML::Node empty_node;
 //   YAML::Node tmp_node = node_;
 //   for (auto it : path_vec) {
 //       try {
 //           tmp_node = tmp_node[it];
 //       } catch (YAML::Exception& e) {
 //           PARSER_ERROR("Find object %s error : %s", it.c_str(), e.what());
 //           _set_ret(ParserRet::PARSER_ENOTFOUND);
 //           return ParserYamlObject(empty_node);
 //       } catch (...) {
 //           _set_ret(ParserRet::ERROR);
 //           return ParserYamlObject(empty_node);
 //       }
 //   }
 //           printf("aa %s\n", tmp_node.as<std::string>().c_str());
 //   return ParserYamlObject(tmp_node);
    YAML::Node tmp_node;
    try {
        tmp_node = node_[index];
    } catch (YAML::Exception& e) {
        PARSER_ERROR("Find object %s error : %s", index.c_str(), e.what());
        _set_ret(ParserRet::PARSER_ENOTFOUND);
        return ParserYamlObject(tmp_node);
    } catch (...) {
        _set_ret(ParserRet::ERROR);
        return ParserYamlObject(tmp_node);
    }
    return ParserYamlObject(tmp_node);
}

void ParserYamlObject::_set_ret(const ParserRet& ret)
{
    ret_ = ret;
}


ParserYaml::ParserYaml() : Parser() {
    object_name_ = "ParserYaml";
    object_type_name_ = "ParserYaml";
    object_describe_ = "Class for parsing yaml format";
}
ParserYaml::~ParserYaml() { }

ParserRet ParserYaml::LoadString(std::string str)
{
    return ParserRet::SUCCESS;
}

ParserRet ParserYaml::LoadFile(file::File& file)
{
    try {
        auto yaml_root = YAML::LoadFile(file.GetFileName());
        root_ = ParserYamlObject(yaml_root);
    } catch (YAML::Exception& e) {
        PARSER_ERROR("Parser file error : %s", e.what());
        return ParserRet::ERROR;
    } catch (...) {
        PARSER_ERROR("Parser file error : Unknow exception.");
        return ParserRet::ERROR;
    }
    return ParserRet::SUCCESS;
}

ParserRet ParserYaml::LoadFile(std::string str)
{
    try {
        auto yaml_root = YAML::LoadFile(str);
        root_ = ParserYamlObject(yaml_root);
    } catch (YAML::Exception& e) {
        PARSER_ERROR("Parser file error : %s", e.what());
        return ParserRet::ERROR;
    } catch (...) {
        PARSER_ERROR("Parser file error : Unknow exception.");
        return ParserRet::ERROR;
    }
    return ParserRet::SUCCESS;
}

ParserYamlObject& ParserYaml::GetDoc()
{
    return root_;
}

}
