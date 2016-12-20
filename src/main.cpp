#include"basicNumeric.h"
#include"stdio.h"
#include<iostream>
#include<string>
#include<stdio.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<stdlib.h>
#include<sstream>

int main()
{
    std::string cmd;
    initialize();
    using_history();
    read_history(NULL);
    int count=0;
    std::ostringstream os;
    std::string homedir;
    std::string rcdir;
    char* hdcp=getenv("HOME");
    if(hdcp==NULL)
        readScript("consts.txt");
    else
    {
        homedir=hdcp;
        rcdir=homedir+"/.bnrc";
        if(readScript(rcdir))
            readScript("consts.txt");
    }
    while(1)
    {
        count++;
        char *text;
        os.str("");
        os.clear(std::stringstream::goodbit);
        os << COLOR_MAGENTA << "In [" << count << "]:" << COLOR_NORMAL;
        text=readline(os.str().c_str());
        if(text==NULL)
        {
            std::cout << std::endl;
            break;
        }
        cmd=text;
        std::cout << COLOR_GREEN << "Out[" << count<< "]:" << COLOR_NORMAL;
        if(!(seperateCmd(cmd)))
        {
            free(text);
            break;
        }
        add_history(text);
        free(text);
    };
    return 0;
}
