#ifndef __CRYPT_SNOWFLAKE_H__
#define __CRYPT_SNOWFLAKE_H__

#include "object.h"
#include "crypt_log.h"
#include "crypt_return.h"

namespace infra::crypt {

class Snowflake : virtual public infra::base::Object {
public:
    enum class Component {
        TimeEpoch = 1,
        TimeBack,
        Machine,
        Serial,
    };
    enum class Mode {
        Default,
        SingleNode,
        Distribute,
    };
    using Composition_t = std::vector<std::tuple<Component, unsigned int>>;
public:
    Snowflake(Mode&& mode) {
        for (auto it = Snowflake::ModeComposition.at(mode).rbegin();
                it != Snowflake::ModeComposition.at(mode).rend(); it++) {
            composition_.push_back({std::get<0>(*it), std::get<1>(*it), 0l});
        }
    }
//    Snowflake(Composition_t&& composition) {
//        for (auto it = composition.rbegin(); it != composition.at(mode).rend(); it++) {
//            composition_.push_back({std::get<0>(it), std::get<1>(it), 0l});
//        }
//    }
    ~Snowflake() { }

    long ID() {

    }
private:
    long _component_id(std::tuple<Snowflake::Component, unsigned int>) {

    }

    long _assemble();
    long _disassemble();
private:
    long prev_;
    std::vector<std::tuple<Component, unsigned int, long>> composition_;
    static const std::map<Mode, std::vector<std::tuple<Component, unsigned int>>> ModeComposition;
};

const std::map<Snowflake::Mode, std::vector<std::tuple<Snowflake::Component, unsigned int>>> Snowflake::ModeComposition = {
    {Mode::Default,    {{ Snowflake::Component::TimeEpoch, 41 },
                        { Snowflake::Component::Machine,   10 },
                        { Snowflake::Component::Serial,    12 }}},
    {Mode::SingleNode, {{ Snowflake::Component::TimeEpoch, 45 },
                        { Snowflake::Component::TimeBack,  2  },
                        { Snowflake::Component::Serial,    16 }}},
    {Mode::Distribute, {{ Snowflake::Component::TimeEpoch, 41 },
                        { Snowflake::Component::TimeBack,  2  },
                        { Snowflake::Component::Machine,   10 },
                        { Snowflake::Component::Serial,    12 }}},
};

}

#endif
