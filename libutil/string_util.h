#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <string>
#include <vector>

namespace infra {

class StringUtil {
public:

    static void RTrim(std::string& str);
    static void LTrim(std::string& str);
    static void Trim(std::string& str);

    static void Reverse(std::string& str);

    static void Distinct(std::string& str);
    static void Distinct(std::string& str, char target);
    static void Distinct(std::string& str, std::string& target);

    static void Split(const std::string& str, std::vector<std::string>& vec);
    static void Split(const std::string& str, std::string& sep, std::vector<std::string>& vec);
    static void Split(const std::string& str, std::string&& sep, std::vector<std::string>& vec);

private:
    StringUtil();
    ~StringUtil();
};

}


#endif
