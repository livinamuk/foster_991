#pragma once
#include "Util.h"

int Util::StringToInt(std::string str)
{
    if (str.length() == 0)
        return 0;

    std::stringstream stringStream(str);
    int result = 0;
    stringStream >> result;
    return result;
}

float Util::StringToFloat(std::string str)
{
    if (str.length() == 0)
        return 0;

    return std::stof(str);
}

std::string Util::RemoveFromBeginning(std::string str, int numberOfCharacters)
{
    return str.substr(numberOfCharacters, str.length() - 1);
}

std::string Util::GetSubstringBetweenQuotes(std::string str)
{
    std::string substringAfterFirstQuote = str.substr(str.find('"') + 1);
    return substringAfterFirstQuote.substr(0, substringAfterFirstQuote.find('"'));
}


std::string Util::GetSubstringAfterChar(std::string str, char c)
{
    return RemoveWhitespace(str.substr(str.find(c) + 1));
}

std::string Util::GetSubstringBeforeChar(std::string str, char c)
{
    int index = str.find(c);

    if (index == -1)
        return "";
    else
        return RemoveWhitespace(str.substr(0, index));
}

std::string Util::GetSubstringBeforeSubstring(std::string str, std::string query)
{
    int index = str.find(query);

    if (index == -1)
        return "";
    else
        return RemoveWhitespace(str.substr(0, index));
}

std::string Util::GetSubstringBeforeParentheses(std::string query)
{
    int index = query.find("(");
    if (index == -1) return query;
    else return query.substr(0, index);
}

std::string Util::GetSubstringAfterSubstring(std::string str, std::string query)
{
    return RemoveWhitespace(str.substr(str.find(query) + query.length()));
}


std::string Util::GetSubstringAfterEquals(std::string str)
{
    std::string substringAfterFirstQuote = str.substr(str.find('"') + 1);
    return RemoveWhitespace(str.substr(str.find("=") + 1));
}

std::string Util::GetSubstringAfterColon(std::string str)
{
    return RemoveWhitespace(str.substr(str.find(':') + 1));
}

std::string Util::GetSubstringBeforeColon(std::string str)
{
    int index = str.find(':');

    if (index == -1)
        return "";
    else
        return RemoveWhitespace(str.substr(0, index));
}

std::string Util::GetSubstringBeforeEquals(std::string str)
{
    int index = str.find('=');

    if (index == -1)
        return "";
    else
        return RemoveWhitespace(str.substr(0, index));
}

std::string Util::GetSubstringAfterSecondQuote(std::string str)
{
    std::string substringAfterFirstQuote = str.substr(str.find('"') + 1);
    return RemoveWhitespace(substringAfterFirstQuote.substr(substringAfterFirstQuote.find('"') + 1));
}

std::string Util::RemoveWhitespace(std::string str)
{    
    return trim(str);
}

std::string Util::GetOperator(std::string str)
{
    std::string cleanStr = RemoveWhitespace(str);

    if (cleanStr[0] == '+')
        return "+";
    else if (cleanStr[0] == '-')
        return "-";
    else if (cleanStr[0] == 'x')
        return "x";
    else if (cleanStr.substr(0, 2) == "<=")
        return "<=";
    else if (cleanStr.substr(0, 2) == ">=")
        return ">=";
    else if (cleanStr.substr(0, 2) == "==")
        return "==";
    else if (cleanStr.substr(0, 2) == "!=")
        return "!=";
    else
        return "";
}

float Util::GetValueAfterOperator(std::string str)
{
    std::string cleanStr = RemoveWhitespace(str);
    std::string opperatorStr = GetOperator(cleanStr);
    std::string valueStr = cleanStr.substr(opperatorStr.length());
    std::string cleanValueStr = RemoveWhitespace(valueStr);
  
    if (cleanValueStr.length() == 0)
        return 0.0f;

    return std::stof(cleanValueStr);
}

bool Util::StringContains(std::string str, std::string query)
{
    if (str.find(query) != std::string::npos)
        return true;
    else
        return false;
}

float Util::GetFloatBetweenParentheses(std::string query)
{
    int left = query.find("(");
    int right = query.find(")");

    if (left == -1 || right == -1)
        return 0;

    if (left == right - 1)
        return 0;

    return StringToFloat(query.substr(left + 1, right - left + 1));
}