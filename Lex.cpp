#pragma once
#include <iostream>
using namespace std;


enum type_of_lex {
    LEX_NULL, //пустая лексема

    //служебные слова
    LEX_MAIN,

    LEX_INT,
    LEX_DOUBLE,
    LEX_BOOL,

    LEX_TRUE,
    LEX_FALSE,
    
    LEX_DO,
    LEX_WHILE,
    LEX_IF,
    LEX_ELSE,
    LEX_FOR,

    LEX_TO,
    LEX_DOWNTO,

    LEX_CINOUT,
    LEX_ENDL,
    
    //разделители, знаки
    LEX_SEMICOLON, // ;
    LEX_COMMA,     // ,
    LEX_COLON,     // :
    
    LEX_LPAREN,    // (
    LEX_RPAREN,    // )
    LEX_LBRACE,    // {
    LEX_RBRACE,    // }
    
    LEX_EQ,        // =
    LEX_LSS,       // <
    LEX_GTR,       // >
    LEX_PLUS,      // +
    LEX_MINUS,     // -
    LEX_IPLUS,     // ++
    LEX_IMINUS,    // --
    LEX_TIMES,     // *
    LEX_SLASH,     // /
    LEX_MOD,       // %
    LEX_POW,       // ^
    LEX_LEQ,       // <=
    LEX_NEQ,       // !=
    LEX_GEQ,       // >=
    LEX_CMP,       // ==
    LEX_NOT,       // !

    LEX_IN,        // >>
    LEX_OUT,       // <<

    LEX_OR,        // ||
    LEX_AND,       // &&

    LEX_LCOM,      // //
    LEX_BCOM,      // /*
    LEX_DQUOTE,    // "
    
    LEX_STR,       // string
    
    //идентефикатор
    LEX_ID,
    LEX_TYPE,
    
    //нечто странное
    POLIZ_LABEL, // ссылка на номер ПОЛИЗА
    POLIZ_ADDRESS, // адрес в табоице идентификаторов
    POLIZ_GO, // безусловный переход
    POLIZ_FGO // переход по лжи
};


class Lex {
    type_of_lex t_lex; // тип лексемы
    string v_lex;      // номер строки в таблице лексем или значение
    int line;          // номер строки в исходном файле (для диагностики ошибок) 

public: 
    Lex (type_of_lex t = LEX_NULL, int line = -1, string v = "") {
        t_lex = t;
        v_lex = v;
        this->line = line;
    }

    type_of_lex get_type() const { return t_lex; }
    int get_line() const { return line; }
    string get_value() const { return v_lex; }

    friend ostream & operator <<(ostream & out, const Lex & l ) {
        out<<'('<<l.t_lex<<','<<l.v_lex<<','<<l.line<<");"<<endl;
        return out; 
    }

};
