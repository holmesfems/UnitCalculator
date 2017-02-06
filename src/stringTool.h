#ifndef _STRINGTOOL_H
#define _STRINGTOOL_H
#include<vector>
#include<string>
namespace StringTool
{
    std::vector<std::string> strSplit(const std::string& s,const std::string& delim);
    std::vector<std::string> strsep(std::string str,int locate,int length);
}
#endif
