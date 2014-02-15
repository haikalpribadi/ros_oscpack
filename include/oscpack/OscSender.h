/* 
 * File:   OscSender.h
 * Author: haikalpribadi
 *
 * Created on 14 February 2014, 20:08
 */

#ifndef OSCSENDER_H
#define	OSCSENDER_H

#include <cstdlib>
#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/String.h>

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

using namespace std;

#define MESSAGE "/message"
#define VECTOR  "/vector"

class OscSender {
public:
    string address;
    int port;
    int output_buffer_size;
    
    OscSender();

private:
    ros::NodeHandle node;
    ros::Subscriber oscMessageSender_sub;
    ros::Subscriber oscVectorSender_sub;
    UdpTransmitSocket socket;
    
    void oscMessageCallback(const std_msgs::String::ConstPtr& msg);
    void oscVectorCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
};

#endif	/* OSCSENDER_H */

