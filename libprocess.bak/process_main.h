#ifndef __PROCESS_MAIN_H__
#define __PROCESS_MAIN_H__

#include <utility>

#include "file.h"
#include "message_agent.h"

#include "process_log.h"
#include "process_return.h"
#include "process_info.h"
#include "group/process_group_keeper.h"

namespace process {

template < typename F >
class ProcessMain {
public:
    ProcessMain(std::string filename, F main) {
        config_filename_ = filename;
        main_ = main;
    }
    ~ProcessMain() { }

    template <typename ... Args>
    ProcessRet Run(int argc, char** argv, Args&& ... args) {
        // process info init
        ProcessInfo* process_info = ProcessInfo::getInstance();
        process_info->SetCmdLine(argc, argv, environ);

        if (config_filename_.empty() || !file::IsExist(config_filename_)) {
            PROCESS_ERROR("Con't access process config file [%s].", config_filename_.c_str());
            return ProcessRet::PROCESS_ERAWCONFIG;
        }

        file::File config_file(config_filename_);
        switch(config_file.GetFileFormat()) {
    //        case file::File::Format::Ini:
            case file::File::Format::Json:
                if (process_info->SetProcessRawConfig<parser::ParserJson>() != ProcessRet::SUCCESS) {
                    PROCESS_ERROR("Con't set process raw config with type Json");
                    return ProcessRet::PROCESS_ERAWCONFIG;
                }
                process_info->GetProcessRawConfig().LoadFile(config_file);
                break;
            case file::File::Format::Yaml:
                if (process_info->SetProcessRawConfig<parser::ParserYaml>() != ProcessRet::SUCCESS) {
                    PROCESS_ERROR("Con't set process raw config with type Yaml");
                    return ProcessRet::PROCESS_ERAWCONFIG;
                }
                process_info->GetProcessRawConfig().LoadFile(config_file);
                break;
            default:
                PROCESS_ERROR("Not support config file format [%s]", file::GetFileExtension(config_filename_).c_str());
                return ProcessRet::PROCESS_ERAWCONFIG;
        }

        if (process_info->GetProcessConfig().Load(process_info->GetProcessRawConfig()) != config::ConfigRet::SUCCESS) {
            PROCESS_FATAL("Load process config file [%s] error", config_filename_.c_str());
            return ProcessRet::PROCESS_ECONFIG;
        }

        // exec main func
        auto config_pool = process_info->GetProcessConfig().GetRoot()->Search<bool>("process/group/switch");
        if (config_pool && config_pool->GetData()) {
            int group_size = process_info->GetProcessConfig().GetRoot()->Search<int>("process/group/size")->GetData();
            group::ProcessGroupKeeper<F, Args&& ...> group_(process_info->GetRealName(),
                            group_size,
                            process_info->GetProcessConfig().GetConfigFileName(),
                            main_, std::forward<Args>(args)...);
            group_.Run();
            message::MessageAgent::getInstance()->SetType(message::MessageAgent::Type::KeeperAgent);
            message::MessageAgent::getInstance()->GetConfig().Load(process_info->GetProcessRawConfig());
            message::MessageAgent::getInstance()->Run();
        } else {
            message::MessageAgent::getInstance()->GetConfig().Load(process_info->GetProcessRawConfig());
            message::MessageAgent::getInstance()->Run();
            main_(std::forward<Args>(args)...);
        }
        return ProcessRet::SUCCESS;
    }

private:
    F main_;
    std::string config_filename_;
};

}

#endif
