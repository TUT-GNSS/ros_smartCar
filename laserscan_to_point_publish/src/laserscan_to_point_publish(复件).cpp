//
// Created by yishuifengxing on 2023/6/1.
//
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>

class LaserToPointPath
{
public:
    LaserToPointPath() : tf_listener_(ros::Duration(10.0))
    {
        // 创建ROS订阅器，订阅名为“/scan”的激光雷达数据
        laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 1000, &LaserToPointPath::laserCallback, this);
        // 创建ROS发布器，发布名为“/scan_points”的路径数据
        path_pub_ = nh_.advertise<nav_msgs::Path>("/scan_points",10);
        pointcloud_publisher = nh_.advertise<sensor_msgs::PointCloud2> ("/laserscan_to_pointcloud", 100);
    }

    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan_msg)
    {

        path_msg_.poses.clear();
        path_msg2_.poses.clear();
        sensor_msgs::LaserScan rotated_scan = *scan_msg;
        //std::reverse(rotated_scan.ranges.begin(), rotated_scan.ranges.end());
        //std::reverse(rotated_scan.intensities.begin(), rotated_scan.intensities.end());
        try {
             tf_listener_.waitForTransform("map", scan_data->header.frame_id, ros::Time(0), ros::Duration(1.0));
         // 获取TF变换
            tf::StampedTransform transform;
            tf_listener_.lookupTransform("map", scan_data->header.frame_id, ros::Time(0), transform);
         // 创建一个转换器
            tf::Transformer transformer;
         // 将激光数据转换到map坐标系
            sensor_msgs::PointCloud transformed_scan;
            transformer.transformPointCloud("map", *scan_data, transformed_scan);
            pointcloud_publisher.publish(transformed_scan);
             
        } catch (tf::TransformException& ex) {
            ROS_ERROR("Received an exception trying to transform a point from \"%s\" to \"map\": %s", scan_msg->header.frame_id.c_str(), ex.what());
        }

        


        

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
                pose.header.frame_id = scan_msg->header.frame_id;
                pose.pose.position.x = x;
                pose.pose.position.y = y;
                pose.pose.orientation.w = 1.0;
                path_msg_.poses.push_back(pose);
            }
        }

        int num = 20;
        int step = path_msg_.poses.size()/num;
        for(int j = 0; j < path_msg_.poses.size(); j += step){
            geometry_msgs::PoseStamped pose1;
            pose1 = path_msg_.poses[j];
            path_msg2_.poses.push_back(pose1);
       }
        
        

        // 发布点云数据
        path_pub_.publish(path_msg2_);
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber laser_sub_;
    ros::Publisher path_pub_;
    nav_msgs::Path path_msg_;
    nav_msgs::Path path_msg2_;
    tf::TransformListener tf_listener_;
    laser_geometry::LaserProjection projector;
    ros::Publisher pointcloud_publisher;
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















#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/LaserScan.h>
 void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan_data)
{
    try
    {
        // 创建一个TransformListener对象
        tf::TransformListener listener;
         // 等待TF变换可用
        listener.waitForTransform("map", scan_data->header.frame_id, ros::Time(0), ros::Duration(1.0));
         // 获取TF变换
        tf::StampedTransform transform;
        listener.lookupTransform("map", scan_data->header.frame_id, ros::Time(0), transform);
         // 创建一个转换器
        tf::Transformer transformer;
         // 将激光数据转换到map坐标系
        sensor_msgs::PointCloud transformed_scan;
        transformer.transformPointCloud("map", *scan_data, transformed_scan);
         // 处理转换后的激光数据
        // ...
    }
    catch (tf::TransformException& ex)
    {
        ROS_ERROR("Transform failed: %s", ex.what());
    }
}
 int main(int argc, char** argv)
{
    ros::init(argc, argv, "scan_transformer");
    ros::NodeHandle nh;
     ros::Subscriber sub = nh.subscribe("/scan", 1, scanCallback);
     ros::spin();
     return 0;
}


















