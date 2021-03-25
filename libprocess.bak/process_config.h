#ifndef __PROCESS_CONFIG_H__
#define __PROCESS_CONFIG_H__

#include "config_static.h"

namespace infra::process {

class ProcessConfig : public config::ConfigStatic {
public:
    ProcessConfig() {

    }
    ~ProcessConfig(){

    }

    config::ConfigRet LoadJson(parser::ParserJson& parser);
    config::ConfigRet LoadYaml(parser::ParserYaml& parser);
};

}

#endif
