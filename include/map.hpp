//
//  map.hpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/16.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#include <vector>
#include <octomap/octomap.h>

class Map{
public:
    Map(float resolution, octomap::point3d bbx_max, octomap::point3d bbx_min);
    Map(std::string file_name, octomap::point3d bbx_max, octomap::point3d bbx_min);
    Map(octomap::OcTree* tree);
    ~Map();
    bool isObstacle(octomap::point3d point);
    double getResolution(){ return map_tree_->getResolution();}
    octomap::point3d getBBXMax(){ return map_tree_->getBBXMax(); }
    octomap::point3d getBBXMin(){ return map_tree_->getBBXMin(); }
    void mixPathMap(octomap::point3d point, bool is_occupied);
    void writeFile(std::string output_name);
private:
    octomap::OcTree* map_tree_;
};
#endif /* map_hpp */
