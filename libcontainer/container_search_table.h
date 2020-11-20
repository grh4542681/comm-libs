#ifndef __CONTAINER_TABLE_H__
#define __CONTAINER_TABLE_H__

#include <vector>
#include <map>

#include "object.h"
#include "container_log.h"
#include "container_return.h"

namespace infra::container {

template < typename ... Args >
class SearchTable : virtual public infra::base::Object {
public:
    SearchTable() { }
    ~SearchTable() { }

    bool AliasColumn(std::vector<std::string>&& field) {
        return AliasColumn("Default", field);
    }
    bool AliasColumn(std::string alias, std::vector<std::string>&& field) {
        if (field.size() != std::tuple_size<std::tuple<Args...>>::value) {
            Log::Error("Record and field do not match");
            return false;
        }
        return true;
    }

private:
    std::map<std::string, std::tuple<Args...>> table_;
    std::map<std::string, std::vector<std::string>> field_;
};

}

#endif
