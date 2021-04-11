#include <iostream>
#include <cmath>
#include "Lex.cpp"
#include "Stack.cpp"
#include "Poliz.cpp"
#include "Ident.cpp"
#include "Parser.cpp"

using namespace std;

class Executer { // обработчик всегоооооо
    Parser parser;
    Stack<Lex,100> stack;
    Stack<int, 100> minus;
    Stack<int, 100> plus;

    int index;

    Lex lex, lex_1, lex_2;
    Ident id;

    double d_op_1, d_op_2; // операнды
    int i_op_1, i_op_2; // операнды
    
    string tmp;

private: 
    void lex_from_address(Lex &lex, tabl_Ident &TID);
    void execute(tabl_Ident &TID, Poliz &P);
public:

    Executer(const char* program) : parser(program), index(0) { }
    ~Executer() {}
    
    void run() {
        parser.analize();
        
        execute(parser.TID, parser.P);
    }
};

void Executer::execute(tabl_Ident &TID, Poliz &P) {
    TID.print();
    P.print();
    while(index != P.get_top()) {

        lex = P[index++];

        switch(lex.get_type()) {

            case LEX_INT        :
            case LEX_BOOL       :
            case LEX_DOUBLE     :
            case LEX_STR        :
            case LEX_ENDL       :
            case POLIZ_ADDRESS  :
            case POLIZ_LABEL    :

                stack.push(lex);
                break;

            case LEX_SEMICOLON :
                stack.clear();

                while(!plus.empty()) {
                    i_op_1 = plus.pop();
                    d_op_1 = 1 + atof(TID[i_op_1].get_value().c_str());
                    if(TID[i_op_1].get_type() == LEX_DOUBLE) 
                        TID[i_op_1].put_value(to_string(d_op_1));
                    else 
                        TID[i_op_1].put_value(to_string((int)d_op_1));
                }

                while(!minus.empty()) {
                    i_op_1 = minus.pop();
                    d_op_1 = -1 + atof(TID[i_op_1].get_value().c_str());
                    if(TID[i_op_1].get_type() == LEX_DOUBLE) 
                        TID[i_op_1].put_value(to_string(d_op_1));
                    else 
                        TID[i_op_1].put_value(to_string((int)d_op_1));
                }

                break;
            
            case LEX_OUT:
                lex = stack.pop();
                switch(lex.get_type()) {
                    case LEX_ENDL :
                        cout << endl;
                        break;
                    case POLIZ_ADDRESS:
                        cout << TID[atoi(lex.get_value().c_str())].get_value();
                        break;
                    default :
                        cout << lex.get_value();
                        break;
                }   
                break;

            case LEX_IN: 
                lex = stack.pop();
                id = TID[atoi(lex.get_value().c_str())];
                
                cin >> tmp;
                
                switch (id.get_type()) {
                    case LEX_INT :
                        id.put_value(to_string(atoi(tmp.c_str())));
                        break;
                    case LEX_BOOL :
                        tmp = tmp == "true" ? "1" : "0";
                        id.put_value(to_string(atoi(tmp.c_str())));
                        break;
                    case LEX_DOUBLE :
                        id.put_value(to_string(atof(tmp.c_str())));
                        break;
                }

                TID[atoi(lex.get_value().c_str())] = id;

                break;

            case LEX_EQ:    

                lex_2 = stack.pop();

                if(lex_2.get_type() == POLIZ_ADDRESS) 
                    lex_from_address(lex_2, TID);

                lex_1 = stack.pop();
                
                id = TID[atoi(lex_1.get_value().c_str())];
                
                switch (id.get_type()) {
                    case LEX_INT :
                        id.put_value(to_string(atoi(lex_2.get_value().c_str())));
                        break;
                    case LEX_BOOL :
                        d_op_2 = atof(lex_2.get_value().c_str());
                        tmp = d_op_2 != 0 ? "1" : "0";
                        id.put_value(to_string(atoi(tmp.c_str())));
                        break;
                    case LEX_DOUBLE :
                        id.put_value(to_string(atof(lex_2.get_value().c_str())));
                        break;
                }

                TID[atoi(lex_1.get_value().c_str())] = id;
                lex_from_address(lex_1, TID);
                stack.push(lex_1);
                break;

            case LEX_PLUS:
            case LEX_MINUS:
            case LEX_POW:
            case LEX_SLASH:
            case LEX_TIMES:
            case LEX_MOD:

                lex_2 = stack.pop();
                if(lex_2.get_type() == POLIZ_ADDRESS) 
                    lex_from_address(lex_2, TID);
                
                
                lex_1 = stack.pop();
                if(lex_1.get_type() == POLIZ_ADDRESS)
                    lex_from_address(lex_1, TID);
                
                
                if(lex_1.get_type() == LEX_DOUBLE || lex_2.get_type() == LEX_DOUBLE) {
                    d_op_1 = atof(lex_1.get_value().c_str());
                    d_op_2 = atof(lex_2.get_value().c_str());

                    switch(lex.get_type()) {
                        case LEX_PLUS: 
                            d_op_1 += d_op_2;
                            break;
                        case LEX_MINUS:
                            d_op_1 -= d_op_2;
                            break;
                        case LEX_POW:
                            d_op_1 = pow(d_op_1, d_op_2);
                            break;
                        case LEX_SLASH:
                            if(d_op_2 == 0) 
                                throw Exception(lex_2,Division_by_zero);
                            d_op_1 /= d_op_2;
                            break;
                        case LEX_TIMES:
                            d_op_1 *= d_op_2;
                            break;
                    }   

                    stack.push(Lex(LEX_DOUBLE, -1, to_string(d_op_1)));
                }
                else {
                    i_op_1 = atoi(lex_1.get_value().c_str());
                    i_op_2 = atoi(lex_2.get_value().c_str());

                    switch(lex.get_type()) {
                        case LEX_PLUS: 
                            i_op_1 += i_op_2;
                            break;
                        case LEX_MINUS:
                            i_op_1 -= i_op_2;
                            break;
                        case LEX_POW:
                            i_op_1 = pow(i_op_1, i_op_2);
                            break;
                        case LEX_SLASH:
                            if(i_op_2 == 0) 
                                throw Exception(lex_2,Division_by_zero);
                            i_op_1 /= i_op_2;
                            break;
                        case LEX_TIMES:
                            i_op_1 *= i_op_2;
                            break;
                        case LEX_MOD:
                            if(i_op_2 == 0) 
                                throw Exception(lex_2,Division_by_zero);
                            i_op_1 %= i_op_2;
                            break;
                    }

                    stack.push(Lex(LEX_INT, -1, to_string(i_op_1)));
                }

                break;
            case LEX_LSS:
            case LEX_GTR:
            case LEX_LEQ:
            case LEX_NEQ:
            case LEX_CMP:
            case LEX_GEQ:
            case LEX_AND:
            case LEX_OR:
                
                lex_2 = stack.pop();
                if(lex_2.get_type() == POLIZ_ADDRESS) 
                    lex_from_address(lex_2, TID);
                
                
                lex_1 = stack.pop();
                if(lex_1.get_type() == POLIZ_ADDRESS)
                    lex_from_address(lex_1, TID);
                
                //cout << lex_1.get_value() << " " << lex_2.get_value() << endl;

                d_op_1 = atof(lex_1.get_value().c_str());
                d_op_2 = atof(lex_2.get_value().c_str());

                switch(lex.get_type()) {
                    case LEX_LSS: 
                        d_op_1 = d_op_1 < d_op_2;
                        break;
                    case LEX_GTR: 
                        d_op_1 = d_op_1 > d_op_2;
                        break;
                    case LEX_LEQ: 
                        d_op_1 = d_op_1 <= d_op_2;
                        break;
                    case LEX_GEQ: 
                        d_op_1 = d_op_1 >= d_op_2;
                        break;
                    case LEX_CMP: 
                        //cout << d_op_1 << " " << d_op_2 << endl;
                        d_op_1 = d_op_1 == d_op_2;
                        break;
                    case LEX_NEQ: 
                        d_op_1 = d_op_1 != d_op_2;
                        break;
                    case LEX_AND:
                        d_op_1 = d_op_1 && d_op_2;
                        break;
                    case LEX_OR:
                        d_op_1 = d_op_1 || d_op_2;
                        break;
                }   

                stack.push(Lex(LEX_BOOL, -1, to_string((int) d_op_1)));

                break;

            case LEX_NOT:
                lex_1 = stack.pop();
                if(lex_1.get_type() == POLIZ_ADDRESS)
                    lex_from_address(lex_1, TID);

                d_op_1 = atof(lex_1.get_value().c_str());
                tmp = d_op_1 != 0 ? "0" : "1";
                
                stack.push(Lex(LEX_BOOL, -1, tmp));

            break;
            
            case POLIZ_GO:
                lex_1 = stack.pop();
                index = atoi(lex_1.get_value().c_str());
                stack.push(Lex(LEX_BOOL, -1, "1"));
                break;

            case POLIZ_FGO:

                lex_2 = stack.pop();
                
                lex_1 = stack.pop();
                if(lex_1.get_type() == POLIZ_ADDRESS)
                    lex_from_address(lex_1, TID);

                d_op_1 = atof(lex_1.get_value().c_str());

                if(d_op_1 == 0) {
                    index = atoi(lex_2.get_value().c_str());
                    stack.push(Lex(LEX_BOOL, -1, "0"));
                }
                else stack.push(Lex(LEX_BOOL, -1, "1"));
                
                break;
            case LEX_IPLUS:
                lex_1 = stack.pop();
                stack.push(lex_1);
                plus.push(atoi(lex_1.get_value().c_str()));
                break;

             case LEX_IMINUS:
                lex_1 = stack.pop();
                stack.push(lex_1);
                minus.push(atoi(lex_1.get_value().c_str()));
                break;
        }        
    }
}

void Executer:: lex_from_address(Lex &lex, tabl_Ident &TID) {
    Ident id = TID[atoi(lex.get_value().c_str())];
    lex = Lex(id.get_type(), -1, id.get_value());
}


int main(int argc, char** argv) {
    if(argc == 2) {
        Executer exec(argv[1]);
    
        try {
            exec.run();
        } 
		catch(Exception &ex) {
			cout << "Error:" << endl;
			cout << ex;
			exit(1);
		}
		catch(const char* str) {
			cout << "Error:" << endl;
			cout << str << endl;
			exit(1);
		}
		catch (Lex &l) {
			
			cout << "Error:" << l;
			exit(1);
		}
        catch (...) {
            cout << "UnlnownError";
			exit(1);
        }
        //cout << "Success" << endl;
    } 
    else cout << "Bad arguments" << endl;
    return 0;
}