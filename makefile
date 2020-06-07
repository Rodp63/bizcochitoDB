install:
	mkdir -p data/tables
	mkdir -p data/indexes
	mkdir -p metadata/indexes
	mkdir -p metadata/tables
	g++ -std=c++11 src/*cpp -o cake 
clean:
	rm -f src/*.cpp~
	rm -f include/*.h~
