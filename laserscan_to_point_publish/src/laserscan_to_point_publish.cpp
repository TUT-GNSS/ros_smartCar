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
        pointcloud_publisher = nh_.advertise<sensor_msgs::PointCloud> ("/laserscan_to_pointcloud", 100);
    }

    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan_msg)
    {

        path_msg_.poses.clear();
        path_msg2_.poses.clear();
        sensor_msgs::LaserScan rotated_scan = *scan_msg;
        //std::reverse(rotated_scan.ranges.begin(), rotated_scan.ranges.end());
        //std::reverse(rotated_scan.intensities.begin(), rotated_scan.intensities.end());
       
        if(!tf_listener_.waitForTransform(
            scan_msg->header.frame_id,
            "/base_link",
            scan_msg->header.stamp + ros::Duration().fromSec(scan_msg->ranges.size()*scan_msg->time_increment),
           ros::Duration(10))){
        ROS_INFO("timestamp error");
        return;
    }   
    try
    {
        projector_.transformLaserScanToPointCloud("/base_link",*scan_msg,mapcloud,tf_listener_);
    }
    catch(const std::exception& e)
    {
        ROS_ERROR("%s", e.what());
    }
    
     pointcloud_publisher.publish(mapcloud);
       
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber laser_sub_;
    ros::Publisher path_pub_;
    nav_msgs::Path path_msg_;
    nav_msgs::Path path_msg2_;
    tf::TransformListener tf_listener_;
    ros::Publisher pointcloud_publisher;
    
    laser_geometry::LaserProjection projector_;
    sensor_msgs::PointCloud mapcloud; 
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


















