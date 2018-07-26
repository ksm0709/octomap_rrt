//
//  map.cpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/16.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#include "../include/map.hpp"

Map::Map(float resolution, octomap::point3d bbx_max, octomap::point3d bbx_min)
{
    map_tree_ = new octomap::OcTree(resolution);
    std::vector<octomap::point3d> points = 
        {
            octomap::point3d(0,0,0), 
            octomap::point3d(1,1,1), 
            octomap::point3d(0.5,0,0), 
            octomap::point3d(1,0,0)
        };
    for(auto p:points){
        map_tree_->updateNode(p, true);
    }
    map_tree_->setBBXMax(bbx_max);
    map_tree_->setBBXMin(bbx_min);
//    map_tree_->writeBinary("./map.bt");
}
Map::Map(std::string file_name, octomap::point3d bbx_max, octomap::point3d bbx_min)
{
    map_tree_ = new octomap::OcTree(file_name);
    map_tree_->setBBXMax(bbx_max);
    map_tree_->setBBXMin(bbx_min);
}
Map::Map(octomap::OcTree* tree)
{
    map_tree_ = tree;
}
Map::~Map()
{
    if(map_tree_ != NULL)
        delete map_tree_;
}
bool Map::isObstacle(octomap::point3d point)
{
    //to do
    octomap::OcTreeNode* node = map_tree_->search(point);
    if(node!=NULL && node->getOccupancy()>0.5){
        return true;
    }
    return false;
}
void Map::mixPathMap(octomap::point3d point, bool is_occupied)
{
    map_tree_->updateNode(point, is_occupied);
}
void Map::writeFile(std::string output_name)
{
    map_tree_->writeBinary(output_name);
}
