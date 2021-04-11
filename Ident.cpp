#pragma once
#include <iostream>
#include <string>
#include "Lex.cpp"
using namespace std;

class Ident {
	string name;
	string value;
	type_of_lex type;
	int name_space;   //пространсвто имен 

public:

	Ident(const Lex &lex = Lex(), int name_space = -1, type_of_lex type = LEX_NULL) {
		this->name = lex.get_value();
		this->name_space = name_space;
		this->type = type;
		this->value = "0";
	}

	string get_name() 	   { return name;  		}
	string get_value()     { return value; 		}

	void put_value(string value) { this->value = value; }

	type_of_lex get_type() { return type;  		} 
	int get_namespace()    { return name_space; }
};

//класс таблица идентефикаторов
class tabl_Ident {
	Ident * p; //массив идентефикаторов
	int size;  //размер
	int top;  //текущая позиция

public:

	tabl_Ident(int max_size = 256) {
		p = new Ident[size = max_size];
		top = 0;
	}

	~tabl_Ident() { delete[] p; }

	Ident& operator[] (int k) { return p[k]; }

	void put(const Lex &lex = Lex(), int name_space = -1, type_of_lex type = LEX_NULL) {
		p[top] = Ident(lex, name_space, type);
		++top;
	}

	int find(const Lex &lex, int name_space) {
		
		string name = lex.get_value();
		for(int i = top -1; i >= 0; i --) {
			
			if(p[i].get_namespace() < name_space)
				name_space--;

			if(p[i].get_namespace() == name_space) {
				if(p[i].get_name() == name)
					return i;
			} 
			
		}
		return -1;
	}

	bool is_exist_name(const Lex &lex, int name_space) {
		string name = lex.get_value();
		for(int i = top -1; i >= 0; i --) {
			if(p[i].get_namespace() < name_space)
				return false;

			if(p[i].get_namespace() == name_space) {
				if(p[i].get_name() == name)
					return true;
			} 
		}
		return false;
	}

	void clear_namespace(bool f) {
		if(f) print();
		int name_space = p[top-1].get_namespace();
		for(int i = top - 1; i >= 0; i--) {
			if(p[i].get_namespace() == name_space) 
				top--;
			else return;
		}
	}

	void print() {
		cout << "TID : ";
			for(int i = 0; i < top; i++) {
				cout << "(" << p[i].get_name() << " ," << p[i].get_type() << " ," << p[i].get_namespace() << ")  ";
			}
			cout << endl; 
	}
};
