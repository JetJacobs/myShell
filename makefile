default: main.cpp
	g++ -o myshell.o main.cpp

clean:
	rm -rvf *.o