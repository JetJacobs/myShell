default: src/main/main.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	g++ -o myshell.o src/main/main.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp

test: src/test/execTest.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	make pipeTest
	make execTest

execTest: src/test/execTest.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	g++ -o execTest.o src/test/execTest.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	./execTest.o

pipeTest: src/test/pipeTest.cpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	g++ -o pipeTest.o src/test/pipeTest.cpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	./pipeTest.o

run:
	./myshell.o
clean:
	rm -rvf *.o