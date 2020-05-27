#Interpreter

This was a project for my progamming language concepts class.

The language follows these grammar rules:

Prog := Sl
Sl := SC { Sl } | Stmt SC { Sl }
Stmt := PrintStmt | PrintlnStmt | RepeatStmt | Expr
PrintStmt := PRINT Expr
PrintlnStmt := PRINTLN 
ExprRepeatStmt:= Repeat 
Expr BEGIN 
Stmt END
Expr := Sum { EQ Sum }
Sum := Prod { (PLUS | MINUS) Prod }
Prod := Primary { (STAR | SLASH) Primary }
Primary := IDENT | ICONST | SCONST | LPAREN Expr RPAREN
