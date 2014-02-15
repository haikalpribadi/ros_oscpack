/* 
 * File:   OscReceiver.h
 * Author: haikalpribadi
 *
 * Created on 14 February 2014, 20:38
 */

#ifndef OSCRECEIVER_H
#define	OSCRECEIVER_H

#include <cstdlib>
#include <iostream>
#include <ros/ros.h>
#include <sstream>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/String.h>
#include <vector>

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

using namespace std;

#define MESSAGE "/message"
#define VECTOR  "/vector"

class OscReceiver : public osc::OscPacketListener {
public:
    int port;
    
    OscReceiver();

private:
    ros::NodeHandle node;
    ros::Publisher oscMessageReceiver_pub;
    ros::Publisher oscVectorReceiver_pub;
    
    void ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& remoteEndpoint);

    
};

#endif	/* OSCRECEIVER_H */

