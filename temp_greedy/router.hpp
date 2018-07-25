/*
 * router.hpp
 *
 *  Created on: 2018-7-21
 *      Author: joey
 */

#ifndef ROUTER_HPP_
#define ROUTER_HPP_


#include "map.hpp"

class Router{
public:
	Router(Map* map, point3d start, point3d end, int max_step, float step_size);
	~Router();

	/**
	 * find right place to step, write into pathNodes
	 *
	 * @par: detect direction
	 * @return: if find the end position
	 * @references: writePathNodes()
	 */
	bool stepDetect(point3d direction);

	/**
	 * the view of sphere with step_size as r, position as center
	 * update this partly map into detectMap
	 *
	 * @par: point to be write
	 */
	void writeDetectMap(point3d position);

	/* write path into Detect Map */
	void writePathNodes();

	/**
	 * test the path from cur position
	 *
	 * @par: direction, size of current step
	 * @return:
	 */
	bool isPathObstacle(point3d direction, float cur_size);

	/**
	 * carry function
	 * distance between two points
	 */
	float calculateDis(point3d pos1, point3d pos2);

	void run();

private:
	/* initial */
	Map* map;
	point3d start;
	point3d end;
	int max_step;
	float step_size;

	point3d cur_position;

	/*
	 * points in the path
	 * chose only the actual fly points
	 */
	vector<point3d> path_nodes;

	/*
	 * write into map within sphere view
	 * when detect obstacle
	 */
	ColorOcTree* detect_map;
};


#endif /* ROUTER_HPP_ */
