#include"basicNumeric.h"
#include"stdio.h"
#include<regex>
#include"math.h"
#include<iostream>
#include<string>
#include<stdio.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<stdlib.h>
#include<exception>
#include<cmath>
#include<fstream>
#include<sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
//#define DEBUG

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

std::vector<std::string> strsep(std::string str,int locate,int length)
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

//PhyBaseFormat of values
PhyTypeDict* _phyTypeDict=NULL;

//PhyBaseUnit
//nothing to do
char _inverseMD=0;

//PhyBaseUnitDict
PhyBaseUnit* PhyBaseUnitDict::searchByName(const std::string& name)
{
    int i;
    PhyBaseUnit* res=NULL;
    for(i=0;i<_dict.size();i++)
    {
        if(name==_dict[i]->getName())
        {
            res=_dict[i];
            break;
        }
    }
    return res;
}

PhyBaseUnit* PhyBaseUnitDict::searchByIndex(int index)
{
    if(index<_dict.size())
        return _dict[index];
    return NULL;
}

int PhyBaseUnitDict::changeDefault(const std::string& defaultUnit)
{
    PhyBaseUnit* newDefault;
    newDefault=this->searchByName(defaultUnit);
    if(newDefault==NULL)
    {
        return 1;
    }
    _defaultUnit=newDefault;
    return 0;
}

int PhyBaseUnitDict::changeDefault(int index)
{
    if(index < _dict.size())
    {
        _defaultUnit=_dict[index];
        return 0;
    }
    return 1;
}

//PhyTypeDict
PhyBaseUnitDict* PhyTypeDict::searchDictByName(const std::string& name)
{
    int i;
    PhyBaseUnitDict* res=NULL;
#ifdef DEBUG
    std::cout << "calling PhyTypeDict::searchDictByName("<<name<<")\n";
#endif
    for(i=0;i<_dict.size();i++)
    {
        if(name==_dict[i]->getType())
        {
            res=_dict[i];
            break;
        }
    }
#ifdef DEBUG
    if(res!=NULL)
        std::cout << "Hit!\n";
    else
        std::cout << "Not Hit!\n";
#endif
    return res;
}

PhyBaseUnit* PhyTypeDict::searchUnitByName(const std::string& name)
{
    PhyBaseUnit* res=NULL;
    for(auto item1 : _dict)
    {
        if((res=item1->searchByName(name))!=NULL)
            break;
    }
    return res;
}

//PhyBaseUnitDemension

PhyBaseUnitDemension::PhyBaseUnitDemension(const PhyBaseUnitDemension& phyBaseUnitDemension)
{
    unitType=phyBaseUnitDemension.unitType;
    demension=phyBaseUnitDemension.demension;
}

PhyBaseUnitDemension PhyBaseUnitDemension::operator=(const PhyBaseUnitDemension& a)
{
    this->unitType=a.unitType;
    this->demension=a.demension;
    return PhyBaseUnitDemension(a);
}

//PhyUnit
PhyUnit::PhyUnit(const std::string& unitStr)
{
    std::regex numeric(R"(^\d+(\.\d+)?$)");
    std::regex bracket(R"(^\([^\)]*\)$)");
    for(auto item:strSplit(unitStr,"*"))
    {
        std::vector<std::string> strsplit;
        strsplit=strSplit(item,"^");
        if(strsplit.size()>2)
        {
            throw compileException("compile error:unit string");
        }
        if(strsplit[0]=="")
        {
            throw compileException("compile error:unit string,blank type");
        }
        for(auto item2:_unitDemension)
        {
            if(item2.unitType==strsplit[0])
            {
                throw compileException("compile error:unit string,unit type set twice");
            }
        }
        double demension;
        if(strsplit.size()==1)
        {
            demension=1.0;
        }
        else
        {
            std::string numericStr;
            numericStr=strsplit[1];
            if(std::regex_match(numericStr,bracket))
            {
                numericStr=numericStr.substr(1,numericStr.length()-2);
            }
            if(!(std::regex_match(numericStr,numeric)))
            {
                throw compileException("compile error: unit string,demension");
            }
            sscanf(numericStr.c_str(),"%lf",&demension);
        }
        PhyBaseUnitDemension pbud;
        pbud.demension=demension;
        pbud.unitType=strsplit[0];
        _unitDemension.push_back(pbud);
    }
}

PhyUnit::PhyUnit(const PhyUnit& phyUnit)
{
    for(auto item:phyUnit.getDict())
    {
        if(item.demension!=0.0)
        {
            _unitDemension.push_back(item);
        }
    }
}

double PhyUnit::getDemension(const std::string& demensionType) const
{
    double res=0.0;
    for(auto item : _unitDemension)
    {
        if(item.unitType==demensionType)
        {
            res=item.demension;
            break;
        }
    }
    return res;
}

void PhyUnit::setDemension(double demension,const std::string& demensionType)
{
    for(auto item : _unitDemension)
    {
        if(item.unitType==demensionType)
        {
            item.demension=demension;
            return;
        }
    }
    //add new
    PhyBaseUnitDemension pbud;
    pbud.demension=demension;
    pbud.unitType=demensionType;
    _unitDemension.push_back(pbud);
}

unsigned char PhyUnit::isNumeric() const
{
    unsigned char res=1;
    for(auto item:_unitDemension)
    {
        if(item.demension!=0.0)
        {
            res=0;
            break;
        }
    }
    return res;
}

std::string PhyUnit::str() const
{
    std::ostringstream res;
    unsigned char first=1;
    for(auto item:_unitDemension)
    {
        if(item.demension!=0.0)
        {
            if(!first)
            {
                res << "*";
            }
            else
            {
                first=0;
            }
            res<<_phyTypeDict->searchDictByName(item.unitType)->getDefault()->getName();
            if(item.demension!=1.0)
                res<<"^("<<item.demension<<")";
        }
    }
    return res.str();
    /*
    std::vector<PhyBaseUnitDemension*> positive;
    std::vector<PhyBaseUnitDemension*> negative;
    for(auto item:_unitDemension)
    {
        if(item.demension>0.0)
        {
            positive.push_back(item);
        }
        else if(item.demension<0.0)
        {
            negative.push_back(item);
        }
    }
    */
}

double PhyUnit::value() const
{
    double res=1.0;
#ifdef DEBUG
    std::cout << "calling unit::value\n";
#endif
    for(auto item:_unitDemension)
    {
#ifdef DEBUG
        std::cout << "search:" << item.unitType << std::endl;
#endif
        res*=std::pow(_phyTypeDict->searchDictByName(item.unitType)->getDefault()->getValue(),item.demension);
    }
    return res;
}

int PhyUnit::variety() const
{
    int count=0;
    for(auto item:_unitDemension)
    {
        if(item.demension!=0.0)
            count++;
    }
    return count;
}

PhyUnit PhyUnit::inverse() const
{
    std::vector<PhyBaseUnitDemension> pbud;
    for(auto item:this->_unitDemension)
    {
        pbud.push_back(PhyBaseUnitDemension(item.unitType,-(item.demension)));
    }
    return PhyUnit(pbud);
}

PhyUnit PhyUnit::operator*(const PhyUnit& unit) const
{
    std::vector<PhyBaseUnitDemension> pbud;
    for(auto item : unit._unitDemension)
    {
        unsigned char hit;
        hit=0;
        for(auto item2:_unitDemension)
        {
            if(item2.unitType==item.unitType)
            {
                hit=1;
                pbud.push_back(PhyBaseUnitDemension(item.unitType,item.demension+item2.demension));
                break;
            }
        }
        if(!hit)
        {
            pbud.push_back(item);
        }
    }
    for(auto item:_unitDemension)
    {
        unsigned char hit;
        hit=0;
        for(auto item2:unit._unitDemension)
        {
            if(item2.unitType==item.unitType)
            {
                hit=1;
            }
        }
        if(!hit)
        {
            pbud.push_back(item);
        }
    }
    return PhyUnit(pbud);
}

PhyUnit PhyUnit::operator/(const PhyUnit& unit) const
{
    return (*this)*(unit.inverse());
    /*
    std::vector<PhyBaseUnitDemension> pbud;
    for(auto item : _unitDemension)
        pbud.push_back(item);
    for(auto item : unit._unitDemension)
    {
        unsigned char hit;
        hit=0;
        for(auto item2:pbud)
        {
            if(item2.unitType==item.unitType)
            {
                hit=1;
                item2.demension-=item.demension;
                break;
            }
        }
        if(!hit)
        {
            pbud.push_back(PhyBaseUnitDemension(item.unitType,-(item.demension)));
        }
    }
    return pbud;
    */
}

PhyUnit PhyUnit::pow(const double a) const
{
    std::vector<PhyBaseUnitDemension> pbud;
    for(auto item:_unitDemension)
    {
        pbud.push_back(PhyBaseUnitDemension(item.unitType,item.demension*a));
    }
    return PhyUnit(pbud);
}

//PhyValue
double PhyValue::getUnitValue() const
{
    return _value/(_unit.value());
}

std::string PhyValue::str() const
{
    std::ostringstream os;
#ifdef DEBUG
    std::cout << "calling PhyValue:str()" << std::endl;
    std::cout << "getunitvalue:" << this->getUnitValue() << "\n";
#endif
    os << this->getUnitValue();
    os << "\t";
    os << _unit.str();
    return os.str();
}

unsigned char PhyValue::isSameUnit(const PhyValue& a) const
{
    if(this->_unit.variety()!=a._unit.variety())
    {
        return 0;
    }
    for(auto item:_unit.getDict())
    {
        if(item.demension!=0)
        {
            if(item.demension!=a._unit.getDemension(item.unitType))
            {
                return 0;
            }
        }
    }
    return 1;
}

PhyValue PhyValue::operator+(const PhyValue& a) const
{
    if(this->_value==0.0)
        return PhyValue(a);
    if(a._value==0.0)
        return PhyValue(*this);
    if(!(this->isSameUnit(a)))
        throw compileException("operator+ error:not same unit");
    return PhyValue(this->_value+a._value,PhyUnit(this->_unit));
}

PhyValue PhyValue::operator-(const PhyValue&a) const
{
    if(this->_value==0.0)
        return PhyValue(-a._value,a._unit);
    if(a._value==0.0)
        return PhyValue(*this);
    if(!(this->isSameUnit(a)))
        throw compileException("operator- error:not same unit");
    return PhyValue(this->_value-a._value,PhyUnit(this->_unit));
}

PhyValue PhyValue::operator*(const PhyValue& a) const
{
    return PhyValue((this->_value)*(a._value),(this->_unit)*(a._unit));
}

PhyValue PhyValue::inverse() const
{
    return PhyValue(1.0/(this->_value),this->_unit.inverse());
}

PhyValue pow(const PhyValue& a,const PhyValue& b)
{
    if(!(b.isNumeric()))
    {
        throw compileException("uncorrect pow use");
    }
    return PhyValue(std::pow(a._value,b._value),a._unit.pow(b._value));
}

PhyValue sin(const PhyValue& a)
{
    if(!(a.isNumeric()))
    {
        throw compileException("uncorrect sin use");
    }
    return PhyValue(std::sin(a._value));
}

//save temporary tokens
std::vector<AnalyzeToken*> _savetoken;
//save global tokens
std::vector<AnalyzeToken*> _globaltoken;
//save unit tokens
std::vector<AnalyzeToken*> _unittoken;

AnalyzeTokenVectorDict _analyzeTokenVectorDict;

//search a token from a specific vector
AnalyzeToken* searchTokenFromVector(const std::string& name,std::vector<AnalyzeToken*>& tokenvector)
{
    int i;
    for(i=0;i<tokenvector.size();i++)
    {
        if(tokenvector[i]->name==name)
        {
#ifdef DEBUG
            std::cout << "hit token:" << name << std::endl;
#endif
            return tokenvector[i];
        }
    }
    return NULL;
}

//search a token from all vectors
AnalyzeToken* searchToken(const std::string& name,int option=0)
{
    AnalyzeToken* result;
#ifdef DEBUG
    std::cout << "searching token:" << name << std::endl;
#endif
    if(name.c_str()[0]=='$')
    {
        if(option!=0)
        {
            throw compileException("can't set value of temporary token");
        }
        return searchTokenFromVector(name,_savetoken);
    }
    else if(name.c_str()[0]=='%')
        return searchTokenFromVector(strsep(name,1,0)[1],_unittoken);
    std::vector<std::string> split=strSplit(name,".");
    if(split.size()==1)
    {
        if((result=searchTokenFromVector(name,_globaltoken))!=NULL)
            return result;
        if((result=searchTokenFromVector(name,_savetoken))!=NULL)
            return result;
        if(option==0)
        {
            for(auto item:_analyzeTokenVectorDict.getDict())
            {
            if((result=searchTokenFromVector(name,(item.to)))!=NULL)
                return result;
            }
            if((result=searchTokenFromVector(name,_unittoken))!=NULL)
            return result;
        }
    }
    else
    {
        std::string newname=name.substr(split[0].length()+1,name.length()-split[0].length()-1);
        if(split[0]=="global")
        {
            return searchTokenFromVector(newname,_globaltoken);
        }
        else
        {
            int tokenvec;
#ifdef DEBUG
            std::cout << "searching for vector:" << split[0] << std::endl;
#endif
            tokenvec=_analyzeTokenVectorDict.searchVector(split[0]);
            if(tokenvec<0)
            {
#ifdef DEBUG
                std::cout << "Nothing hit" << std::endl;
#endif
                return NULL;
            }
#ifdef DEBUG
            else
                std::cout << "Hit one" << std::endl;
#endif
            return searchTokenFromVector(newname,_analyzeTokenVectorDict.getDict()[tokenvec].to);
        }
    }
#ifdef DEBUG
    std::cout << "no token hit by name:" << name << std::endl;
#endif
    return NULL;
}

void addValueToken(const std::string& tokenName,PhyValue value)
{
    std::vector<std::string> splcmds=strSplit(tokenName,".");
    if(tokenName.c_str()[0]=='$')
        throw compileException("can't set value of temporary token");
    if((splcmds.size()==1)&&(splcmds[0].c_str()[0]=='%'))
    {
        std::string newtokenName;
        newtokenName=splcmds[0].substr(1,splcmds[0].length()-1);
        AnalyzeToken* newtoken=new AnalyzeToken(newtokenName,new ValueTree(value));
        _unittoken.push_back(newtoken);
    }
    else if(splcmds.size()==1||splcmds[0]=="global")
    {
        //add to global
        std::string newtokenName;
        if(splcmds.size()==1) newtokenName=tokenName;
        else newtokenName=strsep(tokenName,splcmds[0].length(),1)[1];
#ifdef DEBUG
        std::cout << "create new token to global:"<< newtokenName << std::endl;
        std::cout << "value=" << value << std::endl;
#endif
        AnalyzeToken* newtoken=new AnalyzeToken(newtokenName,new ValueTree(value));
        _globaltoken.push_back(newtoken);
    }
    else
    {
        std::string newtokenName;
        newtokenName=strsep(tokenName,splcmds[0].length(),1)[1];
        int tokenvec;
        tokenvec=_analyzeTokenVectorDict.searchVector(splcmds[0]);
        AnalyzeToken* newtoken=new AnalyzeToken(newtokenName,new ValueTree(value));
#ifdef DEBUG
        std::cout << "create new token:" << newtokenName << std::endl;
#endif
        if(tokenvec<0)
        {
            std::vector<AnalyzeToken*> newtokenvector;
            newtokenvector.push_back(newtoken);
            _analyzeTokenVectorDict.addVector(newtokenvector,splcmds[0]);
#ifdef DEBUG
            std::cout << "add vector:" << splcmds[0] << std::endl;
#endif
        }
        else
        {
            _analyzeTokenVectorDict.getDict()[tokenvec].to.push_back(newtoken);
        }
    }
}

void setTokenValue(const std::string& tokenName,PhyValue value,int option=0)
{
#ifdef DEBUG
    std::cout << "setTokenValue:" << tokenName << " to " << value << "*****" << std::endl;
#endif
    AnalyzeToken* tokenSearch;
    if((tokenSearch=searchToken(tokenName,option))==NULL)
    {
        addValueToken(tokenName,value);
    }
    else
    {
        ValueTree* toChange=(ValueTree*)(tokenSearch->content);
        toChange->changeValue(value);
    }
}

//AnalyzeTree
//constructor
AnalyzeTree::AnalyzeTree(std::vector<AnalyzeTree*> contents,std::string coperator)
{
    _contents=contents;
    _coperator=coperator;
}

int exitThis()
{
    std::cout << "exit!" << std::endl;
    exit(0);
    return 0;
}

//return the calculating value of the tree
PhyValue AnalyzeTree::value()
{
#ifdef DEBUG
    std::cout << "calling generic value by operator:" << _coperator << std::endl;
#endif
    if(_coperator=="+")
    {
        if(_contents.size() == 1)//front operator
        {
            return _contents[0]->value();
        }
        else //two-termed operator
        {
            return _contents[0]->value()+_contents[1]->value();
        }
    }
    if(_coperator=="-")
    {
        if(_contents.size() == 1)//front operator
        {
            return -(_contents[0]->value());
        }
        else //two-termed operator
        {
            return _contents[0]->value()-_contents[1]->value();
        }
    }
    if(_coperator=="*") //two-termed operator
    {
        return _contents[0]->value()*_contents[1]->value();
    }
    if(_coperator=="/") //two-termed operator
    {
        return _contents[0]->value()/_contents[1]->value();
    }
    if(_coperator=="^") //two-termed operator
    {
        return pow(_contents[0]->value(),_contents[1]->value());
    }
    if(_coperator.c_str()[0]=='=')
    {
        PhyValue val=_contents[0]->value();
        setTokenValue(_coperator.substr(1,_coperator.length()-1),val,1);
        return val;
    }
    if(_coperator=="sin") //function
        return sin(_contents[0]->value());
    //if(_coperator=="cos") //function
    //    return std::cos(_contents[0]->value());
    if(_coperator=="pow") //function
        return pow(_contents[0]->value(),_contents[1]->value());
    if(_coperator=="pi")  //function
        return PhyValue(M_PI);
    if(_coperator=="exit") //exit
    {
        return exitThis();
    }
    return PhyValue(0.0);
}

double strToDouble(const std::string& doublestr)
{
    double val;
    sscanf(doublestr.c_str(),"%lf",&val);
    return val;
}

//constructor
BasicTree::BasicTree(std::string& value)
{
    _content = value;
}

//return the value that read from content
PhyValue BasicTree::value()
{
#ifdef DEBUG
    std::cout << "called Basic value()" << std::endl;
#endif
    return PhyValue(strToDouble(_content));
}

//search all matches by regex explaination
std::vector<std::smatch*> regex_searchAll(
        std::regex &regex,
        std::string &target)
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
        std::regex &regex,
        std::string &target)
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
/*
//search last match
std::vector<std::smatch*> regex_searchLast(
        std::regex &regex,
        std::string &target)
{
    std::smatch *match=NULL;
    std::vector<std::smatch*> result;
    std::smatch *res=NULL;
    for(auto it=target.cbegin();
            match = new std::smatch,
            std::regex_search(it,target.cend(),*match,regex);
            it+=match->position(0)+match->length(0)
       )
    {
        res=match;
    }
    if(res!=NULL)
        result.push_back(res);
    return result;
}
*/

AnalyzeTree* searchTreeFromVector(std::string name,std::vector<AnalyzeToken*> tokenvector)
{
    AnalyzeToken* res;
    if((res=searchTokenFromVector(name,tokenvector))==NULL)
        return NULL;
    return res->content;
}

AnalyzeTree* searchTreeFromToken(std::string name)
{
    AnalyzeToken* res;
    if((res=searchToken(name))==NULL)
        return NULL;
    return res->content;
}

void addToken(std::string name,AnalyzeTree* tree)
{
    AnalyzeToken* token=new AnalyzeToken(name,tree);
#ifdef DEBUG
    std::cout << "add token:" << name << std::endl;
#endif
    _savetoken.push_back(token);
#ifdef DEBUG
    std::cout << "done" << std::endl;
#endif
}



std::string headRear(std::string str)
{
    std::ostringstream os;
    os << R"(^)" << str << R"($)";
    return os.str();
}

AnalyzeTree* makeTree(std::string command)
{
    // check if it is a basic tree
    std::string cmd2=command;
    if(cmd2=="")
        return new ValueTree(PhyValue(0.0));
#ifdef DEBUG
    std::cout << "command:" << cmd2 << std::endl;
#endif
    std::string regNumeric=R"((\d+(\.\d+)?))";
    std::regex numeric(headRear(regNumeric));
#ifdef DEBUG
    std::cout << "check numeric:" << std::endl;
#endif
    if (std::regex_match(cmd2,numeric))
    {
#ifdef DEBUG
        std::cout << "put a basic tree" << std::endl;
#endif
        return new BasicTree(cmd2);
    }
#ifdef DEBUG
    std::cout << "done" << std::endl;
#endif
    // check if it is a basic token
#ifdef DEBUG
    std::cout << "check token" << std::endl;
#endif
    std::string regToken=R"(((\$|%)?[a-zA-Z][a-zA-Z0-9]*(\.[a-zA-Z][a-zA-Z0-9]*)*))";
    std::regex token(headRear(regToken));
    if (std::regex_match(cmd2,token))
    {
#ifdef DEBUG
        std::cout << "put a token tree named:" << cmd2 << std::endl;
#endif
        AnalyzeTree* ats=searchTreeFromToken(cmd2);
        if(ats==NULL)
        {
            std::string exceptionNotice="No token hit by name:" + cmd2;
            throw compileException(exceptionNotice);
        }
        return ats;
    }
#ifdef DEBUG
    std::cout << "done" << std::endl;
#endif

    /*
    std::regex func=R"([a-z,A-Z]+\()"
    */
    //check the smallest brackets
    std::ostringstream regFunc;
    regFunc << "(" << regNumeric << "|" << R"(([a-zA-Z][a-zA-Z0-9]*))" << ")?";
    std::regex func(regFunc.str()+R"(\([^\(]*?\))");
    std::vector<std::smatch*> matches=regex_searchAll(func,cmd2);
    int i,iter=0;
    std::string newcmd="";
    for(i=0;i<matches.size();i++)
    {
        std::ostringstream stream;
        std::string temp=matches[i]->str();
        std::vector<AnalyzeTree*> trees;
        AnalyzeTree* newTree=NULL;
        if(matches[i]->str().c_str()[0]=='(')
            newTree=makeTree(temp.substr(1,temp.length()-2));
        else
        {
            std::vector<std::string> splitCmds=strSplit(matches[i]->str(),"(");
            if(splitCmds.size()!=2)
                throw compileException("bracket error");
            if(std::regex_match(splitCmds[0],numeric))
            {
                trees.push_back(makeTree(splitCmds[1].substr(0,splitCmds[1].length()-1)));
                trees.push_back(new BasicTree(splitCmds[0]));
                newTree=new AnalyzeTree(trees,"*");
            }
            else
            {
                for(auto item : strSplit(splitCmds[1].substr(0,splitCmds[1].length()-1),","))
                {
                    //std::cout << "push:" << item << std::endl;
                    trees.push_back(makeTree(item));
                }
                newTree=new AnalyzeTree(trees,splitCmds[0]);
            }
        }
        stream << "$X" << _savetoken.size();
        std::string newname=stream.str();
        addToken(newname,newTree);
        newcmd+=
            cmd2.substr(iter,matches[i]->position(0))
            +newname;
        iter+=matches[i]->position(0)+matches[i]->length(0);
    }
    if(i>0)
    {
        newcmd+=cmd2.substr(iter,cmd2.length()-iter);
#ifdef DEBUG
        std::cout << newcmd << std::endl;
#endif
        return makeTree(newcmd);
    }
    //check operator=
    std::regex eq("=");
    std::vector<std::smatch*> matcheq=regex_searchOne(eq,cmd2);
    if(matcheq.size()==1)
    {
        std::vector<std::string> cmds=strsep(cmd2,matcheq[0]->position(),matcheq[0]->length());
        if(std::regex_match(cmds[0],token))
        {
            std::vector<AnalyzeTree*> trees;
            trees.push_back(makeTree(cmds[1]));
            std::string coperator="=";
            coperator+=cmds[0];
            return new AnalyzeTree(trees,coperator);
        }
        else
        {
            throw compileException("not correct use of operator '='");
        }
    }
    //seperate +-
    std::regex pm(R"((\+|-)+[^\+-]*$)");
    matches=regex_searchOne(pm,cmd2);
    if(matches.size()==1)
    {
        std::vector<AnalyzeTree*> trees;
        std::vector<std::string> sep=strsep(cmd2,matches[0]->position(0),1);
        std::regex dualoperator(R"(((\*|/)|\^)$)");
#ifdef DEBUG
        std::cout << "sep0:" << sep[0] << std::endl;
        std::cout << "sep1:" << sep[1] << std::endl;
#endif
        if(sep[0].length()==0||std::regex_search(sep[0],dualoperator))
        {
            //single operator
            std::string frontoperator=R"((\+|-))";
            std::ostringstream os2;
            os2 << frontoperator << "*(" << regNumeric << "|" << regToken << ")";
#ifdef DEBUG
            std::cout << "regex string:" << os2.str() << std::endl;
#endif
            std::regex soelement(os2.str());
#ifdef DEBUG
            std::cout << "to match:" << sep[1] << std::endl;
#endif
            std::vector<std::smatch*> match2=regex_searchOne(soelement,sep[1]);
            if(match2.size()!=1) return NULL;
#ifdef DEBUG
            std::cout << "match succeed:" << match2[0]->str() << std::endl;
#endif
            std::ostringstream newcmd;
            std::ostringstream newname;
            trees.push_back(makeTree(match2[0]->str()));
            newname << "$X" << _savetoken.size();
#ifdef DEBUG
            std::cout << "newname is:" << newname.str() << std::endl;
            std::cout << "trees has made:" << trees.size() << std::endl;
#endif
            AnalyzeTree* newTree = new AnalyzeTree(trees,matches[0]->str().substr(0,1));
            addToken(newname.str(),newTree);
            newcmd << sep[0] << newname.str() << sep[1].substr(match2[0]->length(),sep[1].length()-match2[0]->length());
#ifdef DEBUG
            std::cout << "new command:" << newcmd.str() << std::endl;
#endif
            return makeTree(newcmd.str());
        }
        trees.push_back(makeTree(sep[0]));
        trees.push_back(makeTree(sep[1]));
        std::string coperator=matches[0]->str().substr(0,1);
        return new AnalyzeTree(trees,coperator);
    }
    //seperate *,/
    std::string mdstr="";
    if(_inverseMD)
    {
        mdstr=R"((\*|/))";
    }
    else
    {
        mdstr=R"((\*|/)[^\*/]*$)";
    }
    std::regex md(mdstr);
    matches=regex_searchOne(md,cmd2);
    if(matches.size()==1)
    {
        std::vector<AnalyzeTree*> trees;
        std::vector<std::string> sep=strsep(cmd2,matches[0]->position(0),1);
        trees.push_back(makeTree(sep[0]));
        trees.push_back(makeTree(sep[1]));
        std::string coperator=matches[0]->str().substr(0,1);
        return new AnalyzeTree(trees,coperator);
    }
    //seperate ^
    std::regex pw(R"(\^)");
    matches=regex_searchOne(pw,cmd2);
    if(matches.size()==1)
    {
        std::vector<AnalyzeTree*> trees;
        std::vector<std::string> sep=strsep(cmd2,matches[0]->position(0),matches[0]->length(0));
        trees.push_back(makeTree(sep[0]));
        trees.push_back(makeTree(sep[1]));
        std::string coperator=matches[0]->str();
        return new AnalyzeTree(trees,coperator);
    }
    throw compileException("compile Error");
    return NULL;
}

PhyValue doCalculate(std::string cmd)
{
    _savetoken.clear();
    PhyValue result;
    AnalyzeTree* root=makeTree(cmd);
//    if(root==NULL)
//        throw compileException("compile error");
    result=root->value();
    setTokenValue(std::string("global.ans"),result);
//    ValueTree* vt=(ValueTree*)searchTreeFromVector("ans",_globaltoken);
//    vt->changeValue(result);
    return result;
}

std::string deleteSpace(std::string cmd)
{
    std::string from=" ";
    std::string to="";
    std::string::size_type pos=0;
    std::string str=cmd;
    while(pos=str.find(from,pos),pos!=std::string::npos)
    {
        str.replace(pos,from.length(),to);
        pos+=to.length();
    }
    return str;
}

std::string calculateFormat(std::string cmd)
{
    std::string res=deleteSpace(cmd);
    std::regex fix3(R"(([^A-Za-z0-9]|^)\.)");
    int iter=0;
    for(std::smatch* item:regex_searchAll(fix3,res))
    {
        iter+=item->position();
        res.insert(iter+item->length()-1,"0");
        iter+=item->length()+1;
    }
    std::regex fix1(R"((([^\.A-Za-z0-9]|(^|-))(\d+(\.\d+)?)((\$|%)?[A-Za-z][A-Za-z0-9]*)))");
    std::regex numeric(R"(\d+(\.\d+)?)");
    iter=0;
    for(std::smatch* item:regex_searchAll(fix1,res))
    {
        iter+=item->position();
        std::vector<std::smatch*> matches;
        std::string tmp;
        tmp=item->str();
        matches=regex_searchOne(numeric,tmp);
        res.insert(iter+matches[0]->length()+matches[0]->position(),"*");
        iter+=item->length()+1;
    }
    std::regex fix2(R"(\)[\$%A-Za-z0-9\(])");
    iter=0;
    for(std::smatch* item:regex_searchAll(fix2,res))
    {
        iter+=item->position();
        res.insert(iter+1,"*");
        iter+=item->length()+1;
    }
    return res;
}

std::string stringTrim(const std::string& string,const char* trimCharacterList="\t\v\r\n")
{
    std::string result="";
    std::string::size_type left=string.find_first_not_of(trimCharacterList);
    if(left!=std::string::npos)
    {
        std::string::size_type right=string.find_last_not_of(trimCharacterList);
        result=string.substr(left,right-left+1);
    }
    return result;
}

void setBaseUnit(std::string& unitName,std::string& typeName,double value)
{
    PhyBaseUnitDict* pbud;
    if((pbud=_phyTypeDict->searchDictByName(typeName))==NULL)
    {
        //add new dict
        pbud=new PhyBaseUnitDict(typeName);
        pbud->addUnit(new PhyBaseUnit(unitName,typeName,value));
        pbud->changeDefault(0);
        _phyTypeDict->addDict(pbud);
    }
    else
    {
        PhyBaseUnit* pbu;
        if((pbu=pbud->searchByName(unitName))==NULL)
        {
            //add new item
            pbud->addUnit(new PhyBaseUnit(unitName,typeName,value));
        }
        else
        {
            pbu->setValue(value);
        }
    }
    AnalyzeToken* hit=NULL;
    if((hit=searchTokenFromVector(unitName,_unittoken))==NULL)
    {
        ValueTree* toAdd=new ValueTree(PhyValue(value,PhyUnit(typeName)));
        AnalyzeToken* newToken=new AnalyzeToken(unitName,toAdd);
        _unittoken.push_back(newToken);
    }
    else
    {
        ValueTree* vt=(ValueTree*)(hit->content);
        vt->changeValue(PhyValue(value,PhyUnit(typeName)));
    }
}

std::vector<std::string> _cmds=
{
    "#exit",
    "#setUnit",
    "#setDefault",
    "#read",
    "#setInverseMD",
    "#help",
    "#list"
};

int seperateCmd(std::string& cmd)
{
    std::string cmd2=stringTrim(cmd);
    if(cmd2=="\n"||cmd2=="")
    {
        std::cout << "blank!\n";
        return 1;
    }
    switch(cmd2.c_str()[0])
    {
        case EOF:
        {
            return 1;
        }
        case '#':
        {
            //command
            std::vector<std::string> strspl=strSplit(cmd2," ");
            if(strspl[0]==_cmds[0])
                return exitThis();
            if(strspl[0]==_cmds[1])
            {
                std::vector<std::string> strspl2=strSplit(strspl[1],"=");
                if(strspl.size()<3)
                {
                    std::cout << "Type of unit is missing!" << std::endl;
                    return -2;
                }
                setBaseUnit(strspl2[0],strspl[2],strToDouble(strspl2[1]));
                std::cout << "Add unit:" << strspl2[0] << " (" << strspl2[1] << " " << strspl[2] << ")" << std::endl;
                return 1;
            }
            if(strspl[0]==_cmds[2])
            {
                std::string unitName=strspl[1];
                std::string typeName;
                PhyBaseUnitDict* pbud=NULL;
                if(strspl.size()==2)
                {
                    PhyBaseUnit* pbu=_phyTypeDict->searchUnitByName(unitName);
                    if(pbu!=NULL)
                    {
                        typeName=pbu->getType();
                    }
                    else
                    {
                        std::cout << "unit '" << unitName << "' doesn't exists\n";
                        return -1;
                    }
                }
                else if(strspl.size()>2)
                {
                    typeName=strspl[2];
                }
                pbud=_phyTypeDict->searchDictByName(typeName);
                if(pbud!=NULL)
                {
                    if(pbud->changeDefault(unitName))
                    {
                        std::cout <<"unit '"<<unitName<<"' doesn't exists in type '"<<typeName<<"'\n";
                        return -2;
                    }
                    std::cout << "Change default unit of " << typeName << " to: " << unitName << std::endl;
                    return 2;
                }
                else
                {
                    std::cout << "type '" << typeName <<"' doesn't exists\n";
                    return -3;
                }
            }
            if(strspl[0]==_cmds[3])
            {
                if(strspl.size()<2)
                {
                    std::cout << "missing filename!\n";
                    return -4;
                }
                readScript(strspl[1]);
                return 4;
            }
            if(strspl[0]==_cmds[4])
            {
                if(strspl.size()<2)
                {
                    std::cout << "missing value to set!\n";
                    return -5;
                }
                if(strspl[1]=="true")
                {
                    _inverseMD=1;
                    std::cout << "set InverseMD to true" << std::endl;
                    return 5;
                }
                else if(strspl[1]=="false")
                {
                    _inverseMD=0;
                    std::cout << "set InverseMD to false" << std::endl;
                    return 5;
                }
                std::cout << "invalid value to set!" << std::endl;
                return -5;
            }
            if(strspl[0]==_cmds[5])
            {
                std::cout << "list all commands:\n";
                for(auto item:_cmds)
                {
                    std::cout << item << std::endl;
                }
                return 6;
            }
            if(strspl[0]==_cmds[6])
            {
                if(_analyzeTokenVectorDict.getDict().size()==0)
                {
                    std::cout << "no other groups!" << std::endl;
                }
                else if(strspl.size() == 1)
                {
                    std::cout << "list all groups:" << std::endl;
                    for(auto item:_analyzeTokenVectorDict.getDict())
                    {
                        std::cout << item.name << std::endl;
                    }
                }
                else
                {
                    std::cout << "list all item of group " << strspl[1] << std::endl;
                    if(strspl[1]=="global")
                    {
                        for(auto item:_globaltoken)
                        {
                            std::cout << item->name << std::endl;
                        }
                    }
                    else if(strspl[1]=="unit")
                    {
                        for(auto item:_unittoken)
                        {
                            std::cout << item->name << std::endl;
                        }
                    }
                    else
                    {
                        char found=0;
                        for(auto item:_analyzeTokenVectorDict.getDict())
                        {
                            if(strspl[1]==item.name)
                            {
                                for(auto item2:item.to)
                                {
                                    std::cout << item2->name << std::endl;
                                }
                                found=1;
                                break;
                            }
                        }
                        if(!found)
                        {
                            std::cout << "no group names this!" << std::endl;
                            return -7;
                        }
                    }

                }
                return 7;
            }
            std::cout << "invalid command:" << strspl[0] << std::endl;
            break;
        }
        default:
        {
            try{
                cmd2=calculateFormat(cmd2);
                std::cout << COLOR_GREEN << doCalculate(cmd2) << COLOR_NORMAL << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cout << COLOR_RED << std::string(e.what()) << COLOR_NORMAL <<std::endl;
            }
            return 2;
        }
    }
    return 1;
}

int readScript(std::string filename)
{
    std::ifstream ifs(filename);
    std::string line;
    if(ifs)
    {
        while(std::getline(ifs,line))
        {
            std::cout << "read:" << line << std::endl;
            seperateCmd(line);
        }
    }
    else
        return 1;
    return 0;
}

int initialize()
{
    ValueTree *vt=new ValueTree(PhyValue(0.0));
    AnalyzeToken *at=new AnalyzeToken("ans",vt);
    _globaltoken.push_back(at);
    if(_phyTypeDict==NULL)
    {
        _phyTypeDict=new PhyTypeDict();
    }
    return 0;
}
