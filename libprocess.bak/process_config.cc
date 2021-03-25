#include "process_config.h"
#include "process_log.h"

namespace infra::process {

config::ConfigRet ProcessConfig::LoadJson(parser::ParserJson& parser)
{
    printf("process config load\n");
    char json_message_string[1024];
    
    auto json_root = parser.find("/");

    return config::ConfigRet::SUCCESS;
}
config::ConfigRet ProcessConfig::LoadYaml(parser::ParserYaml& parser)
{
    printf("process config load\n");
    std::string tmp_string;
    int tmp_int = 0;
    bool tmp_bool = false;

    auto yaml_root = parser.GetDoc();

    auto yaml_process = yaml_root.Find("process");
    if (yaml_root.HasError()) {
        PROCESS_FATAL("Not found config : process");
        return config::ConfigRet::ERROR;
    }
    auto config_process = config_tree_.GetRoot()->Insert("process");

    //process pool
    auto yaml_process_pool = yaml_process.Find("group");
    if (yaml_process.HasError()) {
        PROCESS_FATAL("Not found config : process/group");
        return config::ConfigRet::ERROR;
    }
    auto config_process_pool = config_process->Insert("group");

    if (yaml_process_pool.Find("switch").GetData<bool>(&tmp_bool) != parser::ParserRet::SUCCESS) {
        PROCESS_FATAL("Not found config : process/group/switch");
        return config::ConfigRet::ERROR;
    }
    config_process_pool->Insert<bool>("switch", tmp_bool);
    if (tmp_bool) {
        if (yaml_process_pool.Find("size").GetData<int>(&tmp_int) != parser::ParserRet::SUCCESS) {
            PROCESS_FATAL("Not found config : process/group/size");
            return config::ConfigRet::ERROR;
        }
        config_process_pool->Insert<int>("size", tmp_int);
    }
    config_tree_.Print();
    return config::ConfigRet::SUCCESS;
}

}
