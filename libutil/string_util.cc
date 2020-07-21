#include <iterator>
#include <regex>
#include "string_util.h"

namespace infra::util {

void String::RTrim(std::string& str)
{
    if (str.empty()) {
        return ;
    }
    str.erase(0, str.find_first_not_of(" "));
}

void String::LTrim(std::string& str)
{
    if (str.empty()) {
        return;
    }
    str.erase(str.find_last_not_of(" ") + 1);
}

void String::Trim(std::string& str)
{
    if (str.empty()) {
        return;
    }
    str.erase(0, str.find_first_not_of(" "));
    str.erase(0, str.find_first_not_of(" "));
}

void String::Reverse(std::string& str)
{

}

void String::Distinct(std::string& str)
{
    if (str.size() < 2) {
        return;
    }
    std::string::iterator last = str.begin();
    std::string::iterator it = last + 1;
    for ( ; it != str.end(); ) {
        if (*last == *it) {
            it = str.erase(it);
        } else {
            last = it;
            it++;
        }
    }
}

void String::Distinct(std::string& str, char target)
{
    if (str.size() < 2) {
        return;
    }
    std::string::iterator last = str.begin();
    std::string::iterator it = last + 1;
    for ( ; it != str.end(); ) {
        if (*last == *it && *last == target) {
            it = str.erase(it);
        } else {
            last = it;
            it++;
        }
    }
}

void String::Distinct(std::string& str, std::string& target)
{

}

void String::Split(const std::string& str, std::vector<std::string>& vec)
{
    std::string sep = " ";
    Split(str, sep, vec);
}

void String::Split(const std::string& str, std::string& sep, std::vector<std::string>& vec)
{
    Split(str, std::move(sep), vec);
}
void String::Split(const std::string& str, std::string&& sep, std::vector<std::string>& vec)
{
    std::string expr = "[^" + sep + "]+";
    std::regex word_regex(expr);
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), word_regex);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        vec.push_back(match_str);
    }
}

}
