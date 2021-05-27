#ifndef __TIMER_SCHEDUE_CRONRULE_H__
#define __TIMER_SCHEDUE_CRONRULE_H__

#include <string>
#include <vector>
#include <list>
#include <regex>

#include "container_bitmap.h"

#include "timer_time.h"
#include "timer_schedue.h"

namespace infra::timer {

/**
* @brief - Crontab rule class
*   Example:
*       * * * *  *  * * *
*       Y M D WM WD H M S
*/
class Crontab : virtual public infra::base::Object, public Schedue::Rule {
public:
    enum Field : int {
        Year = 0,
        Month = 1,
        DayOfMonth = 2,
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
    Crontab(std::string rule);
    ~Crontab();

    bool Valid();
    Return Next(Date&& curr_time, Date& next_time);

    static std::string FieldToString(Field&& field);

private:
    bool _parse_rule();
    Date::Unit _field2date(Field&& field);

    std::string rule_;
    std::vector<SubRule> sub_rule_vec_;
    bool skip_DayOfMonth_;
    bool skip_DayOfWeek_;
    bool skip_Month_;
    bool skip_Week_;
};


}

#endif
