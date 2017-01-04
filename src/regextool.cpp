#include"regextool.h"
#include<iostream>
#include<string>
#include<exception>

//search all matches by regex explaination
std::vector<std::smatch*> regex_searchAll(
        const std::regex& regex,
        const std::string& target)
{
    std::smatch* match;
    std::vector<std::smatch*> result;
    for(auto it=target.cbegin();
            match=new std::smatch,
            std::regex_search(it,target.cend(),*match,regex);
            it+=match->position(0)+match->length(0)
       )
    {
        result.push_back(match);
    }
#ifdef DEBUG
    std::cout << "hit " << result.size() << " items" << std::endl;
#endif
    return result;
}

//search first match
std::vector<std::smatch*> regex_searchOne(
        const std::regex& regex,
        const std::string& target)
{
    std::smatch *match;
    std::vector<std::smatch*> result;
    for(auto it=target.cbegin();
            match = new std::smatch,
            std::regex_search(it,target.cend(),*match,regex);
            it+=match->position(0)+match->length(0)
       )
    {
        result.push_back(match);
        break;
    }
    return result;
}

void clearSmatch(std::vector<std::smatch*>& vec)
{
    for(auto item : vec)
    {
        if(item!=NULL)
            delete item;
    }
}
