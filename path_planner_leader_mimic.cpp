/*
  Dynamic leader mimicing.
  For this, you need to publish the motion of the leader under the topic 
  `\bot1\pose_leader`
*/


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
bool initialised[3];
bool check = true;
int diff_x;
int diff_y;

void cb1(const geometry_msgs::Pose& _msg){
  initialised[0] = true;
  activeBots[0].id = 1;
  diff_x = _msg->position.x - activeBots[0].x;
  diff_y = _msg->position.y - activeBots[0].y;
  activeBots[0].x = _msg->position.x;
  activeBots[0].y = _msg->position.y;
  activeBots[0].theta = _msg->orientation.z;
}
void cb2(const geometry_msgs::Pose& _msg){
  initialised[1] = true;
  activeBots[1].id = 2;
  activeBots[1].x = _msg->position.x;
  activeBots[1].y = _msg->position.y;
  activeBots[1].theta = _msg->orientation.z;
}
void cb3(const geometry_msgs::Pose& _msg){
  initialised[2] = true;
  activeBots[2].id = 3;
  activeBots[2].x = _msg->position.x;
  activeBots[2].y = _msg->position.y;
  activeBots[2].theta = _msg->orientation.z;
}
void path_planner(){
  check = true;
  for(int i = 0; i < 3; i++)
    check = check && initialised[i];

  if(check){
    msg.position.x = activeBots[std::stoi(BOT_ID)-1].x;
    msg.position.y = activeBots[std::stoi(BOT_ID)-1].y;
    msg.position.x += diff_x;
    msg.position.y += diff_y;
  }
}

int main(){
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  std::string bot1 = "/bot1/pose_leader";
  std::string bot2 = "/bot2/pose";
  std::string bot3 = "/bot3/pose";
  
  ros::Subscriber sub_bot1 = n.subscribe(bot1, 10, cb1);
  ros::Subscriber sub_bot2 = n.subscribe(bot2, 10, cb2);
  ros::Subscriber sub_bot3 = n.subscribe(bot3, 10, cb3);
  
  ros::Publisher nextPoint = n.advertise<geometry_msgs::Pose>("nextPoint", 1000);
  ros::Rate loop_rate(10);
  for(int i = 0; i < 3 ; i++)
    initialised[i] = false;
  while(ros::ok()){
    path_planner();
    if(check)
      chatter_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
  
