//
// Created by yishuifengxing on 2023/6/1.
//
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>
#include "string"

class LaserToPointPath
{
public:
    LaserToPointPath() : tf_listener_(ros::Duration(10.0))
    {
        // 创建ROS订阅器，订阅名为“/scan”的激光雷达数据
        laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 1000, &LaserToPointPath::laserCallback, this);
        // 创建ROS发布器，发布名为“/scan_points”的路径数据
        path_pub_ = nh_.advertise<nav_msgs::Path>("/scan_points",10);
    }

    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan_msg)
    {

        path_msg_.poses.clear();

        // Loop through the laser scan data and add points to the path
        for (int i = 0; i < scan_msg->ranges.size(); ++i) {
            // Check if the range is valid
            if (scan_msg->ranges[i] > scan_msg->range_min && scan_msg->ranges[i] < scan_msg->range_max) {
                // Convert the polar coordinates to Cartesian coordinates
                double x = scan_msg->ranges[i] * cos(scan_msg->angle_min + i * scan_msg->angle_increment);
                double y = scan_msg->ranges[i] * sin(scan_msg->angle_min + i * scan_msg->angle_increment);

                // Create a new pose and add it to the path
                geometry_msgs::PoseStamped pose;
                pose.header.stamp = scan_msg->header.stamp;
                pose.header.frame_id = path_msg_.header.frame_id;
                pose.pose.position.x = x;
                pose.pose.position.y = y;
                pose.pose.orientation.w = 1.0;
                path_msg_.poses.push_back(pose);
            }
        }
        // 发布点云数据
        path_pub_.publish(path_msg_);
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber laser_sub_;
    ros::Publisher path_pub_;
    nav_msgs::Path path_msg_;
    tf::TransformListener tf_listener_;
};

int main(int argc, char** argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "laser_to_point_publisher");
    // 创建LaserToPointPath对象
    LaserToPointPath laser_to_point_cloud;
    // 循环等待回调函数
    ros::spin();
    return 0;
}