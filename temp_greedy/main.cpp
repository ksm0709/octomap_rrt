/*
 * main.cpp
 *
 *  Created on: 2018-7-21
 *      Author: joey
 */

#include "map.hpp"
#include "router.hpp"

int main(int argc, char** argv){
	Map* map = new Map(0.1);
	cout << "initial map" << endl;

	point3d start(0,0,0);
	point3d end(40,40,40);

	Router router(map, start, end, 1000, 5);
	router.run();

	return 0;
}

