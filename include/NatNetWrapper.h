#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>

#ifdef _WIN32
#   include <conio.h>
#else
#   include <unistd.h>
#   include <termios.h>
#endif

#include <vector>

#include <NatNetTypes.h>
#include <NatNetCAPI.h>
#include <NatNetClient.h>
#include <LinearKalmanFilter.h>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

class NatNetWrapper {
public:
    NatNetWrapper();
    int run();

    void NATNET_CALLCONV ServerDiscoveredCallback( const sNatNetDiscoveredServer* pDiscoveredServer, void* pUserContext );
    void NATNET_CALLCONV DataHandler(sFrameOfMocapData* data, void* pUserData);    // receives data from the server
    void NATNET_CALLCONV MessageHandler(Verbosity msgType, const char* msg);      // receives NatNet error messages

private:
    ros::NodeHandle nh;
    std::vector<ros::Publisher> pubs_vision_pose;
    std::vector<ros::Publisher> pubs_vision_odom;
    std::vector<ros::Publisher> pubs_labeled_marker_pose_array;
    ros::Publisher pub_unlabeled_marker_pose_array;
    std::vector<std::unique_ptr<LinearKalmanFilter>> linearKalmanFilters;
    std::vector<int> model_ids;
    std::vector<std::string> model_names;
    std::string prefix;
    std::string frame_id;
    bool show_latency;
    int verbose_level;
    bool publish_with_twist;
    bool publish_labeled_marker_pose_array;
    bool publish_unlabeled_marker_pose_array;
    bool is_ServerDiscovered;

    void resetClient();
    int ConnectClient();

    static const ConnectionType kDefaultConnectionType = ConnectionType_Multicast;

    char getch();
    NatNetClient* g_pClient = NULL;

    std::vector< sNatNetDiscoveredServer > g_discoveredServers;
    sNatNetClientConnectParams g_connectParams;
    char g_discoveredMulticastGroupAddr[kNatNetIpv4AddrStrLenMax] = NATNET_DEFAULT_MULTICAST_ADDRESS;
    int g_analogSamplesPerMocapFrame = 0;
    sServerDescription g_serverDescription;
    
    tf2_ros::TransformBroadcaster br_;
};

