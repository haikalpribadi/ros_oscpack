/* 
 * File:   test.cpp
 * Author: haikalpribadi
 *
 * Created on 15 February 2014, 17:14
 */

#include <cstdlib>
#include <vector>
#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int32MultiArray.h>

using namespace std;

ros::Publisher pub;
ros::Subscriber sub;

void testCallback(const std_msgs::Empty::ConstPtr msg) {
    ROS_INFO("Running OSC test");
    std_msgs::Int32MultiArray vector;
    vector.data.push_back(1);
    vector.data.push_back(2);
    vector.data.push_back(3);
    vector.data.push_back(4);
    
    pub.publish(vector);
}
/*
 * 
 */
int main(int argc, char** argv) {
    ros::init(argc, argv, "OSCTest");
    ros::NodeHandle node;
    pub = node.advertise<std_msgs::Int32MultiArray>("osc_send_vector", 1000);
    sub = node.subscribe("osc_test", 1000, testCallback);
    
    ros::spin();
    return 0;
}

