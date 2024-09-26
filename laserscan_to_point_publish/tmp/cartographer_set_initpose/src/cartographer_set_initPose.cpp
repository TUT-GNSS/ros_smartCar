#include <string>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "cartographer_ros/map_builder_bridge.h"
//#include "cartographer/mapping/proto/scan_matching/fast_correlative_scan_matcher_results_2d.pb.h"
#include "cartographer/common/configuration_file_resolver.h"
#include "cartographer/io/proto_stream.h"
#include "cartographer/mapping/map_builder.h"
#include "cartographer_ros/node_constants.h"
#include "cartographer_ros/node_options.h"
#include "cartographer_ros/ros_log_sink.h"
//#include "cartographer_ros_msgs/srv/"
//#include "cartographer_ros_msgs/FinishTrajectory.h"
//#include "cartographer_ros_msgs/GetTrajectoryStates.h"
//#include "cartographer_ros_msgs/StartTrajectory.h"
//#include "cartographer_ros_msgs/StatusCode.h"
//#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "gflags/gflags.h"
#include "std_msgs/msg/string.hpp"
//#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>

class InitPoseSubscriber : public rclcpp::Node {
public:
    InitPoseSubscriber()
            : Node("init_pose_subscriber_node")
    {
        // 创建订阅器，订阅/initpose话题
        subscription_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
                "/initpose", 1,
                std::bind(&InitPoseSubscriber::initPoseCallback, this, std::placeholders::_1));
    }

private:
    void initPoseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg) {
        // 处理接收到的初始化位姿数据

    }

    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr subscription_;
};

int main(int argc, char** argv) {
    // 初始化ROS节点
    rclcpp::init(argc, argv);

    // 创建InitPoseSubscriber对象
    auto subscriber = std::make_shared<InitPoseSubscriber>();

    // 开启ROS事件循环
    rclcpp::spin(subscriber);

    // 关闭ROS节点
    rclcpp::shutdown();

    return 0;
}


