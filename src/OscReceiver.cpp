/* 
 * File:   OscReceiver.cpp
 * Author: haikalpribadi
 * 
 * Created on 14 February 2014, 20:38
 */

#include "OscReceiver.h"

OscReceiver::OscReceiver() {
    if(node.getParam("osc_port", port)){
        ROS_INFO("OscReceiver using port: %d", port);
    }
    else {
        port = 7000;
    }
    oscMessageReceiver_pub = node.advertise<std_msgs::String>("osc_receive_message", 1000);
    oscVectorReceiver_pub = node.advertise<std_msgs::Int32MultiArray>("osc_receive_vector", 1000);
}

void OscReceiver::ProcessMessage(const osc::ReceivedMessage& message, const IpEndpointName& remoteEndpoint) {
    (void) remoteEndpoint;
    
    try {
        if(strcmp(message.AddressPattern(), MESSAGE) == 0) {
            osc::ReceivedMessageArgumentStream args = message.ArgumentStream();
            const char *str;
            args >> str >>osc::EndMessage;
            std_msgs::String msg;
            msg.data = str;
            oscMessageReceiver_pub.publish(msg);
        }
        else if(strcmp(message.AddressPattern(), VECTOR) == 0) {
            std_msgs::Int32MultiArray msg;
            osc::ReceivedMessageArgumentIterator iter = message.ArgumentsBegin();
            int size = iter->ComputeArrayItemCount();
            ++iter;
            for(int i=0; i<size; i++) {
                osc::int32 val = iter->AsInt32();
                msg.data.push_back(val);
                ++iter;
            }
            oscVectorReceiver_pub.publish(msg);
        }
        else {
            ROS_INFO("OSC of AddressPattern: %s received but unrecognized", message.AddressPattern());
        }
    } 
    catch(osc::Exception& e) {
        ROS_ERROR("OscReceiver error: %s - %s", message.AddressPattern(), e.what());
    }
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "OscReceiver");
    
    OscReceiver receiver;
    UdpListeningReceiveSocket socket(IpEndpointName(IpEndpointName::ANY_ADDRESS, receiver.port), &receiver);
    
    socket.RunUntilSigInt();
    
    return 0;
}
