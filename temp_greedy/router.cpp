/*
 * router.cpp
 *
 *  Created on: 2018-7-21
 *      Author: joey
 */

#include "router.hpp"

Router::Router(Map* map, point3d start, point3d end, int max_step, float step_size){
	this->map = map;
	cur_position = this->start = start;
	this->end = end;
	this->max_step = max_step;
	this->step_size = step_size;

	detect_map = new ColorOcTree(map->getResolution());

	/* first push the start point */
	path_nodes.push_back(start);
}

Router::~Router(){
	detect_map->clear();
	path_nodes.clear();
}

bool Router::stepDetect(point3d direction){
	/* first step */
	point3d max_view_pos;

	/* distance from the end along this direction */
	float dis = sqrt(pow(direction.x(), 2) + pow(direction.y(), 2) + pow(direction.z(), 2));
	float unit = step_size/dis;
	max_view_pos.x() = cur_position.x() + unit * direction.x();
	max_view_pos.y() = cur_position.y() + unit * direction.y();
	max_view_pos.z() = cur_position.z() + unit * direction.z();

	/* if obstacle is in the view */
	if(isPathObstacle(direction, step_size)){
		cout << "path obstacle" << endl;
		float cur_size = step_size/2;
		while(isPathObstacle(direction, cur_size)){
			cur_size /= 2;
		}

		/* fly, update cur position */
		cur_position.x() += cur_size/dis * direction.x();
		cur_position.y() += cur_size/dis * direction.y();
		cur_position.z() += cur_size/dis * direction.z();
		path_nodes.push_back(cur_position);

		/* change the direction */
		while(1){
			float x_rand = rand() / float(RAND_MAX) * 10;
			float y_rand = rand() / float(RAND_MAX) * 10;
			float z = (direction.x() * x_rand + direction.y() * y_rand) / direction.z();

			float unit = 1.5 * step_size / sqrt(pow(x_rand, 2) + pow(y_rand, 2) + pow(z, 2));
			point3d change_to_pos;
			change_to_pos.x() = cur_position.x() + unit * x_rand;
			change_to_pos.y() = cur_position.y() + unit * y_rand;
			change_to_pos.z() = cur_position.z() + unit * z;
			if(!isPathObstacle(change_to_pos - cur_position, step_size)){
				cur_position = change_to_pos;
				path_nodes.push_back(cur_position);
				break;
			}
		}
	}else{
		/* find the destination */
		if(dis < step_size){
			path_nodes.push_back(end);
			return true;
		}

		/* fly */
		cur_position = max_view_pos;
		path_nodes.push_back(cur_position);
	}
	return false;
}

bool Router::isPathObstacle(point3d direction, float cur_size){
	float dis = sqrt(pow(direction.x(), 2) + pow(direction.y(), 2) + pow(direction.z(), 2));
	float unit = cur_size/dis;
	int time = 0;

	while(1){
		/* par to consider */
		if(time > 4){
			return false;
		}
		point3d test_point, test_point_i;
		float pow_time = 1.0/pow(2.0, time);
		test_point.x() = cur_position.x() + unit * pow_time * direction.x();
		test_point.y() = cur_position.y() + unit * pow_time * direction.y();
		test_point.z() = cur_position.z() + unit * pow_time * direction.z();

		float pow_time_i = (pow(2.0, time)-1)/pow(2.0, time);
		test_point_i.x() = cur_position.x() + unit * pow_time_i * direction.x();
		test_point_i.y() = cur_position.y() + unit * pow_time_i * direction.y();
		test_point_i.z() = cur_position.z() + unit * pow_time_i * direction.z();
		if(map->isObstacle(test_point) || map->isObstacle(test_point_i)){
			return true;
		}
		time++;
	}
}

void Router::writeDetectMap(point3d position){
	float radius = step_size;
	OcTree::tree_iterator it, end;
	OcTree* view = map->getOcTree();

	for(it = view->begin_tree(), end = view->end_tree(); it != end; it++){
		point3d center = it.getCoordinate();
		/* change to whole view .bt file */
		if(calculateDis(position, center) < radius){
			detect_map->updateNode(center, true);
			detect_map->updateInnerOccupancy();
		}
	}
}

float Router::calculateDis(point3d pos1, point3d pos2)
{
	point3d direction = pos2 - pos1;
	float dis = sqrt(pow(direction.x(), 2) + pow(direction.y(), 2) + pow(direction.z(), 2));
	return dis;
}

void Router::writePathNodes()
{
	vector<point3d>::iterator it;
	for(it=path_nodes.begin();it!=path_nodes.end();it++){
		point3d p = *it;
		cout << p << endl;
		detect_map->updateNode(p, true);
		detect_map->integrateNodeColor(p.x(), p.y(), p.z(), 255, 0, 0);
		detect_map->updateInnerOccupancy();
	}
}

void Router::run(){
	int step_count = 1;
	for(step_count = 1; step_count <= max_step; step_count++){
		writeDetectMap(cur_position);
		cout << "step: " << step_count << endl;
		bool find = stepDetect(end - cur_position);
		if(find == true){
			cout << "find the end position" << endl;
			break;
		}
	}

	/* obstacle has been updated into detect map */
	writePathNodes();

	detect_map->write("detect.ot");
}




