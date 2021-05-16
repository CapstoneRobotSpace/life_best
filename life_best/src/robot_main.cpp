#include "ros/ros.h"
#include <iostream>
#include "life_msgs/motor.h"
#include "life_msgs/distance.h"
#include "sensor_msgs/Imu.h"
#include <Eigen/Dense>
#include <queue>

using namespace std;
using namespace Eigen;

int dist = -1;

struct Position{
  int x = 0;
  int y = 0;
  int z = 0;
}pos;


struct Rotation{
  double r = 0;
  double p = 0;
  double y = 0;
  ros::Time time;
  Rotation(double r = 0,double p = 0,double y = 0,ros::Time time = ros::Time::now()) : r(r),p(p),y(y),time(time){};
}rot;

queue<Rotation> rot_q;

void DistFun(const life_msgs::distance& msg){
  dist = msg.dist;
};

void ImuFun(const sensor_msgs::Imu& msg){
  static int cnt = 0;
  rot.r = msg.orientation.x;
  rot.p = msg.orientation.y;
  rot.y = msg.orientation.z;
  rot.time = msg.header.stamp;
  cnt++;
  if(cnt == 100){
    rot_q.push(rot);
    cnt = 0;
  }
};

int main(int argc,char** argv){
  cout<<"Hello!"<<endl;
  ros::init(argc,argv,"ROBOT_CORE");
	ros::NodeHandle nh;
  ros::Publisher m_pub = nh.advertise<life_msgs::motor>("/motor", 1);
  ros::Subscriber dist_sub = nh.subscribe("/distance", 1 ,DistFun);
  ros::Subscriber imu_sub = nh.subscribe("/imu", 1 ,ImuFun);
  life_msgs::motor m_msg;
  Matrix4d roll,pitch,yaw,rotation;
  Vector4d target(15,15,0,0),target_pos;
  while(ros::ok()){
    //cout<<"dist : "<<dist<<endl;

    roll<< 1,0,0,0,
           0,cos(rot.r),-sin(rot.r),0,
           0,sin(rot.r),cos(rot.r),0,
           0,0,0,1;

    pitch<<cos(rot.p),0,sin(rot.p),0,
           0,1,0,0,
           -sin(rot.p),0,cos(rot.p),0, 
           0,0,0,1;

    yaw<<cos(rot.y),-sin(rot.y),0,0,
         sin(rot.y),cos(rot.y),0,0,
         0,0,1,0,
         0,0,0,1;

    rotation = yaw*pitch*roll;
    rotation(0,3) = pos.x;
    rotation(1,3) = pos.y;
    rotation(2,3) = pos.z;
    target_pos = rotation*target;
    cout<<target_pos<<endl;
    m_pub.publish(m_msg);
    ros::spinOnce();
  }
  return 0;
}
