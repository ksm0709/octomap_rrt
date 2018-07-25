/*
 * map.cpp
 *
 *  Created on: 2018-7-21
 *      Author: joey
 */
#include "map.hpp"

Map::Map(float resolution)
{
	// write a test octomap
    map_tree = new OcTree("hello.bt");
//    map_tree = new OcTree(resolution);
//
//	int x, y, z;
//    for(x = 390; x <= 410; x++)
//		for(y = 390; y <= 410; y++)
//			for(z = 390; z <= 410; z++){
//				point3d ocurPoint ((float)x*0.05f, (float)y*0.05f, (float)z*0.05f);
//				map_tree->updateNode(ocurPoint, true);
//			}
//
//    for(x = 220; x <= 240; x++)
//    	for(y = 220; y <= 240; y++)
//    		for(z = 220; z <= 240; z++){
//    			point3d ocurPoint ((float)x*0.05f, (float)y*0.05f, (float)z*0.05f);
//    			map_tree->updateNode(ocurPoint, true);
//    		}


	map_tree->updateInnerOccupancy();
	map_tree->writeBinary("test.bt");
	cout << "test map done." << endl;

    point3d bbx_max = point3d(40,40,40);
    point3d bbx_min = point3d(0,0,0);
    map_tree->setBBXMax(bbx_max);
    map_tree->setBBXMin(bbx_min);
}

Map::~Map()
{
    delete map_tree;
}

bool Map::isObstacle(point3d point)
{
    OcTreeNode* node = map_tree->search(point);
    if(node!=NULL && node->getOccupancy()>0.5){
        return true;
    }
    return false;
}

double Map::getResolution()
{
	return map_tree->getResolution();
}

OcTree* Map::getOcTree(){
	return map_tree;
}


