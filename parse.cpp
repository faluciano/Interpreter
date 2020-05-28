#include "parse.h"
#include "pt.h"
namespace Parser {
bool pushed_back = false;
Tok    pushed_token;

static Tok GetNextToken(istream& in, int& line) {
    if( pushed_back ) {
        pushed_back = false;
        return pushed_token;
    }
    return getNextToken(in, line);
}

static void PushBackToken(Tok& t) {
    if( pushed_back ) {
        abort();
    }
    pushed_back = true;
    pushed_token = t;
}

}
static int error_count = 0;

void ParseError(int line, string msg)
{
    ++error_count;
    cout << line << ": " << msg << endl;
}

Pt *Prog(istream& in, int& line)
{
    Pt *sl = Sl(in, line);

    if( sl == 0 )
        ParseError(line, "No statements in program");

    if( error_count )
        return 0;

    return sl;
}

// Sl is a Statement followed by a Statement List
Pt *Sl(istream& in, int& line) {
    Pt *s = Stmt(in, line);
    if( s == 0 )
        return 0;
    if( Parser::GetNextToken(in, line) != SC ) {
        ParseError(line-1, "Missing semicolon");
        delete s;
        return 0;
    }
    return new StmtList(s, Sl(in,line));
}

Pt *Stmt(istream& in, int& line) {
    Tok temp = Parser::GetNextToken(in,line);
    Pt *p;
    Tok temp2;
    Pt *s2;
    switch(temp.GetToken()){
        case PRINT:
            p = PrintStmt(in,line);
            break;
        case PRINTLN:
            p = PrintlnStmt(in,line);
            break;
        case REPEAT:
            p = RepeatStmt(in,line);
            break;
        case DONE:
            return 0;
        case IDENT:
            Parser::PushBackToken(temp);
            p = Primary(in, line);
            if(p == 0){
                return 0;
            }
            while(true){
                temp2 = Parser::GetNextToken(in,line);
                if(temp2!=EQ){
                    Parser::PushBackToken(temp2);
                    return p;
                }
                s2 = Sum(in,line);
                if(s2 == 0){
                    ParseError(line,"Missing sum after equal");
                    return 0;
                }
                
                p = new Assign(temp2.GetLinenum(),p,s2);
            }
            break;
        default:
            ParseError(line,"Error in statment");
            p=0;
            break;
     }
    
       return p;
}
    

Pt *PrintStmt(istream& in, int& line) {
    int l = line;

    Pt *ex = Expr(in, line);
    if( ex == 0 ) {
        ParseError(line, "Missing expression after print");
        return 0;
    }
    return new Print(l, ex);
}

Pt *PrintlnStmt(istream& in, int& line) {
// FIXME add code
    int l = line;
    Pt *ex = Expr(in, line);
    if( ex == 0 ) {
        ParseError(line, "Missing expression after print");
        return 0;
    }

    return new Println(l, ex);

}

Pt *RepeatStmt(istream& in, int& line) {
    int l = line;
    Pt* ex = Expr(in,line);
    if(ex == 0){
        ParseError(line,"Missing expression after repeat");
        return 0;
    }
    Tok t = Parser::GetNextToken(in,line);
    if(t == BEGIN){
        Pt* stmt = Stmt(in,line);
           if(stmt == 0){
            ParseError(line,"Missing expression after begin");
            return 0;
        }
        t = Parser::GetNextToken(in,line);
        if(t == END){
            return new Repeat(l,ex,stmt);
        }
        ParseError(line,"Missing end inside of repeat");
        return 0;
    }
    
    return 0;
}

Pt *Expr(istream& in, int& line) {
    Pt* s = Sum(in, line);
    if(s == 0){
        return 0;
    }
    while(true){
        Tok t = Parser::GetNextToken(in,line);
        if(t!=EQ){
            Parser::PushBackToken(t);
            return s;
        }
        Pt* s2 = Sum(in,line);
        if(s2 == 0){
            ParseError(line,"Missing sum after equal");
            return 0;
        }
        if(s->isIdent() && s2->isIdent()){
            
        }
        s = new Assign(t.GetLinenum(),s,s2);
        
    }
}

Pt *Sum(istream& in, int& line) {
    Pt *t1 = Prod(in, line);
    if( t1 == 0 ) {
        return 0;
    }

    while ( true ) {
        Tok t = Parser::GetNextToken(in, line);

        if( t != PLUS && t != MINUS ) {
            Parser::PushBackToken(t);
            return t1;
        }

        Pt *t2 = Prod(in, line);
        if( t2 == 0 ) {
            ParseError(line, "Missing expression after operator");
            return 0;
        }
        
        if( t == PLUS )
            t1 = new PlusExpr(t.GetLinenum(), t1, t2);
        else
            t1 = new MinusExpr(t.GetLinenum(), t1, t2);
    }
}

Pt *Prod(istream& in, int& line) {
    Pt *p1 = Primary(in, line);
    if( p1 == 0 ) {
        return 0;
    }

    while ( true ) {
        Tok t = Parser::GetNextToken(in, line);
        if( t != STAR && t != SLASH ) {
            Parser::PushBackToken(t);
            return p1;
        }

        Pt *p2 = Primary(in, line);
        if( p2 == 0 ) {
            ParseError(line, "Missing primary after operator");
            return 0;
        }
        
        if( t == STAR )
            p1 = new TimesExpr(t.GetLinenum(), p1, p2);
        else
            p1 = new DivideExpr(t.GetLinenum(), p1, p2);
    }
}

Pt *Primary(istream& in, int& line) {
    Tok t = Parser::GetNextToken(in, line);
    Pt* ex;
    Tok t2;
    
    switch(t.GetToken()) {
        case IDENT:
            return new Ident(t);
        case ICONST:
            return new IConst(t);
        case SCONST:
            return new SConst(t);
        case MINUS:
            t2 = Parser::GetNextToken(in, line);
            if(t2 == ICONST){
                return new IConst(t2,'-');
            }
            Parser::PushBackToken(t);
            break;
        case LPAREN:
            ex = Expr(in,line);
            if(ex == 0){
                ParseError(line,"Missing expression after left parenthesis");
                return 0;
            }
            t2 = Parser::GetNextToken(in,line);
            if(t2 != RPAREN){
                ParseError(line,"Missing closing parenthesis");
                return 0;
            }
            return ex;
        default:
            ParseError(line,"Not a valid Primary");
            return 0;
    }
    ParseError(line,"Not a valid Primary");
    return 0;
}

