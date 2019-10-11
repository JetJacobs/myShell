default: src/main/main.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	g++ -o myshell.o src/main/main.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp

test: src/test/test.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	g++ -o test.o src/test/test.cpp src/main/executor.cpp src/main/include/executor.hpp src/main/cmdParser.cpp src/main/include/cmdParser.hpp
	./test.o
run:
	./myshell.o
clean:
	rm -rvf *.o