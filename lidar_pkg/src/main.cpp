#include "serial.cpp"
#include <iostream>
#include <cstring>
#include "life_msgs/distance.h"
#include "ros/ros.h"

using namespace std;
int main(int argc,char **argv){
  ros::init(argc,argv,"LIDAR");
	ros::NodeHandle nh;
  ros::Publisher dist_pub = nh.advertise<life_msgs::distance>("/distance", 1);
  life_msgs::distance dist_msg;

	Serial LRR("/dev/Lidar",115200);
	if(!LRR.good){
		cout<<"Sensor ERROR"<<endl;
		return 0;
	}
	char buf[9] {};
	int buf_index = 0;
	memset(buf,'\0',9);
  ROS_INFO("START NODE...");
	while(ros::ok()){
		if(buf_index < 2){
			LRR.sread(buf+buf_index,1);
			if(*(buf+buf_index) == 0x59)
				buf_index ++;
			else
				buf_index = 0;
		}
		else{
			LRR.sread(buf+buf_index,1);
			buf_index++;
			if(buf_index == 9){
				char check = 0;
				for(int i=0;i<8;i++)
					check += buf[i];
				if(check == buf[8]){
					unsigned int temp = (0xffff & buf[3])<<8;
					temp += buf[2];
					if(temp < 180000)
						dist_msg.dist = temp;
	        dist_pub.publish(dist_msg);
				}
				buf_index = 0;
			}
		}
	}
}
