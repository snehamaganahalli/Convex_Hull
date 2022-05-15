all:
	g++ convex_hull.cpp -std=c++11 -o exe

clean:
	/bin/rm -rf exe
