#include<iostream>
#include<ros/ros.h>
#include<std_msgs/String.h>
// #include <unistd.h>

int main(int argc,char *argv[]){
    ros::init(argc,argv,"pub1_node");
    printf("Hello test1!\n");


    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("test1",10);

    ros::Rate loopRate(1);


    while(ros::ok()){

        printf("hhh!\n");
        std_msgs::String msg;
        msg.data = "message1,test1";
        pub.publish(msg);
        // sleep(1);
        loopRate.sleep();
    }
    return 0;
}