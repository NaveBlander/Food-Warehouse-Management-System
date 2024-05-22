all: clean compile link run

compile: bin/Action.o bin/Customer.o bin/Order.o bin/Parser.o bin/Volunteer.o bin/WareHouse.o bin/main.o

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Action.cpp -o bin/Action.o

bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Customer.cpp -o bin/Customer.o

bin/Order.o: src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Order.cpp -o bin/Order.o

bin/Parser.o: src/Parser.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Parser.cpp -o bin/Parser.o

bin/Volunteer.o: src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Volunteer.cpp -o bin/Volunteer.o

bin/WareHouse.o: src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/WareHouse.cpp -o bin/WareHouse.o

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/main.cpp -o bin/main.o

link: bin/warehouse

bin/warehouse: bin/Action.o bin/Customer.o bin/Order.o bin/Parser.o bin/Volunteer.o bin/WareHouse.o bin/main.o
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/warehouse bin/*.o

clean:
	rm -f bin/warehouse bin/*.o

run: 
	./bin/warehouse bin/rest/configFileExample.txt
