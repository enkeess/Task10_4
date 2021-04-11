all:
	
	g++ -w Executer.cpp -o ex
	g++ -w test_lexical_stage.cpp -o test_lexical_stage
	
	
clean:
	rm -f test_lexical_stage
	rm -f ex