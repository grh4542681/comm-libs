#include <regex>
#include <algorithm>

#include "string_util.h"

#include "timer_log.h"
#include "timer_return.h"
#include "timer_schedue_crontab.h"

namespace infra::timer {

std::regex Crontab::SubRule::RegexSyntax("([0-9]|[\\/]|[\\*]|[\\-]|[\\,])*");
std::regex Crontab::SubRule::RegexRuleFrequency("(\\*)");
std::regex Crontab::SubRule::RegexRuleFrequencyRange("\\*[\\/]([0-9]+)");
std::regex Crontab::SubRule::RegexRuleRange("[0-9]+\\-[0-9]+");
std::regex Crontab::SubRule::RegexRuleValue("[0-9]+(\\,[0-9]+)*");

Crontab::SubRule::SubRule(Field&& field, std::string sub_rule)
        : field_(field), sub_rule_(sub_rule), type_(static_cast<int>(Type::Fault) + 1)
{

    if (!_parse_sub_rule()) {
        value_vec_.clear();
        frequency_vec_.clear();
    }
    for (auto it : frequency_vec_) {
        Log::Debug("    f[", it, "]");
    }
    for (auto it : value_vec_) {
        Log::Debug("    v[", it, "]");
    }
}

Crontab::SubRule::~SubRule() { }

Return Crontab::SubRule::Next(int curr, int& next)
{
    bool buckle_flag = false;
    if (!Valid()) {
        return Return::TIMER_SCHEDUE_RULE_INVALID;
    }
    if (frequency_vec_.size() > 0) {
        next = curr + frequency_vec_[0];
        switch (field_) {
            case Field::Year:
                break;
            case Field::Month:
                if (next > 12) {
                    buckle_flag = true;
                    next %= 12;
                }
                break;
            case Field::DayOfMonth:
                if (next > 31) {
                    buckle_flag = true;
                    next %= 31;
                }
                break;
            case Field::Week:
                if (next > 56) {
                    buckle_flag = true;
                    next %= 56;
                }
                break;
            case Field::DayOfWeek:
                if (next > 7) {
                    buckle_flag = true;
                    next %= 7;
                }
                break;
            case Field::Hour:
                if (next > 24) {
                    buckle_flag = true;
                    next %= 24;
                }
                break;
            case Field::Minute:
                if (next >= 60) {
                    buckle_flag = true;
                    next %= 60;
                }
                break;
            case Field::Second:
                if (next > 60) {
                    buckle_flag = true;
                    next %= 60;
                }
                break;
            default:
                return Return::TIMER_SCHEDUE_RULE_FIELD_INVALID;
        }
    } else if (value_vec_.size() > 0) {
        buckle_flag = true;
        for (auto it : value_vec_) {
            if (it > curr) {
                next = it;
                buckle_flag = false;
                break;
            }
        }
        if (buckle_flag) {
            next = value_vec_[0];
        }
    }
    return (buckle_flag ? Return::TIMER_SCHEDUE_RULE_BUCKLE : Return::SUCCESS);
}

bool Crontab::SubRule::Valid()
{
    return (value_vec_.size() != 0 || frequency_vec_.size() != 0);
}

bool Crontab::SubRule::_parse_sub_rule()
{
    if (!sub_rule_.size()) {
        return false;
    }
    if (!std::regex_match(sub_rule_, RegexSyntax)) {
        Log::Error("Sub rule[", sub_rule_, "] syntax check failed");
        return false;
    }

    infra::util::String::Trim(sub_rule_);
    std::vector<std::string> sub_rule_field_vec;
    infra::util::String::Split(sub_rule_, ",", sub_rule_field_vec);

    Log::Info("Sub rule[", sub_rule_, "]");
    for (auto sub_rule_field : sub_rule_field_vec) {
        if (std::regex_match(sub_rule_field, RegexRuleFrequency)) {
            Log::Debug("Frequency rule [", sub_rule_field, "]");
            frequency_vec_.push_back(1);
            type_.Set(static_cast<int>(Type::Frequency));
        } else if (std::regex_match(sub_rule_field, RegexRuleFrequencyRange)) {
            Log::Debug("Frequency range rule [", sub_rule_field, "]");
            std::smatch sm;
            if (!std::regex_search(sub_rule_field, sm, std::regex("([0-9]+)"))) {
                Log::Error("Not found frequency range in rule[" , sub_rule_field, "]");
                return false;
            }
            if (sm.size() != 2) {
                Log::Error("Not found right range in rule[" , sub_rule_field, "]");
                return false;
            }
            frequency_vec_.push_back(std::stoi(sm.str(1)));
            type_.Set(static_cast<int>(Type::FrequencyRange));
        } else if (std::regex_match(sub_rule_field, RegexRuleRange)) {
            Log::Debug("Range rule [", sub_rule_field, "]");
            std::smatch sm;
            if (!std::regex_search(sub_rule_field, sm, std::regex("([0-9]+)\\-([0-9]+)"))) {
                Log::Error("Not found range in rule[" , sub_rule_field, "]");
                return false;
            }
            if (sm.size() != 3) {
                Log::Error("Not found right range in rule[" , sub_rule_field, "]");
                return false;
            }
            int fa_s = std::stoi(sm.str(1));
            int fa_e = std::stoi(sm.str(2));
            for (int i = fa_s; i <= fa_e; i++) {
                value_vec_.push_back(i);
            }
            type_.Set(static_cast<int>(Type::Range));
        } else if (std::regex_match(sub_rule_field, RegexRuleValue)) {
            Log::Debug("Value rule [", sub_rule_field, "]");
            std::smatch sm;
            std::string tmp_rule = sub_rule_field;
            while(std::regex_search(tmp_rule, sm, std::regex("([0-9]+)"))) {
              value_vec_.push_back(std::stoi(sm.str()));
              tmp_rule = sm.suffix();
            }
            type_.Set(static_cast<int>(Type::Value));
        } else {
            Log::Debug("Fault rule [", sub_rule_field, "]");
            return false;
        }
    }

    std::sort(frequency_vec_.begin(), frequency_vec_.end());
    frequency_vec_.erase(std::unique(frequency_vec_.begin(), frequency_vec_.end()), frequency_vec_.end());
    std::sort(value_vec_.begin(), value_vec_.end());
    value_vec_.erase(std::unique(value_vec_.begin(), value_vec_.end()), value_vec_.end());

    if (value_vec_.size() > 0 && frequency_vec_.size() > 0) {
        Log::Error("Both frequency and value are specified in the rule");
        return false;
    }
    if (frequency_vec_.size() > 1) {
        Log::Error("Not support mutli-frequency rule");
        return false;
    }

    for (auto value : value_vec_) {
        switch (field_) {
            case Field::Year:
                break;
            case Field::Month:
                if (value > 12) {
                    Log::Error("Bad value[", value, "] for Month");
                    return false;
                }
                break;
            case Field::DayOfMonth:
                if (value > 31) {
                    Log::Error("Bad value[", value, "] for DayOfMonth");
                    return false;
                }
                break;
            case Field::Week:
                if (value > 56) {
                    Log::Error("Bad value[", value, "] for Week");
                    return false;
                }
                break;
            case Field::DayOfWeek:
                if (value > 7) {
                    Log::Error("Bad value[", value, "] for DayOfWeek");
                    return false;
                }
                break;
            case Field::Hour:
                if (value > 24) {
                    Log::Error("Bad value[", value, "] for Hour");
                    return false;
                }
                break;
            case Field::Minute:
                if (value > 60) {
                    Log::Error("Bad value[", value, "] for Minute");
                    return false;
                }
                break;
            case Field::Second:
                if (value > 60) {
                    Log::Error("Bad value[", value, "] for Second");
                    return false;
                }
                break;
            default:
                return false;
        }
    }

    return true;
}

Crontab::Crontab(std::string rule) : rule_(rule),
        skip_DayOfMonth_(false), skip_DayOfWeek_(false),
        skip_Month_(false), skip_Week_(false)
{
    _parse_rule();
}

Crontab::~Crontab() { }

bool Crontab::Valid() {
    return (sub_rule_vec_.size() == (static_cast<int>(Field::Second) + 1));
}

Return Crontab::Next(Date&& curr, Date& next) {
    if (!Valid()) {
        return Return::TIMER_SCHEDUE_RULE_INVALID;
    }
    next = curr;
    for (int i = static_cast<int>(Field::Second); i>= static_cast<int>(Field::Year); i--) {
        if (i == static_cast<int>(Field::Week) && skip_Week_) continue;
        if (i == static_cast<int>(Field::Month) && skip_Month_) continue;
        if (i == static_cast<int>(Field::DayOfWeek) && skip_DayOfWeek_) continue;
        if (i == static_cast<int>(Field::DayOfMonth) && skip_DayOfMonth_) continue;
        int next_value;
        auto ret = sub_rule_vec_[i].Next(next.Get(_field2date(static_cast<Field>(i))), next_value);
        next.Set(next_value, _field2date(std::move(static_cast<Field>(i))));
        if (ret == Return::SUCCESS) {
            break;
        } else if (ret == Return::TIMER_SCHEDUE_RULE_BUCKLE) {
            continue;
        } else {
            return ret;
        }
    }
    return Return::SUCCESS;
}

Date::Unit Crontab::_field2date(Field&& field)
{
    switch (field) {
        case Field::Year:
            return Date::Unit::Year;
        case Field::Month:
            return Date::Unit::Month;
        case Field::DayOfMonth:
            return Date::Unit::DayOfMonth;
        case Field::Week:
            return Date::Unit::Week;
        case Field::DayOfWeek:
            return Date::Unit::DayOfWeek;
        case Field::Hour:
            return Date::Unit::Hour;
        case Field::Minute:
            return Date::Unit::Minute;
        case Field::Second:
            return Date::Unit::Second;
        default:
            return Date::Unit::Second;
    }
}

bool Crontab::_parse_rule()
{
    infra::util::String::Trim(rule_);
    std::vector<std::string> rule_field_vec;
    infra::util::String::Split(rule_, " ", rule_field_vec);
    if (rule_field_vec.size() != (static_cast<int>(Field::Second) + 1)) {
        Log::Error("Wrong number of cron rule fields");
        return false;
    }
    if (rule_field_vec[static_cast<int>(Field::Week)] == "*" &&
        rule_field_vec[static_cast<int>(Field::Month)] == "*") {
        skip_Week_ = true;
    } else if (rule_field_vec[static_cast<int>(Field::Month)] == "*") {
        skip_Month_ = true;
    } else if (rule_field_vec[static_cast<int>(Field::Week)] == "*") {
        skip_Week_ = true;
    } else {
        Log::Error("Both define Month and Week");
        return false;
    }
    if (rule_field_vec[static_cast<int>(Field::DayOfWeek)] == "*" &&
        rule_field_vec[static_cast<int>(Field::DayOfMonth)] == "*") {
        skip_DayOfWeek_ = true;
    } else if (rule_field_vec[static_cast<int>(Field::DayOfMonth)] == "*") {
        skip_DayOfMonth_ = true;
    } else if (rule_field_vec[static_cast<int>(Field::DayOfWeek)] == "*") {
        skip_DayOfWeek_ = true;
    } else {
        Log::Error("Both define DayofMonth and DayofWeek");
        return false;
    }
    int loop = 0;
    for (auto sub_rule_str : rule_field_vec) {
        sub_rule_vec_.push_back(SubRule(static_cast<Field>(loop), sub_rule_str));
        if (!sub_rule_vec_.back().Valid()) {
            sub_rule_vec_.clear();
            return false;
        }
        loop++;
    }
    return true;
}

std::string Crontab::FieldToString(Crontab::Field&& field)
{
    std::string str;
    switch (field) {
        case Crontab::Field::Year:
            str = "Year";
            break;
        case Crontab::Field::Month:
            str = "Month";
            break;
        case Crontab::Field::DayOfMonth:
            str = "DayOfMonth";
            break;
        case Crontab::Field::Week:
            str = "Week";
            break;
        case Crontab::Field::DayOfWeek:
            str = "DayOfWeek";
            break;
        case Crontab::Field::Hour:
            str = "Hour";
            break;
        case Crontab::Field::Minute:
            str = "Minute";
            break;
        case Crontab::Field::Second:
            str = "Second";
            break;
        default:
            str = "";
            break;
    }
    return str;
}

}
