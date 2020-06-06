install:
	g++ -std=c++11 src/*cpp -o cake 
clean:
	rm -f src/*.cpp~
	rm -f include/*.h~
