#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
using namespace std;

enum ValType { VINT, VSTR, VERR };

class Value {
    ValType    T;
    int        I;
    string    S;
    
public:
    Value() : T(VERR), I(0) {}
    Value(int vi) : T(VINT), I(vi) {}
    Value(string vs) : T(VSTR), I(0), S(vs) {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsStr() const { return T == VSTR; }
    
    int GetInt() const { if( IsInt() ) return I; throw string("RUNTIME ERROR: Value not an integer"); }
    string GetStr() const { if( IsStr() ) return S; throw string("RUNTIME ERROR: Value not a string"); }
        
    Value operator+(const Value& op) const {
        if( this->IsInt() && op.IsInt() )
            return this->GetInt() + op.GetInt();
        if( this->IsStr() && op.IsStr() )
            return this->GetStr() + op.GetStr();
        throw string(": Not a valid operation");
    }
    
    Value operator-(const Value& op) const{
        if( this->IsInt() && op.IsInt() )
            return this->GetInt() - op.GetInt();
        if( IsStr() && op.IsStr() ){
            string cop = this->GetStr();
            unsigned long index = cop.find(op.GetStr());
            if(index == std::string::npos){
                return cop;
            }
            cop.erase(index,op.GetStr().length());
            return cop;
        }
        throw string(": Type error on subtraction");
        
    }
    
    Value operator*(const Value& op) const{
        if(this->IsInt() && op.IsInt())
            return this->GetInt() * op.GetInt();
        if(this->IsInt() && op.IsStr()){
            if(GetInt()<0)
                throw string(": Too low of a number");
            string temp = "";
            for(int i =0; i<GetInt();i++){
                temp +=op.GetStr();
            }
            return temp;
        }
        if(op.IsInt() && this->IsStr()){
            if(op.GetInt()<0)
                throw string(": Too low of a number");
            string temp = "";
            for(int i =0; i<op.GetInt();i++){
                temp += this->GetStr();
            }
            return temp;
        }
        throw string(": Type error on multiplication");
    };
    
    // divide this by op
    Value operator/(const Value& op) const{
        if(this->IsInt() && op.IsInt()){
            return this->GetInt() / op.GetInt();
        }
        throw string(": Not a valid division");
    };
    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.I;
        else if( op.IsStr() ) out << op.S;
        else out << "ERROR";
        return out;
    }
};

#endif
