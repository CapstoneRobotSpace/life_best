#include "ros/ros.h"
#include <iostream>
#include "life_msgs/motor.h"
#include "life_msgs/distance.h"

using namespace std;

int dist = 0;

void dist_fun(const life_msgs::distance& msg){
  dist = msg.dist;
};

int main(int argc,char** argv){
  cout<<"Hello!"<<endl;
  ros::init(argc,argv,"ROBOT_CORE");
	ros::NodeHandle nh;
  ros::Publisher m_pub = nh.advertise<life_msgs::motor>("/motor", 1);
  ros::Subscriber dist_sub = nh.subscribe("/distance", 1 ,dist_fun);
  life_msgs::motor m_msg;
  while(ros::ok()){
    cout<<"dist : "<<dist<<endl;
    m_pub.publish(m_msg);
    ros::spinOnce();
  }
  return 0;
}
