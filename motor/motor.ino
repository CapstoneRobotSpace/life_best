#include <Servo.h>
#include <ros.h>
#include <life_msgs/motor.h>

ros::NodeHandle  nh;
Servo left,right;


void messageCb( const life_msgs::motor& msg){
  left.write(msg.left);
  right.write(msg.right);
}

ros::Subscriber<life_msgs::motor> sub("motor", &messageCb );
void setup() {
  left.attach(5);
  right.attach(6);
  left.write(90);
  right.write(90);
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
}
