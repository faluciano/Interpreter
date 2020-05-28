#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
#include "val.h"
#include "parse.h"
#include "lex.h"

class Pt {
protected:
    int    linenum;
    Pt    *left;
    Pt    *right;

public:
    Pt(int linenum, Pt *l = 0, Pt *r = 0)
        : linenum(linenum), left(l), right(r) {}

    virtual ~Pt() {
        delete left;
        delete right;
    }
    
    int GetLineNumber() const { return linenum; }
    Pt* getleft(){
        return left;
    }
    Pt* getright(){
        return right;
    }
    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
    virtual bool isIdent() const { return false; }
    virtual bool isVar() const { return false; }
    virtual string getId() const { return ""; }
    virtual Value Eval(map<string,Value>& syms) = 0;
    int maxDepth(){
        if (this == nullptr)
            return 0;
        else
        {
            int lDepth = this->left->maxDepth();
            int rDepth = this->right->maxDepth();

            if (lDepth > rDepth)
                return(lDepth + 1);
            else
                return(rDepth + 1);
        }
    }
    int plusCount() const {
        int pCount = 0;
        if(left) {
            pCount += left->plusCount();
        }
        if(right) {
            pCount += right->plusCount();
        }
        if(isPlus()) {
            pCount++;
        }
        return pCount;
    }
    int EqCount() const {
        int eCount = 0;
        if(left) {
            eCount += left->EqCount();
        }
        if(right) {
            eCount += right->EqCount();
        }
        if(isEq()) {
            eCount++;
        }
        return eCount;
    }
};

class StmtList : public Pt {
    public:
        StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
        Value Eval(map<string,Value>& syms){
            left->Eval(syms);
            if( right )
                right->Eval(syms);
            return Value();
        }
};

class Print : public Pt {
    public:
        Print(int line, Pt *e) : Pt(line, e) {}
        Value Eval(map<string,Value>& syms){
            cout<<left->Eval(syms);
            return Value();
        }
};

class Println : public Pt {
    public:
        Println(int line, Pt *e) : Pt(line,e){}
        Value Eval(map<string,Value>& syms){
            cout<<left->Eval(syms)<<endl;
            return Value();
        }
};

class Repeat : public Pt {
    public:
        Repeat(int line, Pt *e,Pt *stmt):Pt(line,e,stmt){}
        Value Eval(map<string,Value>& syms){
            Value leftval = left->Eval(syms);
            if(!leftval.IsInt() )
                throw string(to_string(linenum+1)+": control expression not an integer");
            if(leftval.GetInt()<0)
                throw string(to_string(linenum+1)+": control expression not an positive integer");
            for(int i = leftval.GetInt();i>0;i--){
                right->Eval(syms);
            }
            return Value();
        }
        
};

class Assign : public Pt {
    string val;
    public:
        Assign(int line, Pt* l, Pt*r): Pt(line, l, r){
            val = "=";
        }
        bool isEq()const{
                return true;
        }
        Value Eval(map<string,Value>& syms) {
            Value rightval = right->Eval(syms);
            if(left->isEq()&&right){
                syms[left->getright()->getId()] = rightval;
                return left->Eval(syms);
            }
            else{
                string leftstr = left->getId();
                syms[leftstr] = rightval;
            }
            return rightval;
        }
};

class PlusExpr : public Pt {
    string val;
        public:
        PlusExpr(int line, Pt* l, Pt*r): Pt(line, l, r){
            val = "+";
        }
        bool isPlus()const{
            return true;
        }
        Value Eval(map<string,Value>& syms){
            try{
                return (left->Eval(syms))+(right->Eval(syms));
            }
            catch(string& e){
                if(isdigit(e[0]))
                   throw e;
                throw string(to_string(linenum+1)+e);
            }
        }
};

class MinusExpr : public Pt {
    string val;
    public:
    MinusExpr(int line, Pt* l, Pt*r): Pt(line, l, r){
        val = "-";
    }
    Value Eval(map<string,Value>& syms){
        try{
            return (left->Eval(syms))-(right->Eval(syms));
        }
        catch(string& e){
            if(isdigit(e[0]))
                throw e;
            throw string(to_string(linenum+1)+e);
        }
    }
};

class TimesExpr : public Pt {
    string val;
    public:
    TimesExpr(int line, Pt* l, Pt*r): Pt(line, l, r){
        val = "*";
    }
    Value Eval(map<string,Value>& syms){
        try{
            return (left->Eval(syms))*(right->Eval(syms));
        }
        catch(string& e){
            if(isdigit(e[0]))
                throw e;
            throw string(to_string(linenum+1)+e);
        }
    }
};

class DivideExpr : public Pt {
    string val;
    public:
    DivideExpr(int line, Pt* l, Pt*r): Pt(line, l, r){
        val = "/";
    }
    Value Eval(map<string,Value>& syms){
        try{
            return (left->Eval(syms))/(right->Eval(syms));
        }
        catch(string& e){
            if(isdigit(e[0]))
                throw e;
            throw string(to_string(linenum+1)+e);
        }
    }
};

class IConst : public Pt {
    int val;

public:
    IConst(Tok& t) : Pt(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }
    IConst(Tok& t, char sig) : Pt(t.GetLinenum()) {
        if(sig == '-')
            val = -stoi(t.GetLexeme());
        else
            val = stoi(t.GetLexeme());
    }
    Value Eval(map<string,Value>& syms) {
        return Value(val);
    }
};

class SConst : public Pt {
    string val;

public:
    SConst(Tok& t) : Pt(t.GetLinenum()) {
        val = t.GetLexeme();
    }
    Value Eval(map<string,Value>& syms) {
        return Value(val);
    }
};

class Ident : public Pt {
    string val;
public:
    Ident(Tok& t):Pt(t.GetLinenum()){
        val = t.GetLexeme();
    }
    bool isIdent() const{return true;}
    string getId() const{
        return val;
    }
    Value Eval(map<string,Value>& syms) {
        if(syms.find(val)!=syms.end()){
            return syms[val];
        }
        else{
            throw string(to_string(linenum+1)+": Variable used but not assigned");
        }
    }
};

#endif /* PT_H_ */
