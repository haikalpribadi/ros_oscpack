/* 
 * File:   OscSender.cpp
 * Author: haikalpribadi
 * 
 * Created on 14 February 2014, 20:08
 */

#include "OscSender.h"

#define ADDRESS "127.0.0.1"
#define PORT    7000

OscSender::OscSender() : socket(IpEndpointName(ADDRESS, PORT)) {
    if(node.getParam("osc_address", address)) {
        ROS_INFO("OscSender using address: %s", address.c_str());
    }
    else {
        address = "127.0.0.1";
        ROS_INFO("OscSender using address: 127.0.0.1");
    }
    
    if(node.getParam("osc_port", port)) {
        ROS_INFO("OscSender using port: %d", port);
    }
    else {
        port = 7000;
    }
    
    if(node.getParam("osc_offset", offset)) {
        ROS_INFO("OscSender sending values from offset: %d", offset);
    }
    else {
        offset = 0;
    }
    
    new(&socket) UdpTransmitSocket(IpEndpointName(address.c_str(), port));
    
    output_buffer_size = 1024;
    oscMessageSender_sub = node.subscribe("osc_send_message", 1000, &OscSender::oscMessageCallback, this);
    oscVectorSender_sub = node.subscribe("osc_send_vector", 1000, &OscSender::oscVectorCallback, this);
    
}

void OscSender::oscMessageCallback(const std_msgs::String::ConstPtr& msg) {
    char buffer[output_buffer_size];
    osc::OutboundPacketStream packet(buffer, output_buffer_size);
    
    packet << osc::BeginBundleImmediate << osc::BeginMessage( MESSAGE )
           << msg->data.c_str() 
           << osc::EndMessage << osc::EndBundle;
    
    socket.Send(packet.Data(), packet.Size());
}

void OscSender::oscVectorCallback(const std_msgs::Int32MultiArray::ConstPtr& msg) {
    char buffer[output_buffer_size];
    osc::OutboundPacketStream packet(buffer, output_buffer_size);
    stringstream ss;
    
    /* OLD OSC BUNDLE: SENDING AN INTEGER OF ARRAY AT ONCE
    packet << osc::BeginBundleImmediate << osc::BeginMessage(VECTOR) << osc::BeginArray;
    for(int i=0; i<msg->data.size(); i++) {
        packet << msg->data[i];
    }
    packet << osc::EndArray << osc::EndMessage << osc::EndBundle;
     */
    
    
    packet << osc::BeginBundleImmediate;
    for(int i=0; i<msg->data.size(); i++) {
        packet << osc::BeginMessage(ss.str().c_str()) << (msg->data[i] + offset) << osc::EndMessage;
    }
    packet << osc::EndBundle;
    
    socket.Send(packet.Data(), packet.Size());
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "OscSender");
    
    OscSender oscSender;
    
    ros::spin();
    
    return 0;
}