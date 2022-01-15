testing: test clean
test: database.o unit_testing.o
	g++ database.o unit_testing.o -o main

database.o: src/database.cpp
	g++ -c src/database.cpp -o database.o

unit_testing.o: src/test/unit_testing.cpp
	g++ -c src/test/unit_testing.cpp -o unit_testing.o

.PHONY: clean
clean:
	rm -f *.o
