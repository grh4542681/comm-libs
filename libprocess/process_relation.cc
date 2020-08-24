#include "process_relation.h"

namespace infra::process {

std::vector<std::tuple<Relation::Type, std::string, std::string>> Relation::TypeDescription = {
    //   type        string          describe
    { Type::Normal, "Normal", "Default relationrelation" },
    { Type::Direct, "Direct", "The process of having immediate family relations" },
    { Type::Adopt,  "Adopt",  "The process of having an adopted relationrelation" },
    { Type::Parent, "Parent", "Parent process" },
    { Type::Child,  "Child",  "Child process" },
};

Relation::Relation() { }
Relation::Relation(Relation::Type&& type)
{
    _BitSet(std::forward<Relation::Type>(type));
}
Relation::Relation(std::vector<Relation::Type>&& relations)
{
    for(auto type : relations) {
        _BitSet(std::move(type));
    }
}
Relation::Relation(const Relation& other)
{
    relation_ = other.relation_;
}
Relation::Relation(const Relation&& other)
{
    relation_ = other.relation_;
}
Relation::~Relation() { }

Relation& Relation::operator=(const Relation& other)
{
    relation_ = other.relation_;
    return *this;
}

Relation Relation::operator+(Relation& relation)
{
    Relation other(*this);
    other._BitSet(std::move(relation.relation_));
    return other;
}
Relation Relation::operator+(Relation&& relation)
{
    Relation other(*this);
    other._BitSet(std::move(relation.relation_));
    return other;
}
Relation Relation::operator+(Relation::Type&& type)
{
    Relation other(*this);
    other._BitSet(std::forward<Relation::Type>(type));
    return other;
}
Relation& Relation::operator+=(Relation& relation)
{
    _BitSet(std::move(relation.relation_));
    return *this;
}
Relation& Relation::operator+=(Relation&& relation)
{
    _BitSet(std::move(relation.relation_));
    return *this;
}
Relation& Relation::operator+=(Relation::Type&& type)
{
    _BitSet(std::forward<Relation::Type>(type));
    return *this;
}

Relation Relation::operator-(Relation& relation)
{
    Relation other(*this);
    other._BitUnset(std::move(relation.relation_));
    return other;
}
Relation Relation::operator-(Relation&& relation)
{
    Relation other(*this);
    other._BitUnset(std::move(relation.relation_));
    return other;
}
Relation Relation::operator-(Relation::Type&& type)
{
    Relation other(*this);
    other._BitUnset(std::forward<Relation::Type>(type));
    return other;
}
Relation& Relation::operator-=(Relation& relation)
{
    _BitUnset(std::move(relation.relation_));
    return *this;
}
Relation& Relation::operator-=(Relation&& relation)
{
    _BitUnset(std::move(relation.relation_));
    return *this;
}
Relation& Relation::operator-=(Relation::Type&& type)
{
    _BitUnset(std::forward<Relation::Type>(type));
    return *this;
}

Relation& Relation::RelationAdd(Relation& relation)
{
    _BitSet(std::move(relation.relation_));
    return *this;
}
Relation& Relation::RelationAdd(Relation&& relation)
{
    _BitSet(std::move(relation.relation_));
    return *this;
}
Relation& Relation::RelationAdd(Relation::Type&& type)
{
    _BitSet(std::forward<Relation::Type>(type));
    return *this;
}
Relation& Relation::RelationDelete(Relation& relation)
{
    _BitUnset(std::move(relation.relation_));
    return *this;
}
Relation& Relation::RelationDelete(Relation&& relation)
{
    _BitUnset(std::move(relation.relation_));
    return *this;
}
Relation& Relation::RelationDelete(Relation::Type&& type)
{
    _BitUnset(std::forward<Relation::Type>(type));
    return *this;
}

bool Relation::Has(Relation& relation)
{
    return Has(std::move(relation));
}
bool Relation::Has(Relation&& relation)
{
    return _BitCheck(std::move(relation.relation_));
}
bool Relation::Has(Relation::Type&& type)
{
    return _BitCheck(std::move(type));
}
bool Relation::Has(std::vector<Relation::Type>&& relations)
{
    for (auto type : relations) {
        if (!_BitCheck(std::move(type))) {
            return false;
        }
    }
    return true;
}

void Relation::_BitSet(Relation::Type&& type)
{

}
void Relation::_BitSet(unsigned long long && bitset)
{

}
void Relation::_BitUnset(Relation::Type&& type)
{

}
void Relation::_BitUnset(unsigned long long && bitset)
{

}
bool Relation::_BitCheck(Relation::Type&& type)
{
    return true;
}
bool Relation::_BitCheck(unsigned long long && bitset)
{
    return true;
}

}
