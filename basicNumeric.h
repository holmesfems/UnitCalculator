#ifndef BASICNUMERIC
#define BASICNUMERIC
#include<iostream>
#include<string.h>
#include<vector>
#define consts PhyValue

class PhyBaseUnit
{
    private:
        std::string _unitName; //like kg,s,m,etc.
        std::string _unitType; //like mass,time,length,etc.
        double _value; //only used at unit transformation
    public:
        PhyBaseUnit(std::string& name,std::string& type,double value)
        {
            _unitName=name;_unitType=type;_value=value;
        }
        std::string getName(){return _unitName;}
        std::string getType(){return _unitType;}
        double getValue(){return _value;}
        void setValue(double value){_value=value;}
};

class PhyBaseUnitDict //dictionary to search BaseUnit for convenience to unit transformation
{
    private:
        std::vector<PhyBaseUnit*> _dict;
        std::string _unitType="";
        PhyBaseUnit* _defaultUnit;
    public:
        PhyBaseUnitDict(){};
        PhyBaseUnitDict(std::string& typeName)
        {
            _unitType=typeName;
        }
        void addUnit(PhyBaseUnit* baseUnit){_dict.push_back(baseUnit);}
        PhyBaseUnit* searchByName(const std::string& name);
        PhyBaseUnit* searchByIndex(int index);
        PhyBaseUnit* getDefault(){return _defaultUnit;}
        int changeDefault(const std::string& defaultUnit);
        int changeDefault(int index);
        std::string getType(){return _unitType;}
};

class PhyTypeDict
{
    private:
        std::vector<PhyBaseUnitDict*> _dict;
    public:
        PhyTypeDict(){};
        PhyTypeDict(std::vector<PhyBaseUnitDict*>& dict)
        {
            _dict=dict;
        }
        PhyBaseUnitDict* searchDictByName(const std::string& name);
        PhyBaseUnit* searchUnitByName(const std::string& name);
        void addDict(PhyBaseUnitDict* item){_dict.push_back(item);}
};

class PhyBaseUnitDemension
{
    public:
        std::string unitType;
        double demension;
        PhyBaseUnitDemension(){};
        PhyBaseUnitDemension(const PhyBaseUnitDemension& phyBaseUnitDemension);
        PhyBaseUnitDemension(std::string unitType,double demension){this->unitType=unitType;this->demension=demension;}
        PhyBaseUnitDemension operator=(const PhyBaseUnitDemension& a);
};

class PhyUnit
{
    private:
        std::vector<PhyBaseUnitDemension> _unitDemension;
    public:
        PhyUnit(){};
        PhyUnit(std::vector<PhyBaseUnitDemension>& unitDemension)
        {
            _unitDemension=unitDemension;
        }
        PhyUnit(const std::string& unitStr);
        PhyUnit(const PhyUnit& phyUnit);
        std::vector<PhyBaseUnitDemension> getDict() const {return _unitDemension;}
        double getDemension(const std::string& demensionType) const;
        void setDemension(double demension,const std::string& demensionType);
        unsigned char isNumeric() const;
        std::string str() const;
        double value() const;
        int variety() const;
        PhyUnit inverse() const;
        PhyUnit operator*(const PhyUnit& unit) const;
        PhyUnit operator/(const PhyUnit& unit) const;
        PhyUnit pow(const double a) const;
};

class PhyValue
{
    private:
        PhyUnit _unit;
        double _value;
    public:
        PhyValue(double value=0.0,const PhyUnit& unit=PhyUnit())
        {
            _value=value;_unit=unit;
        }
        PhyValue(const PhyValue& a){_value=a._value;_unit=a._unit;}
        double getValue()const{return _value;}
        double getUnitValue() const;
        std::string str() const;
        unsigned char isNumeric()const{return _unit.isNumeric();}
        unsigned char isSameUnit(const PhyValue &a) const;
        PhyValue operator+(const PhyValue& a) const;
        PhyValue operator+() const{return PhyValue(0.0)+(*this);}
        PhyValue operator-(const PhyValue& a) const;
        PhyValue operator-() const{return PhyValue(0.0)-(*this);}
        PhyValue operator*(const PhyValue& a) const;
        PhyValue inverse() const;
        PhyValue operator/(const PhyValue& a) const{return (*this)*(a.inverse());}
        PhyValue operator=(const PhyValue& a){_unit=a._unit;_value=a._value;return PhyValue(*this);}
        friend PhyValue pow(const PhyValue& a,const PhyValue& b);
        friend PhyValue sin(const PhyValue& a);
        friend std::ostream& operator<<(std::ostream &os,const PhyValue &a){return os<<a.str();}
};

class AnalyzeTree
{
    private:
        std::vector<AnalyzeTree*> _contents;
        std::string _coperator;
    public:
        AnalyzeTree(std::vector<AnalyzeTree*> contents,std::string coperator);
        AnalyzeTree(){}
        virtual PhyValue value();
};

class BasicTree:public AnalyzeTree
{
    private:
        std::string _content;
    public:
        BasicTree(std::string& value);
        PhyValue value();
};

class ValueTree:public AnalyzeTree
{
    private:
        PhyValue _content;
    public:
        ValueTree(const PhyValue& value){_content=value;}
        PhyValue value(){return _content;}
        void changeValue(const PhyValue& value){_content=value;}
};


#endif
