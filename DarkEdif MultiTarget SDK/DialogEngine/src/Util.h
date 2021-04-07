#pragma once
#include <string>
#include <sstream> 
#include <vector>

class Util
{
public:

    // Static Methods
    static int StringToInt(std::string str);
    static float StringToFloat(std::string str);
    static std::string RemoveFromBeginning(std::string str, int numberOfCharacters);
    static std::string GetSubstringBetweenQuotes(std::string str);
    static std::string GetSubstringBeforeEquals(std::string str);
    static std::string GetSubstringAfterEquals(std::string str);
    static std::string GetSubstringAfterColon(std::string str);
    static std::string GetSubstringBeforeColon(std::string str);
    static std::string GetSubstringAfterSecondQuote(std::string str);
    static std::string RemoveWhitespace(std::string str);
    static std::string GetOperator(std::string str);
    static float GetValueAfterOperator(std::string str);
    static bool StringContains(std::string str, std::string query);
    static float GetFloatBetweenParentheses(std::string str);
    static std::string GetSubstringAfterChar(std::string str, char c);
    static std::string GetSubstringBeforeChar(std::string str, char c);
    static std::string GetSubstringAfterSubstring(std::string str, std::string query);
    static std::string GetSubstringBeforeSubstring(std::string str, std::string query);
    static std::string GetSubstringBeforeParentheses(std::string query);

    // trim from left
    static inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    // trim from right
    static inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from left & right
    static inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        return ltrim(rtrim(s, t), t);
    }
};
