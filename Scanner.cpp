#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <list>

#include "Exception.cpp"
#include "Lex.cpp"

using namespace std;

class Scanner {
    enum state { START, IDENT, INUMB, DNUMB, PLIMUS, ALE, LOGIC, DELIM, NEQ, COM, QUOTE};
    static const string TW[];   //таблица служебных слов
    static type_of_lex words[]; //таблица типов лексем службеных слов
    static const string TD[];   //таблица ограничителей
    static type_of_lex dlms[];  //таблица типов лексем ограничителей
    state CS;                   //текущее состояние
    FILE * fp;                  //указатель на файл
    char c;                     //текущий символ
    int line;
    string buf;
    int buf_top;                //вершина буфера

    void clear () {   
        buf_top = 0;
        buf = "";
    }

    void add () {
        if(c != EOF) {
            buf_top++;
            buf += c;
        }
    }

    bool if_letter(char c);
    bool if_digit(char c);
    bool if_valid(char c);

    int look (const string buf, const string *list ){
        int i = 1;
        while (list[i] != "") {
            if(buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }

    void gc () {
        c = fgetc (fp);
    }

    bool print_buf() { 
        cout << buf << endl;
        return true;
    }

public:
    Lex get_lex ();
    Scanner() {}

    ~Scanner() {
        pclose(fp);
    }

    Scanner (const char * program) {
        if((fp = fopen(program, "r")) == NULL) {
            printf ("Cannot open file.\n");
            exit(1); 
        }

        line = 1;

        CS = START;
        clear();
        gc();
    }
};

//таблица служебных слов

const string Scanner::TW[] = {
    "" ,        // 0 позиция 0 не используется
    "main",     // 1
    "int" ,     // 2
    "double",   // 3
    "bool" ,    // 4
    "true",     // 5
    "false",    // 6
    "do",       // 7
    "while",    // 8
    "if" ,      // 9
    "else",     // 10
    "for",      // 11
    "cinout",   // 12
    "endl",     // 13
    "to",       // 14
    "downto",   // 15
    
    ""
};

//таблица ограничителей
const string Scanner::TD[] ={
    "",   // 0 
    ";" , // 1
    "," , // 2
    ":" , // 3
    "(" , // 4
    ")" , // 5
    "{" , // 6
    "}" , // 7
    "=" , // 8
    "<" , // 9
    ">" , // 10
    "+" , // 11
    "-" , // 12
    "++", // 13
    "--", // 14
    "*" , // 15
    "/" , // 16
    "^" , // 17
    "<=", // 18
    "!=", // 19
    ">=", // 20
    "!" , // 21
    ">>", // 22
    "<<", // 23
    "||", // 24
    "&&", // 25
    "//", // 26
    "/*", // 27
    "%",  // 28
    "==", // 29

    ""
};

//таблица типов лексем службеных слов
type_of_lex Scanner::words[] ={
    LEX_NULL,

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

    LEX_CINOUT,
    LEX_ENDL,

    LEX_TO,
    LEX_DOWNTO,

    LEX_NULL
};

//таблица типов лексем ограничителей
type_of_lex Scanner::dlms[] ={
    LEX_NULL,

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
    LEX_POW,       // ^
    LEX_LEQ,       // <=
    LEX_NEQ,       // !=
    LEX_GEQ,       // >=
    LEX_NOT,       // !

    LEX_IN,        // >>
    LEX_OUT,       // <<

    LEX_OR,        // ||
    LEX_AND,       // &&

    LEX_LCOM,      // //
    LEX_BCOM,      // /*

    LEX_MOD,       // %
    LEX_CMP,       // ==

    LEX_NULL
};

bool Scanner::if_letter(char c) {
    if ((c >= 'a') && (c <= 'z') || (c >= 'A') && (c <= 'Z') || (c == '_'))
        return true;
    else
        return false;
}

bool Scanner::if_digit(char c) {
    if ((c >= '0') && (c <= '9'))
        return true;
    else
        return false;
}

bool Scanner::if_valid(char c) {
    return (c == ')' || c == ';' || isspace(c) || c == '+' || c == '-' || c == '=' || c == ':' ||
            c == '*' || c == '/' || c == '^'   || c == '%' || c == '|' || c == '&' || 
            c == '(' || c == '{' || c == '}'   || c == '<' || c == '>' || c == ',' || feof(fp));
}

Lex Scanner::get_lex () {
    int d, j;
    CS = START;
    do {
        switch (CS) {
            case START:
                if (isspace(c)) { 
                    if (c == '\n') line++;
                    gc();
                }
                else if (if_letter(c)) {
                    CS = IDENT;
                }
                else if (if_digit(c)) {
                    CS = INUMB;
                }
                else if (feof(fp)) {
                    return Lex(LEX_NULL, line);
                }
                else if (c == '<' || c == '>' || c == '=') {
                    CS = ALE;
                }
                else if ( c == '!' ) {
                    CS = NEQ;
                }
                else if (c == '+' || c == '-') {
                    CS = PLIMUS;
                }
                else if (c == '|' || c == '&') {
                    CS = LOGIC;
                } 
                else if (c == '/') {
                    CS = COM;
                }
                else if (c == '"') {
                    CS = QUOTE;
                }
                else
                    CS = DELIM;

                if(CS != DELIM && CS != START) {
                    clear ();
                    add ();
                    gc ();
                }
                break;

            case IDENT:
                if ((if_letter(c) || if_digit(c))) {
                    add ();
                    gc ();
                }
                else if(if_valid(c)) {
                    if (j = look(buf, TW)) {
                        if(j == LEX_TRUE) 
                            return Lex(LEX_BOOL, line ,"1");
                        else if(j == LEX_FALSE)
                            return Lex(LEX_BOOL, line,"0");
                        else
                            return Lex(words[j], line);
                    }
                    else {
                        return Lex (LEX_ID, line, buf);
                    }
                }
                else {
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
            
                break;

            case INUMB:
                if (if_digit(c)) {
                    add();
                    gc();
                }
                else if(c == '.') {
                    add();
                    gc();
                    CS = DNUMB;
                } 
                else if(if_valid(c))
                    return Lex (LEX_INT, line, buf);
                else { 
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
                
                break;
            case DNUMB:
                if (if_digit(c)) {
                    add();
                    gc();
                }
                else if(if_valid(c))
                    return Lex (LEX_DOUBLE, line, buf);
                else { 
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
                break;

            case ALE:
                if (c == '=' || c == '<' || c == '>') {
                    add();
                    gc();
                }
                
                if((j = look(buf,TD)) == 0) { 
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
                return Lex (dlms[j], line);
                
                break;

            case PLIMUS:
                if (c == '+' || c == '-') {
                    add();
                    gc();
                } 
                
                if((j = look(buf,TD)) == 0) { 
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
                return Lex(dlms[j], line);
            
                break;

            case LOGIC:
                if (c == '|' || c == '&') {
                    add();
                    gc();
                }

                if((j = look(buf,TD)) == 0) { 
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
                return Lex(dlms[j], line);
                break;

            case NEQ:
                if (c == '=') {
                    add();
                    gc();
                }
                
                j = look (buf, TD);
                return Lex(dlms[j],line);
            
                break;

            case COM: 
                if (c == '/' || c == '*') {
                    add();
                    gc();
                }
                j = look (buf, TD);
                if(dlms[j] == LEX_LCOM) {
                    while(c != '\n' && !feof(fp)) {
                        gc();
                    }
                    CS = START;
                } 
                else if(dlms[j] == LEX_SLASH)
                    return Lex(dlms[j], line);
                else if(dlms[j] == LEX_BCOM) {
                    clear();
                    while((buf != "*/") && !feof(fp)) {
                        buf = c;
                        gc();
                        add();
                        if(c == '\n') line++;
                    }
                    if(!feof(fp)) gc();
                    CS = START;
                }
            
                break;

            case QUOTE: 
                clear();
                while(!feof(fp) && c != '"') {
                    add();
                    gc();
                    if(c == '\n') line++;
                }
                if(feof(fp)) { 
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
                else {
                    gc();
                    return Lex(LEX_STR, line, buf);
                }
                break;

            case DELIM:
                clear(); 
                if(if_valid(c))
                    add();
                
                if (j = look(buf, TD)) {
                    gc();
                    
                    return Lex(dlms[j], line);
                }
                else { 
                    add();
                    throw Exception(Lex(LEX_NULL, line, buf), InvalidLexeme);
                }
                break;
        }// end of switch
    }
    while (true);
}

bool lexical_stage_pass(char* program) {
    try {
            Scanner S(program);
            Lex l;
            while (1) {
                l = S.get_lex(); 
                if (l.get_type() == LEX_NULL)
                    break;
                cout << l << endl;
            }
        }
        catch(Exception &l) {
            cout << l << endl;
            return false; 
        }
    cout << "lexical_stage PASSED" << endl;
    return true;
}
