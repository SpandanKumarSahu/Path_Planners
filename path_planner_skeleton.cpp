#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include <apriltags_ros/AprilTagDetection.h>
#include <apriltags_ros/AprilTagDetectionArray.h>
#include <bits/stdc++.h>

static const int ROWS = 600;
static const int COLS = 800;
static const std::string BOT_ID = getenv("BOT_ID");

struct Bot{
  int id;
  int x;
  int y;
  float theta;
}

geometry_msgs::Pose msg;
Bot activeBots[3];

void cb1(const geometry_msgs::Pose& _msg){
  activeBots[0].id = 1;
  activeBots[0].x = _msg->position.x;
  activeBots[0].y = _msg->position.y;
  activeBots[0].theta = _msg->orientation.z;
}
void cb2(const geometry_msgs::Pose& _msg){
  activeBots[1].id = 2;
  activeBots[1].x = _msg->position.x;
  activeBots[1].y = _msg->position.y;
  activeBots[1].theta = _msg->orientation.z;
}
void cb3(const geometry_msgs::Pose& _msg){
  activeBots[2].id = 3;
  activeBots[2].x = _msg->position.x;
  activeBots[2].y = _msg->position.y;
  activeBots[2].theta = _msg->orientation.z;
}
void path_planner(){


  /*
    You need to set the following, at the least :
    msg.position.x
    msg.position.y

    The pose value can be found in the activeBots.
   */
}

int main(){
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  std::string bot1 = "/bot1/pose";
  std::string bot2 = "/bot2/pose";
  std::string bot3 = "/bot3/pose";
  
  ros::Subscriber sub_bot1 = n.subscribe(bot1, 10, cb1);
  ros::Subscriber sub_bot2 = n.subscribe(bot2, 10, cb2);
  ros::Subscriber sub_bot3 = n.subscribe(bot3, 10, cb3);
  
  ros::Publisher nextPoint = n.advertise<geometry_msgs::Pose>("nextPoint", 1000);
  ros::Rate loop_rate(10);
  while(ros::ok()){
    path_planner();
    chatter_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
  
