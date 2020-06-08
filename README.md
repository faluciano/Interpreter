# Interpreter

This was a project for my progamming language concepts class.

The Interpreter takes in a file as an argument or reads from standard in.

There are more debuggin options included in main to test the lexer.

## To run:

compile with " --std=c++11 -w *.cpp -o interp " then "./interp filepath" 

## The language follows these grammar rules:

* Prog := Sl
* Sl := SC { Sl } | Stmt SC { Sl }
* Stmt := PrintStmt | PrintlnStmt | RepeatStmt | Expr
* PrintStmt := PRINT Expr
* PrintlnStmt := PRINTLN Expr
* RepeatStmt:= Repeat Expr BEGIN Stmt END
* Expr := Sum { EQ Sum }
* Sum := Prod { (PLUS | MINUS) Prod }
* Prod := Primary { (STAR | SLASH) Primary }
* Primary := IDENT | ICONST | SCONST | LPAREN Expr RPAREN

## Features

1. The language contains two types: integer and string.
2. The PLUS MINUS STAR and SLASH operators are left associative.
3. The EQ operator is right associative.
4. A Repeat statement evaluates the Expr. The Expr must evaluate to an integer. Ifthe integer is nonzero, then the Stmt is executed, otherwise it is not. The value of theExpr is decremented by 1 each repetition. In other words “repeat 5 begin println “hello”end” will print “hello” 5 times.
5. A PrintStmt evaluates the Expr and prints its value.
6. A PrintlnStmt evaluates the Expr and prints its value, followed by a newline character.
7. The type of an IDENT is the type of the value assigned to it.
8. The EQ operator assigns values to variables. It evaluates the Expr on the right hand sideand saves its value in memory associated with the left hand side (an IDENT). If theIDENT does not exist, it is created. If the IDENT already exists, its value is replaced.
9. The PLUS and MINUS operators in Expr represent addition and subtraction.
10. The STAR and SLASH operators in Prod represent multiplication and division.
11. It is an error if a variable is used before a value is assigned to it.
12. Addition is defined between two integers (the result being the sum) or two strings (theresult being the concatenation).
13. Subtraction is defined between two integers (the result being the difference) or twostrings (the result being removal of the first instance of the second string from the firststring, if any).
14. Multiplication is defined between two integers (the result being the product) or for aninteger and a string (the result being the string repeated integer times).
15. Division is defined between two integers.
16. Performing an operation with incorrect types or type combinations is an error.17.Multiplying a string by a negative integer is an error.
18. A Repeat statement whose Expr is not integer typed is an error.
