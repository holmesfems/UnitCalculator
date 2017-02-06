#include "stringTool.h"
#include <sstream>
//#define DEBUG
namespace StringTool
{
    std::vector<std::string> strSplit(
            const std::string& s,
            const std::string& delim)
    {
    #ifdef DEBUG
        std::cout << "calling strsplit( "<<s<<" , "<<delim<<" )\n";
    #endif
        std::vector<std::string> result;
        std::string::size_type pos=0;
        while(pos!=std::string::npos)
        {
            std::string::size_type p = s.find(delim,pos);
            if(p==std::string::npos)
            {
                result.push_back(s.substr(pos));
                break;
            }
            else
            {
                result.push_back(s.substr(pos,p-pos));
            }
            pos=p+delim.size();
        }
        return result;
    }

    std::vector<std::string> strsep(const std::string &str,int locate,int length)
    {
    #ifdef DEBUG
        std::cout << "calling strsep(" << str << "," << locate << "," << length << ")" << std::endl;
    #endif
        std::vector<std::string> result;
        result.push_back(str.substr(0,locate));
        result.push_back(str.substr(locate+length,str.length()-locate-length));
    #ifdef DEBUG
        std::cout <<"result:" << result[0] << " | " << result[1] << std::endl;
    #endif
        return result;
    }

    double convertToDouble(const std::string &str)
    {
        std::istringstream iss;
        iss.str(str);
        double res;
        iss >> res;
        return res;
    }

    int convertToInt(const std::string &str)
    {
        std::istringstream iss;
        iss.str(str);
        int res;
        iss >> res;
        return res;
    }
}
