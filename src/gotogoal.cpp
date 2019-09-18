#include "gotogoal.hpp"
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <iostream>
#include <string>

using namespace std;

Go::Go(ros::NodeHandle& node, double tolerance): _Node(node), _accep(tolerance){

    _subPosition = _Node.subscribe("/turtle1/pose",1, &Go::subPoseCallback, this);
    _pubvel = _Node.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
    _posegoal[0] = 5.544444561;
    _posegoal[1] = 5.544444561;
    _x = 0;
    _y = 0;
    
}

void Go::subPoseCallback(const turtlesim::Pose::ConstPtr& msg) {

    _pose[0] = msg->x;
    _pose[1] = msg->y;
    _pose[2] = msg->theta;
}
void Go::getcoords() {
    cout << "digite a coordenada x do goal";
    cin >> _x;
    _posegoal[0] = _x;
    cout << "digite a coordenada y do goal";
    cin >> _y;
    _posegoal[1] = _y;  
    ROS_INFO("%f %f", _posegoal[0], _posegoal[1]);
}   

void Go::gotogoal(){
    geometry_msgs::Twist vel;
    double linearVelocity;
    double angle =  atan2(_posegoal[1] - _pose[1], _posegoal[0] - _pose[0]);
    double angle_error = angle - _pose[2];
    double target_distance = sqrt(pow(_posegoal[0] - _pose[0],2) + pow(_posegoal[1] - _pose[1],2));
    ROS_INFO("Erro: %f", target_distance);
    if(angle_error > pi){
        angle_error -= 2*(pi);
        }
    else if (angle_error < -pi){
        angle_error += 2*(pi);
        }
    vel.angular.z = angle_error*.5;
    if (vel.angular.z > .5){
        vel.angular.z = .5;
    }
    else if (vel.angular.z < -.5){
        vel.angular.z = -.5;
    }
    if (target_distance > _accep){
        vel.linear.x = fabs(target_distance)*.5;
        if (vel.linear.x > 1){
            vel.linear.x = 1;
            }
    }
    else{
        vel.linear.x = vel.angular.z = 0;
    }
    
    _pubvel.publish(vel);
}

int main(int argc, char* argv[]){
    ros::init(argc, argv, "turtle_sim_goto");
    ros::NodeHandle node;
    Go  turtle_sim_go(node, 0.1);
    turtle_sim_go.getcoords();
    while (ros::ok())
    {
        turtle_sim_go.gotogoal();
        ros::spinOnce();
    }
    ROS_WARN("done");
}