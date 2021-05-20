#include "ros/ros.h"
#include <iostream>
#include "life_msgs/motor.h"
#include "life_msgs/distance.h"
#include "life_msgs/yolo.h"
#include "sensor_msgs/Imu.h"
#include "nav_msgs/Odometry.h"
#include <Eigen/Dense>
#include <queue>
#define ROBOT_WEIGHT 5.5


using namespace std;
using namespace Eigen;

int dist = -1;

struct Position{
  double x = 0;
  double y = 0;
  double z = 0;
  double abs_sum(){
    return abs(x) + abs(y) + abs(z);  
  };
}pos,force;


struct Rotation{
  double r = 0;
  double p = 0;
  double y = 0;
  ros::Time stamp;
}rot;


void DistFun(const life_msgs::distance& msg){
  dist = msg.dist;
};

queue<Rotation> rot_q;
Vector4d target_pos,view_target,unit;

void ImuFun(const sensor_msgs::Imu& msg){
  static int cnt = 0;
  rot.r = msg.orientation.x;
  rot.p = msg.orientation.y;
  rot.y = msg.orientation.z;
  rot.stamp = msg.header.stamp;
  cnt++;
  if(cnt == 10){
    rot_q.push(rot);
    cnt = 0;
  }
  Matrix4d roll,pitch,yaw,rotation;
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
  view_target = rotation.inverse()*target_pos;
  view_target(0) *= -1;
  view_target(2) *= -1;
  Vector4d x_temp(1,0,0,0),y_temp(0,1,0,0);
  force.x = ROBOT_WEIGHT*msg.linear_acceleration.x;
  force.y = ROBOT_WEIGHT*msg.linear_acceleration.y;
  force.z = ROBOT_WEIGHT*(msg.linear_acceleration.z+1);
  unit = rotation*x_temp + rotation*y_temp;
};

void CameraFun(const life_msgs::yolo& msg){
  Rotation pre = rot;
  if(rot_q.size()){
    while(msg.stamp >= rot_q.front().stamp){
      pre = rot_q.front();
      rot_q.pop();  
    }
  }
  Matrix4d roll,pitch,yaw,rotation;
  roll<< 1,0,0,0,
         0,cos(pre.r),-sin(pre.r),0,
         0,sin(pre.r),cos(pre.r),0,
         0,0,0,1;

  pitch<<cos(pre.p),0,sin(pre.p),0,
         0,1,0,0,
         -sin(pre.p),0,cos(pre.p),0, 
         0,0,0,1;

  yaw<<cos(pre.y),-sin(pre.y),0,0,
       sin(pre.y),cos(pre.y),0,0,
       0,0,1,0,
       0,0,0,1;

  rotation = yaw*pitch*roll;
  rotation(0,3) = pos.x;
  rotation(1,3) = pos.y;
  rotation(2,3) = pos.z;
  double x_angle = (320 - msg.x)/320.0*85.0*3.1415/180.0;
  double y_angle = (240 - msg.y)/240.0*63.0*3.1415/180.0;
  Vector4d target(sin(x_angle)*dist,cos(x_angle)*dist,tan(y_angle)*dist,1);
  target_pos = rotation*target;
  
};

void OdomFun(const nav_msgs::Odometry& msg){
  pos.x = -msg.pose.pose.position.x;
  pos.y = msg.pose.pose.position.y;
  pos.z = -msg.pose.pose.position.z;
};
 

int main(int argc,char** argv){
  cout<<"Hello!"<<endl;
  ros::init(argc,argv,"ROBOT_CORE");
	ros::NodeHandle nh;
  ros::Publisher m_pub = nh.advertise<life_msgs::motor>("/motor", 1);
  ros::Subscriber dist_sub = nh.subscribe("/distance", 1 ,DistFun);
  ros::Subscriber imu_sub = nh.subscribe("/imu", 1 ,ImuFun);
  ros::Subscriber cam_sub = nh.subscribe("/cam", 1 ,CameraFun);
  ros::Subscriber odom_sub = nh.subscribe("/odom", 1 ,OdomFun);
  life_msgs::motor m_msg;
  int state = 0;
  while(ros::ok()){
    if(state == 0){
      cout<<force.abs_sum()<<endl;
      if(abs(unit(2)) > 0.4)
        cout<<"warn";
    }
    m_pub.publish(m_msg);
    ros::spinOnce();
  }
  return 0;
}
