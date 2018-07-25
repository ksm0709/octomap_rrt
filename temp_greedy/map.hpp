/*
 * map.hpp
 *
 *  Created on: 2018-7-21
 *      Author: joey
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include <iostream>
#include <vector>
#include <octomap/octomap.h>
#include <octomap/ColorOcTree.h>

using namespace std;
using namespace octomap;
using namespace octomath;

class Map{
public:
    Map(float resolution);
    ~Map();

    bool isObstacle(point3d point);

    OcTree* getOcTree();

    double getResolution();
private:
    OcTree* map_tree;
};
#endif /* MAP_HPP_ */
