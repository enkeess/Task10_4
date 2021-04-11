#pragma once
#include <iostream>
#include <list>

#include "Ident.cpp"
#include "Stack.cpp"
#include "Poliz.cpp"
#include "Scanner.cpp"

#define TID_SIZE 100
#define POLIZ_SIZE 1000
#define PRINT_TID 0
#define STACK_TRACE 0
#define PRINT_STACK 0
#define PRINT_POLIZ 0
#define DEBUG 0

using namespace std;

class Parser {
    Scanner S;			              //
    Lex lex;			              //
    //--------------------------------//
    type_of_lex lex_t = LEX_LCOM;	  //
    string lex_v;	  	              //
    //--------------------------------//
    bool have_main;					  //

	Stack<Lex, 1> buf;
	Stack<Lex, 1000> stack;

	int name_space;

public:
	
	tabl_Ident TID;
	Poliz P;

private: 
    void program();
	
	void oper(bool is_fin, bool from_special_operation);
    
	void expression_operator(bool is_fin);
    void expression(bool is_fin);
    void expression_one(bool is_fin);
	void expression_two(bool is_fin);
	void expression_three(bool is_fin);
	void expression_four(bool is_fin);
    void simple_expression(bool is_fin);
    void term(bool is_fin);
    void atom_one(bool is_fin);
    void atom(bool is_fin);
    void special_atom(bool is_fin);
    
    bool addition_operation();
    bool multiplication_operation();
    bool exponentiation();
    bool assignment_operation();
    bool increment();
    bool relationship_operation();
	bool relationship_operation_one();
    bool type();
    bool sign();
    bool boolean_value();

    void cinout(bool is_fin);
    void list_of_items(bool is_fin);
    void item(bool is_fin);

	void composite_operator(bool is_fin, bool from_special_operation);
    
    void description(bool is_fin);
    void section(bool is_fin, type_of_lex type);
    void constant(bool is_fin);

    void special_operation(bool is_fin);

    void dowhile_operation(bool is_fin);
    void for_operation(bool is_fin);
    void while_operation(bool is_fin);
    void if_operation(bool is_fin);

    void push_buf();
    void fin();

public :

    Parser(const char* program) : S(program), have_main(false), name_space(0), P(1000) {}
    ~Parser() { buf.clear(); stack.clear(); }

    void analize() {
        program();
		if(PRINT_TID) TID.print();

     	if(PRINT_STACK) {
			 cout << "from main : ";
			 stack.print();
		}

		if (PRINT_POLIZ) {
			cout << "Poliz: " << endl;
			P.print();
		}
    }

};

void Parser::fin() {
	if (buf.empty())
		lex = S.get_lex();
	else {
		lex = buf.pop();
	}

	lex_t = lex.get_type();
	lex_v = lex.get_value();
}

void Parser::push_buf() {
	buf.push(lex);
}

void Parser::program() {
	if(STACK_TRACE) { cout<< "program" << endl; }

	while (lex_t != LEX_MAIN && lex_t != LEX_NULL) {
		description(true);
	}

	if (lex_t != LEX_MAIN)
		throw Exception(lex, SyntaxError, LEX_MAIN);

	fin();
	if (lex_t != LEX_LPAREN)
		throw Exception(lex, SyntaxError, LEX_LPAREN);

	fin();
	if (lex_t != LEX_RPAREN)
		throw Exception(lex, SyntaxError, LEX_RPAREN);
		
	composite_operator(true, false);

	fin();
	if (lex_t != LEX_NULL)
		throw Exception(lex, NotEndOfFIle);
}

void Parser::description(bool is_fin) {

	if(STACK_TRACE) { cout<< "description" << endl; }
	if (is_fin) fin();

	if (type()) {
		type_of_lex type = lex_t;
		section(true, type);
		if (lex_t == LEX_MAIN)
			return;
		fin();
		while (lex_t == LEX_COMMA) {
			P.put_lex(Lex(LEX_SEMICOLON));
			section(true, type);
			fin();
		}
		
		if (lex_t != LEX_SEMICOLON)
			throw Exception(lex, SyntaxError, LEX_SEMICOLON); // ожидалось ";"
		P.put_lex(lex);
	}
	else throw Exception(lex, SyntaxError, LEX_TYPE); // должен быть тип переменной
}

void Parser::section(bool is_fin, type_of_lex type) { // выходит со считанным символом или main
	if(STACK_TRACE) { cout<< "section" << endl; }
	if (is_fin) fin();
	if (lex_t == LEX_MAIN) {
		if (type == LEX_INT && !have_main) {
			have_main = true;
			return;
		}
		else if(have_main) 
			throw Exception(lex, Redefinition); // ожидалось int main получена бяка
		else throw Exception(lex, SyntaxError);
	}

	if (lex_t == LEX_ID) {

		// - надо проверить есть ли уже такое имя
		if(!TID.is_exist_name(lex, name_space)) {
			TID.put(lex, name_space, type);
		}
		else throw Exception(lex , Redefinition); // двойное определение переменной

		int index = TID.find(lex, name_space);
		// - надо положить в TID имя если его нет

		fin();
		if (lex_t == LEX_EQ) {
			
			if(DEBUG) { cout << "From section: " << endl; P.print();}
			P.put_lex(Lex(POLIZ_ADDRESS, name_space, to_string(index)));
			expression(true);

			P.put_lex(Lex(LEX_EQ));

			if(PRINT_STACK) {
			 cout << "from section : ";
			 stack.print();
			}

			stack.pop();
		}
		else push_buf();
	}
	else throw Exception(lex, SyntaxError, LEX_ID); // ожидалось имя переменной а получено lex
}

void Parser::expression_operator(bool is_fin) {
	if(STACK_TRACE) { cout<< "expression_operator" << endl; }
	if (is_fin) fin();
	expression(false);
	fin();

	
	if(PRINT_STACK) {
			cout << "from expression_operator : ";
			stack.print();
	}

	Lex tmp = stack.pop();

	if (lex_t != LEX_SEMICOLON)
		throw Exception(lex, SyntaxError, LEX_SEMICOLON); // ожидалось ";"
	P.put_lex(lex);

}

void Parser::expression(bool is_fin) {
	if(STACK_TRACE) { cout<< "expression" << endl; }
	if (is_fin) fin();
	if (lex_t == LEX_ID) {
		int index;
		//нужно проверить описана ли переменная
		if((index = TID.find(lex, name_space)) == -1) {
			throw Exception(lex , UndeclaredIdentifier); // неописанная переменная
		}

		Lex tmp = lex;
		fin();
		if (lex_t == LEX_EQ) {
			P.put_lex(Lex(POLIZ_ADDRESS, name_space, to_string(index)));
			expression(true);
			
			P.put_lex(Lex(LEX_EQ));

			if(PRINT_STACK) {
				cout << "from expression : ";
				stack.print();
			}
			Lex tmp_1 = stack.pop();
			
			stack.push(Lex(TID[index].get_type()));

		}
		else {
			push_buf();
			lex = tmp;
            lex_t = tmp.get_type();
            lex_v = tmp.get_value();
			expression_four(false);
		}
	}
	else expression_four(false);
}

void Parser::expression_four(bool is_fin) {
	if(STACK_TRACE) { cout<< "expression_four" << endl; }
	if (is_fin) fin();
	expression_three(false);
	fin();
	
	while (lex_t == LEX_OR) {

		// надо добавить переход по лжи на вычисление след условия
		// переход по истине на выход из выражения 
		P.put_lex(Lex(POLIZ_LABEL, -1, to_string(P.get_top() + 4)));   //101
		P.put_lex(POLIZ_FGO);

		//P.put_lex(Lex(LEX_BOOL, -1 , "1"));

		int if_true = P.get_top();
		P.blank();
		P.put_lex(POLIZ_GO);

		Lex tmp = lex;
		expression_three(true);

		//P.put_lex(tmp); // при ленивых вычислениях не нужно класть || // 101

		int address = P.get_top();                                      // 101
		P.put_lex(Lex(POLIZ_LABEL, -1, to_string(address)), if_true);

		if(PRINT_STACK) {
			 cout << "from expression_four : ";
			 stack.print();
		}

		Lex tmp_1 = stack.pop();
		Lex tmp_2 = stack.pop();

		stack.push(Lex(LEX_BOOL));

		fin();
	}
	push_buf();
}

void Parser::expression_three(bool is_fin) {
	if(STACK_TRACE) { cout<< "expression_three" << endl; }
	if (is_fin) fin();
	expression_two(false);
	fin();
	
	while (lex_t == LEX_AND) {
		
		int if_false = P.get_top();  // 101
		P.blank();
		P.put_lex(POLIZ_FGO);

		Lex tmp = lex;
		expression_two(true);

		
		int address = P.get_top();                                      // 101
		P.put_lex(Lex(POLIZ_LABEL, -1, to_string(address)), if_false);

		//P.put_lex(tmp);												// 101

		if(PRINT_STACK) {
			 cout << "from expression_three : ";
			 stack.print();
		}

		Lex tmp_1 = stack.pop();
		Lex tmp_2 = stack.pop();

		stack.push(Lex(LEX_BOOL));

		fin();
	}
	push_buf();
}

void Parser::expression_two(bool is_fin) {
	if(STACK_TRACE) { cout<< "expression_two" << endl; }
	if (is_fin) fin();
	expression_one(false);
	fin();
	
	while (relationship_operation_one()) {
		Lex tmp = lex;
		expression_one(true);

		P.put_lex(tmp);

		if(PRINT_STACK) {
			 cout << "from expression_two : ";
			 stack.print();
		}

		Lex tmp_1 = stack.pop();
		Lex tmp_2 = stack.pop();

		stack.push(Lex(LEX_BOOL));

		fin();
	}
	push_buf();
}

void Parser::expression_one(bool is_fin) {
	if(STACK_TRACE) { cout<< "expression_one" << endl; }
	if (is_fin) fin();
	simple_expression(false);
	fin();
	
	while (relationship_operation()) {
		Lex tmp = lex;
		simple_expression(true);

		P.put_lex(tmp);

		if(PRINT_STACK) {
			 cout << "from expression_one : ";
			 stack.print();
		}

		Lex tmp_1 = stack.pop();
		Lex tmp_2 = stack.pop();

		stack.push(Lex(LEX_BOOL));

		fin();
	}
	push_buf();
}

void Parser::cinout(bool is_fin) {
	if(STACK_TRACE) { cout<< "cinout" << endl; }
	if (is_fin) fin();

	list_of_items(true);
	fin();

	if (lex_t != LEX_SEMICOLON)
		throw Exception(lex, SyntaxError, LEX_SEMICOLON); // ожидалось ";"
	P.put_lex(lex);
}

void Parser::list_of_items(bool is_fin) {
	if(STACK_TRACE) { cout<< "list_of_items" << endl; }
	if (is_fin) fin();
	if (lex_t != LEX_IN && lex_t != LEX_OUT)
		throw Exception(lex, SyntaxError, LEX_IN); //ожидалось список элеменов на вывод
	while (lex_t == LEX_IN || lex_t == LEX_OUT) {
		Lex tmp = lex;
		item(false);

		P.put_lex(tmp);

		fin();
	}
	push_buf();
}

void Parser::item(bool is_fin) {
	if(STACK_TRACE) { cout<< "item" << endl; }
	if (is_fin) fin();
	if (lex_t == LEX_OUT) {
		fin();
		if (lex_t == LEX_STR || lex_t == LEX_ENDL) {
			P.put_lex(lex);
			return;
		}
		else { 
			expression(false);
			if(PRINT_STACK) {
				cout << "from item : ";
				stack.print();
			}
			stack.pop();
		}
	}
	else if (lex_t == LEX_IN) {
		fin();
		if (lex_t != LEX_ID)
			throw Exception(lex, SyntaxError, LEX_ID); // ожидалась переменная
		//нужно проверить описана ли переменная
		int index;
		if((index = TID.find(lex, name_space)) == -1)
			throw Exception(lex, UndeclaredIdentifier); // неописанная переменная

		if(DEBUG) { cout << "From item : " << endl; P.print();}
			P.put_lex(Lex(POLIZ_ADDRESS, lex.get_line(), to_string(index)));
		//
	}
	else throw Exception(lex, SyntaxError); // ожидалось что то на ввод/вывод
}

void Parser::simple_expression(bool is_fin) {
	if(STACK_TRACE) { cout<< "simple_expression" << endl; }
	if (is_fin) fin();
	term(false);
	fin();
	while (addition_operation()) {
		Lex tmp = lex;

		term(true);

		P.put_lex(tmp);

		if(PRINT_STACK) {
			 cout << "from simple_expression : ";
			 stack.print();
		}
		Lex tmp_1 = stack.pop();
		Lex tmp_2 = stack.pop();

		stack.push(tmp_1.get_type() > tmp_2.get_type() ? tmp_1 : tmp_2);

		fin();
	}
	push_buf();
}

void Parser::term(bool is_fin) {
	if(STACK_TRACE) { cout<< "term" << endl; }
	if (is_fin) fin();
	atom_one(false);
	fin();
	while (multiplication_operation()) {
		
		Lex tmp = lex;

		atom_one(true);

		P.put_lex(tmp);

		if(PRINT_STACK) {
			 cout << "from term : ";
			 stack.print();
		}
		Lex tmp_1 = stack.pop();
		Lex tmp_2 = stack.pop();

		if(tmp.get_type() == LEX_MOD && (tmp_1.get_type() != LEX_INT || tmp_2.get_type() != LEX_INT)) {
			throw Exception(lex,TypeError, LEX_MOD);
		} 
		else stack.push(tmp_1.get_type() > tmp_2.get_type() ? tmp_1 : tmp_2);

		fin();
	}
	push_buf();
}

void Parser::atom_one(bool is_fin) {
	if(STACK_TRACE) { cout<< "atom_one" << endl; }
	if (is_fin) fin();
	atom(false);
	fin();
	if (exponentiation()) {
		atom(true);

		P.put_lex(LEX_POW);

		if(PRINT_STACK) {
			 cout << "from atom_one exponentiation : ";
			 stack.print();
		}
		Lex tmp_1 = stack.pop();
		Lex tmp_2 = stack.pop();

		if(tmp_1.get_type() == LEX_BOOL || tmp_2.get_type() == LEX_BOOL)
			throw Exception(lex,TypeError, LEX_POW);
		stack.push(tmp_1.get_type() > tmp_2.get_type() ? tmp_1 : tmp_2);
	}
	else if (!increment())
		push_buf();
	else {
		P.put_lex(lex);

		if(PRINT_STACK) {
			 cout << "from atom_one increment : ";
			 stack.print();
		}
		Lex tmp_1 = stack.pop();

		if(tmp_1.get_type() == LEX_BOOL || tmp_1.get_value() != "")
			throw Exception(lex,TypeError, LEX_IPLUS);
		stack.push(tmp_1);
	}
}

void Parser::atom(bool is_fin) {
	if(STACK_TRACE) { cout<< "atom" << endl; }
	if (is_fin) fin();
	if (lex_t == LEX_LPAREN) {
		expression(true); // 
		fin();
		if (lex_t != LEX_RPAREN)
			throw Exception(lex, SyntaxError, LEX_RPAREN); // ожидалось ")"
	}
	else if (lex_t == LEX_ID) {
		//нужно проверить описана ли переменная
		int index;
		if((index = TID.find(lex, name_space)) == -1)
			throw Exception(lex, UndeclaredIdentifier); // неописанная переменная
		
		if(DEBUG) { cout << "From atom : " << endl; P.print();}
			P.put_lex(Lex(POLIZ_ADDRESS, name_space, to_string(index)));

		stack.push(Lex(TID[index].get_type()));
		//
	} 
	else special_atom(false);
}

void Parser::special_atom(bool is_fin) {
	if(STACK_TRACE) { cout<< "special_atom" << endl; }
	if (is_fin) fin();
	if (lex_t == LEX_NOT) {
		atom(true);

		P.put_lex(LEX_NOT);

		if(PRINT_STACK) {
			 cout << "from atom_one special_atom : ";
			 stack.print();
		}

		Lex tmp_1 = stack.pop(); // проверить что выражение типа bool
		if(tmp_1.get_type() != LEX_BOOL)
			throw Exception(lex,TypeError, LEX_TRUE);
		stack.push(tmp_1);

	}
	else if (sign() || lex_t == LEX_INT || lex_t == LEX_DOUBLE)
		constant(false);
	else if (!boolean_value())
		throw Exception(lex, SyntaxError, LEX_TRUE); // ожидалось false / true 
	else { 
		stack.push(lex);
		P.put_lex(lex);
	}
}

void Parser::constant(bool is_fin) {
	if(STACK_TRACE) { cout<< "constant" << endl; }
	if (is_fin) fin();
	if (sign()) {
		bool minus = lex_t == LEX_MINUS;
		fin();
		if (lex_v != "" && lex_t != LEX_ID) {
			if (minus)
				lex_v = "-" + lex_v;
			stack.push(Lex(lex_t, lex.get_line() ,lex_v));
			P.put_lex(Lex(lex_t, lex.get_line() ,lex_v));
		}
		else throw Exception(lex, SyntaxError);
	}
	else if (lex_v == "")
		throw Exception(lex, SyntaxError); // ожидалось увидеть чиселку 
	else { 
		stack.push(lex);
		P.put_lex(lex);
	}
}

bool Parser::boolean_value() {
	return (lex_t == LEX_BOOL && lex_v != "");
}

bool Parser::sign() {
	return (lex_t == LEX_PLUS || lex_t == LEX_MINUS);
}

bool Parser::addition_operation() {
	return (lex_t == LEX_PLUS || lex_t == LEX_MINUS);
}

bool Parser::multiplication_operation() {
	return (lex_t == LEX_TIMES || lex_t == LEX_SLASH ||
		    lex_t == LEX_MOD);
}

bool Parser::exponentiation() {
	return (lex_t == LEX_POW);
}

bool Parser::assignment_operation() {
	return (lex_t == LEX_EQ);
}

bool Parser::increment() {
	return (lex_t == LEX_IPLUS || lex_t == LEX_IMINUS);
}

bool Parser::type() {
	return (lex_t == LEX_INT || lex_t == LEX_BOOL || lex_t == LEX_DOUBLE);
}

bool Parser::relationship_operation() {
	return (lex_t == LEX_LSS || lex_t == LEX_GTR);
}

bool Parser::relationship_operation_one() {
	return (lex_t == LEX_NEQ || lex_t == LEX_LEQ ||
			lex_t == LEX_GEQ || lex_t == LEX_CMP);
}

void Parser::composite_operator(bool is_fin, bool from_special_operation) {
	if(STACK_TRACE) { cout<< "composite_operator" << endl; }
	if (is_fin) fin();

	if(!from_special_operation) { 
		name_space++; 
		TID.put(Lex(), name_space);
	}
	
	if (lex_t != LEX_LBRACE)
		throw Exception(lex, SyntaxError, LEX_LBRACE);

	fin();
	if(lex_t == LEX_RBRACE)
		throw Exception(lex, EmptyOperation);
	while (lex_t != LEX_RBRACE && lex_t != LEX_NULL) {
		
		oper(false, false);
		fin();
	}
	if(!from_special_operation) {
	
		name_space--;
	}

	if (lex_t == LEX_NULL)
		throw Exception(lex, EndOfFile);
}

void Parser::special_operation(bool is_fin) {
	if(STACK_TRACE) { cout<< "special_operation" << endl; }

	if (is_fin) fin();
	if (lex_t == LEX_DO)
		dowhile_operation(false);
	else if (lex_t == LEX_FOR)
		for_operation(false);
	else if (lex_t == LEX_IF)
        if_operation(false);
    else while_operation(false);
}

void Parser::oper(bool is_fin, bool from_special_operation) {
	if(STACK_TRACE) { cout<< "oper" << endl; }
	if (is_fin) fin();
	switch (lex_t) {
	case LEX_LBRACE:
		composite_operator(false, from_special_operation);

		break;

	case LEX_INT:
	case LEX_DOUBLE:
	case LEX_BOOL:
		description(false);
		break;

	case LEX_CINOUT:
		cinout(false);
		break;

	case LEX_DO:
	case LEX_FOR:
    case LEX_IF:
    case LEX_WHILE:
		special_operation(false);
		break;

	default:
		expression_operator(false);
		break;
	}
}

void Parser::while_operation(bool is_fin) {
	if(STACK_TRACE) { cout<< "while" << endl; }
    if(is_fin) fin();

	int while_start = P.get_top();

	name_space++;
	TID.put(Lex(), name_space);
	
    fin();
    if(lex_t != LEX_LPAREN)
        throw Exception(lex, SyntaxError, LEX_LPAREN);
    expression(true);
	if(PRINT_STACK) {
		 cout << "from while_operation : ";
		 stack.print();
	}
	stack.pop();
    fin();
    if(lex_t != LEX_RPAREN)
        throw Exception(lex, SyntaxError, LEX_RPAREN);
    
	int tmp = P.get_top(); // адрес после oper
	P.blank();
	P.put_lex(POLIZ_FGO);

	oper(true, true);

	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(while_start)));
	P.put_lex(POLIZ_GO);

	int end_of_oper = P.get_top();
	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(end_of_oper)), tmp);

	name_space--;
}

void Parser::if_operation(bool is_fin) {
	if(STACK_TRACE) { cout<< "if_operation" << endl; }
    if(is_fin) fin();

	
    fin();
    if(lex_t != LEX_LPAREN)
        throw Exception(lex, SyntaxError, LEX_LPAREN);
    expression(true);

	int if_false = P.get_top();
	P.blank();
	P.put_lex(POLIZ_FGO);

	if(PRINT_STACK) {
		 cout << "from if_operation : ";
		 stack.print();
	}
	stack.pop();
    fin();
    if(lex_t != LEX_RPAREN)
        throw Exception(lex, SyntaxError, LEX_RPAREN);
    
	name_space++;
	TID.put(Lex(), name_space);

	oper(true, true);

	int end_of_else = P.get_top();
	
	P.blank();
	P.put_lex(POLIZ_GO);

	int tmp = P.get_top();

	P.put_lex(Lex(POLIZ_LABEL, -1 , to_string(tmp)), if_false);

	name_space--;
    
	fin();
    if(lex_t == LEX_ELSE) {

		TID.put(Lex(), name_space);

		oper(true, true);
		tmp = P.get_top();
	}
    else {
        push_buf();
    }

	P.put_lex(Lex(POLIZ_LABEL, -1 , to_string(tmp)), end_of_else);

}

void Parser::dowhile_operation(bool is_fin) {
	if(STACK_TRACE) { cout<< "dowhile" << endl; }
	if (is_fin) fin();

	int do_start = P.get_top(); //начало do

	name_space++;
	TID.put(Lex(), name_space);
	
	oper(true, true);

	fin();
	if (lex_t != LEX_WHILE)
		throw Exception(lex, SyntaxError, LEX_WHILE); 
	
	name_space--;
	fin();
	if (lex_t != LEX_LPAREN)
		throw Exception(lex, SyntaxError, LEX_LPAREN);
	expression(true);

	int tmp = P.get_top();
	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(tmp + 4)));
	P.put_lex(POLIZ_FGO);
	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(do_start)));
	P.put_lex(POLIZ_GO);

	if(PRINT_STACK) {
		 cout << "from dowhile_operation : ";
		 stack.print();
	}
	stack.pop();

	fin();
	if (lex_t != LEX_RPAREN)
		throw Exception(lex, SyntaxError, LEX_RPAREN);
	fin();
	if (lex_t != LEX_SEMICOLON)
		throw Exception(lex, SyntaxError, LEX_SEMICOLON);
}

void Parser::for_operation(bool is_fin) {
	if(STACK_TRACE) { cout<< "for" << endl; }

	name_space++;
	TID.put(Lex(), name_space);
	
	if (is_fin) fin();

	fin();
	if (lex_t != LEX_LPAREN)
		throw Exception(lex, SyntaxError, LEX_LPAREN);

	fin();
	if (lex_t != LEX_SEMICOLON) {
		expression(false);
		if(PRINT_STACK) {
			 cout << "from for_operation 1 : ";
			 stack.print();
		}
		stack.pop();
		fin();
	}
	if (lex_t != LEX_SEMICOLON)
		throw Exception(lex, SyntaxError, LEX_SEMICOLON);
	P.put_lex(lex);

	int start_condition = P.get_top();
	
	fin();
	if (lex_t != LEX_SEMICOLON) {
		expression(false);
		if(PRINT_STACK) {
			 cout << "from for_operation 2 : ";
			 stack.print();
		}
		stack.pop();
		fin();
	} else P.put_lex(Lex(LEX_BOOL, -1, "1")); // true

	if (lex_t != LEX_SEMICOLON)
		throw Exception(lex, SyntaxError, LEX_SEMICOLON);

	int addres_if_false = P.get_top();
	P.blank();
	P.put_lex(POLIZ_FGO);
	int addres_if_true = P.get_top();
	P.blank();
	P.put_lex(POLIZ_GO);

	int start_action = P.get_top();
	fin();
	if (lex_t != LEX_RPAREN) {
		expression(false);
		if(PRINT_STACK) {
			 cout << "from for_operation 3 : ";
			 stack.print();
		}
		stack.pop();
		fin();
	}
	if (lex_t != LEX_RPAREN)
		throw Exception(lex, SyntaxError, LEX_RPAREN);
	P.put_lex(LEX_SEMICOLON);  

	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(start_condition)));
	P.put_lex(POLIZ_GO);

	int start_oper = P.get_top();
	oper(true, true);

	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(start_action)));
	P.put_lex(POLIZ_GO);

	int end_oper = P.get_top();

	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(start_oper)), addres_if_true);
	P.put_lex(Lex(POLIZ_LABEL, -1, to_string(end_oper)), addres_if_false);

	name_space--;
}
