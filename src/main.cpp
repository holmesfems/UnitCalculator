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
    std::string homedir=getenv("HOME");
    std::string rcdir=homedir+"/.bnrc";
    if(readScript(rcdir))
        readScript("consts.txt");
    while(1)
    {
        count++;
        char *text;
        os.str("");
        os.clear(std::stringstream::goodbit);
        os << "\033[35;1mIn [" << count << "]:\033[0m";
        text=readline(os.str().c_str());
        if(text==NULL)
        {
            std::cout << std::endl;
            break;
        }
        cmd=text;
        std::cout << "\033[32;1mOut[" << count<< "]:\033[0m";
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
