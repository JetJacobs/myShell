default: main.cpp
	g++ -o myshell main.cpp

clean:
	rm -rvf myshell *.o