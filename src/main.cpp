//
//  main.cpp
//  OctomapRTT
//
//  Created by Daxin Wang on 2018/7/15.
//  Copyright © 2018 Daxin Wang. All rights reserved.
//
#include <iostream>
#include <assert.h>

//pcl
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include "../include/octomap_rrt.hpp"
#include "../include/map.hpp"
#include <octomap/octomap.h>
#include <octomap/ColorOcTree.h>

int main(int argc, char** argv) {

    if (argc != 3)
    {
        cout<<"Usage: pcd2octomap <input_file> <output_file>"<<endl;
        return -1;
    }

    string input_file = argv[1], output_file = argv[2];
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::io::loadPCDFile<pcl::PointXYZ> ( input_file, cloud );

    cout<<"point cloud loaded, piont size = "<<cloud.points.size()<<endl;
    
    //声明octomap变量
    cout<<"copy data into octomap..."<<endl;
    
    //define the bounding box
    octomap::point3d max;
    octomap::point3d min;
    max.x()=3;
    max.y()=3;
    max.z()=3;
    min.x()=-3;
    min.y()=-3;
    min.z()=-3;

    double resolution=0.005;

    // 创建八叉树对象，参数为分辨率，这里设成了0.05
    octomap::OcTree tree(resolution);

    tree.setBBXMax(max);
    tree.setBBXMin(min);

    for (auto p:cloud.points)
    {
        // 将点云里的点插入到octomap中
        //if(p.x<max.x()&&p.y<max.y()&&p.z<max.z()){
        octomap::point3d temp(p.x,p.y,p.z);
        if(tree.inBBX(temp)){
            tree.updateNode( octomap::point3d(p.x, p.y, p.z),true);
            //cout<<"("<<p.x<<","<<p.y<<","<<p.z<<")"<<endl;
        }

        
        //}
    }
  
    // 更新octomap
    tree.updateInnerOccupancy();
    // 存储octomap
    tree.writeBinary( output_file );
    cout<<"done."<<endl;

    Map* map = new Map("./pcd_output/guidance_0.bt");
    octomap::point3d start_position(0,0,0);
    octomap::point3d end_position(1,1,1);
    RRT3D rrt(start_position, end_position, map, 10000, 2);
    rrt.run();
    rrt.writeMap();
    map->writeFile("./path_octotree.bt");
}
