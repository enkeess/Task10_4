// #define NDEBUG
#include <cassert>
#include <iostream>
#include <fstream>

#include "Scanner.cpp"

using namespace std;

void files_equals(string s1, string s2) {
    string str1, str2;
    ifstream fin1(s1);
    ifstream fin2(s2);
    int line = 1;
    while(!fin1.eof() && !fin2.eof()) {
        getline(fin1, str1);
        getline(fin2, str2);
        if(str1 != str2) {
            cout << "Error in line : " << line << endl;
            cout << "Received : " << str2 << endl;
            cout << "Exepted : " << str1 << endl;
            exit(0);
        }
        line++;
    }

    assert(fin1.eof() && fin2.eof()); 
    fin1.close();
    fin2.close();
}

void test_keywords() {
    try {
        Scanner S("lexical_stage_tests/test_keywords.txt");
        ofstream fout("test_keywords_out.txt");

        Lex l;
        while (1) {
            l = S.get_lex(); 
            if (l.get_type() != LEX_NULL)
                fout << l;
            else 
                break;
        }
        
        fout.close();
        
        files_equals("lexical_stage_tests/test_keywords_key.txt", "test_keywords_out.txt");

        cout << "test_keywords      DONE" << endl;

    } catch(Exception &l) {
        cout << l << endl;
    }

    remove("test_keywords_out.txt");
}


void test_delimetrs() {
    try {
        Scanner S("lexical_stage_tests/test_delimetrs.txt");
        ofstream fout("test_delimetrs_out.txt");

        Lex l;
        while (1) {
            l = S.get_lex(); 
            if (l.get_type() != LEX_NULL)
                fout << l;
            else 
                break;
        }
        
        fout.close();
        
        files_equals("lexical_stage_tests/test_delimetrs_key.txt", "test_delimetrs_out.txt");

        cout << "test_delimetrs     DONE" << endl;

    } catch(Exception &l) {
        cout << l << endl;
    }

    remove("test_delimetrs_out.txt");
}

void test_num() {
    try {
        Scanner S("lexical_stage_tests/test_num.txt");
        ofstream fout("test_num_out.txt");

        Lex l;
        while (1) {
            l = S.get_lex(); 
            if (l.get_type() != LEX_NULL)
                fout << l;
            else 
                break;
        }
        
        fout.close();
        
        files_equals("lexical_stage_tests/test_num_key.txt", "test_num_out.txt");

        cout << "test_num           DONE" << endl;

    } catch(Exception &l) {
        cout << l << endl;
    }

    remove("test_num_out.txt");
}

void test_all() {
    try {
        Scanner S("lexical_stage_tests/test_all.txt");
        ofstream fout("test_all_out.txt");

        Lex l;
        while (1) {
            l = S.get_lex(); 
            if (l.get_type() != LEX_NULL)
                fout << l;
            else 
                break;
        }
        
        fout.close();
        
        files_equals("lexical_stage_tests/test_all_key.txt", "test_all_out.txt");

        cout << "test_all           DONE" << endl;

    } catch(Exception &l) {
        cout << l << endl;
    }

    remove("test_all_out.txt");
}

int main() {
    
    test_keywords();
    test_delimetrs();
    test_num();
    test_all();

    return 0;
}
