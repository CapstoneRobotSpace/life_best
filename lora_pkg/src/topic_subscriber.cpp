#include "ros/ros.h"
#include <iostream>
#include "sensor_msgs/NavSatFix.h"
#include "serial.hpp"
#include <string>
#include "std_msgs/Int32.h"


string send_0 = "";
string state_str = "";
string latitude_str = "";
string longitude_str = "";
string time_str = "";
int state =0;
bool is_get = false;



void msgCallback(const sensor_msgs::NavSatFix::ConstPtr& gps_msg)
{
	latitude_str = to_string(gps_msg->latitude);
	longitude_str = to_string(gps_msg->longitude);
	state_str = to_string(state);
}
void msgCallback2(const std_msgs::Int32::ConstPtr& gps_msg2){
	time_str = to_string(gps_msg2->data);
	send_0 = "FF0," + state_str+ "," + latitude_str + "," + longitude_str + "," + time_str;
  is_get = true;
	
}

void msgCallback3(const std_msgs::Int32::ConstPtr& msg){
  state = msg->data;	
}

int main(int argc,char **argv){
    using namespace std; 
	ros::init(argc, argv, "LORA_NODE");
	ros::NodeHandle nh; 
	ros::Subscriber sub2= nh.subscribe("/gps2",1,msgCallback2);
	ros::Subscriber sub= nh.subscribe("/gps",1,msgCallback);
  ros::Subscriber sub3= nh.subscribe("/state",1,msgCallback3);
  string param;
  if(nh.getParam("/LORA_NODE/port",param))
    cout<<"port : "<<param<<endl;
  Serial lora(param.c_str(),9600);
  while(ros::ok()){
    ros::spinOnce();
    if(is_get){
      if(lora.swrite(send_0.c_str(), send_0.size()+1)>0){
      	cout<<"I send :"<<send_0.c_str()<<endl;
      }
      is_get = false;
    }
  }   
	return 0;
} 

