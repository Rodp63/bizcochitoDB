all:
	g++ -std=c++11 src/*cpp -o cake
install:
	mkdir -p data/tables
	mkdir -p data/indexes
	mkdir -p metadata/tables
	g++ -std=c++11 src/*cpp -o cake 
clean:
	rm -f src/*.cpp~
	rm -f include/*.h~
reset:
	rm -f metadata/*.gaa
	rm -f metadata/tables/*.gaa
	rm -f data/tables/*.aea
