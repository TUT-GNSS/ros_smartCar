#include<iostream>
#include<ros/ros.h>
#include<std_msgs/String.h>
// #include <unistd.h>

int main(int argc,char *argv[]){
    ros::init(argc,argv,"pub2_node");
    printf("Hello test2!\n");

    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("test2",10);

    ros::Rate loopRate(1);


    while(ros::ok()){

        printf("hhh!\n");
        std_msgs::String msg;
        msg.data = "message2,test2";
        pub.publish(msg);
        // sleep(1);
        loopRate.sleep();
    }
    return 0;
}