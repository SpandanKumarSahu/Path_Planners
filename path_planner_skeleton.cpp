#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include <apriltags_ros/AprilTagDetection.h>
#include <apriltags_ros/AprilTagDetectionArray.h>
#include <bits/stdc++.h>

static const int ROWS = 600;
static const int COLS = 800;
static const int BOT_ID = 0;

struct Bot{
  int id;
  int x;
  int y;

  Bot(int i, int j, int k){
    id = i;
    x = j;
    y = k;
  }
}

geometry_msgs::PoseArray res;
std::vector<Bot> activeBots;

void callback(const AprilTagDetectionArray& msg){
  int i = 0;
  activeBots.clear();
  for(std::vector<AprilTags::AprilTagDetection>::const_iterator it = msg->detections.begin(); it != msg->detections.end(); ++it){
    AprilTags::AprilTagDetection temp = *it;
    activeBots.push_back(Bot( temp.id, (int) temp.pose.pose.position.x, (int) temp.pose.pose.position.y));
    ++i;
  }
}

void path_planner(){
  
}

void inserWayPoint(int x, int y){
  geometrt_msgs::Pose pose;
  pose.pose.position.x = x;
  pose.pose.position.y = y;
  res.poses.push_back(pose.pose);
}

int main(){
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("tag_detections", 10, callback);
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::PoseArray>("wayPoints", 1000);
  ros::Rate loop_rate(10);

  while(ros::ok()){
    res.poses.clear();
    res.header.stamp = ros::Time::now();
    res.header.fram_id = "/map";
    
    path_planner();
    chatter_pub.publish(res);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
  
