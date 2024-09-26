#include<ros/ros.h>
#include<std_msgs/String.h>

void test1_callback(std_msgs::String msg){//回调函数
    ROS_INFO(msg.data.data());//输出信息且显示时间戳
}
void test2_callback(std_msgs::String msg)
{                              //回调函数
    ROS_WARN(msg.data.data()); //输出且显示时间戳，为黄色警告
}

int main(int argc, char *argv[])
{
    //定义中文环境
    setlocale(LC_ALL, "");//从当前系统默认Locale读取，英文版“zh_CN.UTF-8”

    ros::init(argc, argv, "sub1_node");

    ros::NodeHandle nh;//节点处理类
    ros::Subscriber sub = nh.subscribe("test1", 10, test1_callback); // subscribe参数：接收的话题topic，缓冲区大小，回调函数名称

    ros::Subscriber sub2 = nh.subscribe("test2", 10, test2_callback); // subscribe参数：接收的话题topic，缓冲区大小，回调函数名称

    while(ros::ok()){
        ros::spinOnce();//轮询一次（出while循环瞅一眼）
    }
    return 0;
}
