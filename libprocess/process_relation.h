#ifndef __PROCESS_RELATION_H__
#define __PROCESS_RELATION_H__

#include <string>
#include <tuple>
#include <vector>

#include "object.h"

namespace infra::process {

class Relation : virtual public base::Object {
public:
    enum class Type : unsigned int {
        Normal = 0,
        Direct,
        Adopt,
        Parent,
        Child,
    };
private:
    static std::vector<std::tuple<Relation::Type, std::string, std::string>> TypeDescription;
public:
    Relation();
    Relation(Type&& type);
    Relation(std::vector<Type>&& relations);
    Relation(const Relation&);
    Relation(const Relation&&);
    ~Relation();

    Relation& operator=(const Relation& other);

    Relation operator+(Relation& relation);
    Relation operator+(Relation&& relation);
    Relation operator+(Type&& type);
    Relation& operator+=(Relation& relation);
    Relation& operator+=(Relation&& relation);
    Relation& operator+=(Type&& type);

    Relation operator-(Relation& relation);
    Relation operator-(Relation&& relation);
    Relation operator-(Type&& type);
    Relation& operator-=(Relation& relation);
    Relation& operator-=(Relation&& relation);
    Relation& operator-=(Type&& type);

    Relation& RelationAdd(Relation& relation);
    Relation& RelationAdd(Relation&& relation);
    Relation& RelationAdd(Type&& type);
    Relation& RelationDelete(Relation& relation);
    Relation& RelationDelete(Relation&& relation);
    Relation& RelationDelete(Type&& type);

    bool Has(Relation& relation);
    bool Has(Relation&& relation);
    bool Has(Type&& type);
    bool Has(std::vector<Type>&& relations);

private:
    void _BitSet(Type&& type);
    void _BitSet(unsigned long long && bitset);
    void _BitUnset(Type&& type);
    void _BitUnset(unsigned long long && bitset);
    bool _BitCheck(Type&& type);
    bool _BitCheck(unsigned long long && bitset);

    unsigned long long relation_ = 0;
};

}

#endif
