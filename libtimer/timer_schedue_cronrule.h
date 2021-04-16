#ifndef __TIMER_SCHEDUE_CRONRULE_H__
#define __TIMER_SCHEDUE_CRONRULE_H__

#include <string>
#include <vector>
#include <list>
#include <regex>

#include "container_bitmap.h"

#include "timer_date.h"
#include "timer_schedue.h"

namespace infra::timer {

class CronRule : virtual public infra::base::Object, public Schedue::Rule {
public:
    enum class Field : int {
        Year = 0,
        Month,
        DayOfMonth,
        Week,
        DayOfWeek,
        Hour,
        Minute,
        Second,
    };
    class SubRule {
    public:
        enum class Type : int {
            Frequency = 1,
            FrequencyRange,
            Range,
            Value,
            Fault,
        };
    public:
        SubRule(Field&& field, std::string sub_rule);
        ~SubRule();

        bool Valid();
        Return Next(int curr, int& next);
    private:
        bool _parse_sub_rule();

        Field field_;
        std::string sub_rule_;
        infra::container::Bitmap type_;
        std::vector<int> frequency_vec_;
        std::vector<int> value_vec_;

        static std::regex RegexSyntax;
        static std::regex RegexRuleFrequency;
        static std::regex RegexRuleFrequencyRange;
        static std::regex RegexRuleRange;
        static std::regex RegexRuleValue;
    };
public:
    CronRule(std::string rule);
    ~CronRule();

    bool Valid();
    Return Next(Date&& curr_time, Date& next_time);
private:
    bool _parse_rule();
    Date::Unit _field2date(Field&& field);

    std::string rule_;
    std::vector<SubRule> sub_rule_vec_;
};

}

#endif
