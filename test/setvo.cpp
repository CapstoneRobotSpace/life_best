#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>
#define PIN1 32
#define PIN2 33
using namespace std;

int main(int argc,char**argv){
  if(wiringPiSetupPhys() == -1)
    return 0;
  pinMode(PIN1,PWM_OUTPUT);
  pinMode(PIN2,PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  pwmSetClock(384);
  pwmSetRange(1000);
  cout<<"pre pare ..."<<endl;
  while(1){
    cout<<"50"<<endl;
    pwmWrite(PIN1,50);
    pwmWrite(PIN2,50);
    sleep(5);
    cout<<"75"<<endl;
    pwmWrite(PIN1,75);
    pwmWrite(PIN2,75);
    sleep(5);
    cout<<"100"<<endl;
    pwmWrite(PIN1,100);
    pwmWrite(PIN2,100);
    sleep(5);
    cout<<"75"<<endl;
    pwmWrite(PIN1,75);
    pwmWrite(PIN2,75);
    sleep(5);
  }
  return 0;
}
