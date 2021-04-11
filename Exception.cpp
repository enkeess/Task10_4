#pragma once
#include <iostream>
#include "Lex.cpp"
using namespace std;


enum exception_status {
    UnknownError,
    InvalidLexeme,
    UndeclaredIdentifier,
    Redefinition,
    SyntaxError,
    SemanticError,
    EndOfFile,
    NotEndOfFIle,
    EmptyOperation,
    TypeError,
    Division_by_zero
};

class Exception {
public:
    exception_status status;
    Lex received;
    type_of_lex expected;
    static const string TE[];
    static const string TL[];

public:
    Exception( Lex received,
                exception_status status = UnknownError,  
                type_of_lex expected = LEX_NULL           ) 
    {
        this->status = status;
        this->received = received;
        this->expected = expected;
    }

    friend ostream & operator <<(ostream & out, const Exception & l ) {
        out << "!!! Exeption !!!" << " in line : " << l.received.get_line() << endl;
        out << "Exception status : " << TE[l.status] << endl;
        out << "Recieved         : " << l.received ;
        if(l.expected != LEX_NULL) out << "Expected         : " << TL[l.expected] << endl;

        return out;
    }

};

const string Exception::TE[] ={
    "UnknownError" ,
    "InvalidLexeme",
    "UndeclaredIdentifier", 
    "Redefinition",
    "SyntaxError",
    "SemanticError",
    "EOF",
    "NotEndOfFile",
    "EmptyOperation",
    "TypeError",
    "Division_by_zero"
};

const string Exception::TL[] ={
    "LEX_NULL",

    "main",
    
    "int",
    "double",
    "bool",

    "true || false", //LEX_TRUE
    "false",

    "do",
    "while",
    "if",
    "else",
    "for",

    "to",
    "downto",

    "cinout", //LEX_CINOUT
    "endl",

    ";" , 
    "," , 
    ":" , 
    "(" , 
    ")" , 
    "{" , 
    "}" , 
    "=" , 
    "<" , 
    ">" , 
    "Wrond operand's type for '+' or '-' operation" , // LEX_PLUS 
    "-" , 
    "Wrond operand's type for '++' or '--' operation", //LEX_IPLUS
    "--", 
    "Wrond operand's type for '*' or '/' or '%' operation" , //LEX_TIMES
    "/" , 
    "%",  
    "Wrond operand's type for '^' operation" , // LEX_POW
    "<=", 
    "!=", 
    ">=", 
    "==", 
    "!" , 

    "'>>' or '<<'", // LEX_IN
    "<<", 

    "Wrond operand's type for '||' operation", // LEX_OR
    "Wrond operand's type for '&&' operation", // LEX_AND

    "//", 
    "/*",

    "\"",
    "string",
    "variable name", // LEX_ID
    "int || double || bool" // LEX_TYPE
    
};

// int main() {
//     cout << "hello world" << endl;
//     return 0;
// }