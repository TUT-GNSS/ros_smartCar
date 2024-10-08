
#include <sensor_msgs/LaserScan.h>
#include <tf/tf.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Imu.h>

void LidarCallback(const sensor_msgs::LaserScan msg)
{
    // float fMidDist0 = msg.ranges[0];
    // ROS_INFO("前方测距 ranges[0] =%f 米", fMidDist0); //获取正前方数据

    // float fMidDist90 = msg.ranges[90];
    // ROS_INFO("前方测距 ranges[90] =%f 米", fMidDist90); //获取正前方数据


    // float fMidDist180 = msg.ranges[180];
    // ROS_INFO("前方测距 ranges[180] =%f 米", fMidDist180); //获取正前方数据

    float fMidDist270 = msg.ranges.size();
    ROS_INFO("前方测距 ranges[270] =%f 米", fMidDist270); //获取正前方数据
}

int main(int argc, char *argv[])
{

    setlocale(LC_ALL, "");
    ros::init(argc, argv, "lidar_node");
    ros::NodeHandle nh;
    ros::Subscriber lidar_sub = nh.subscribe("/scan", 10, &LidarCallback);

    ros::spin();

    return 0;

    // ros::init(argc, argv, "vel_node");

    // ros::NodeHandle nd;
    // ros::Publisher vel_pub = nd.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // geometry_msgs::Twist vel_msg;
    // vel_msg.linear.x = 0.1;
    // vel_msg.linear.y = 0;
    // vel_msg.linear.z = 0;

    // vel_msg.angular.x = 0;
    // vel_msg.angular.y = 0;
    // vel_msg.angular.z = 0;
    
    // ros::Rate r(1);

    // while (ros::ok())
    // {
    //     vel_msg.linear.x = 0.1;
    //     r.sleep();
    //     vel_pub.publish(vel_msg);

    //     vel_msg.linear.x = 0;
    //     r.sleep();
    //     vel_pub.publish(vel_msg);
    // }
    // return 0;
}



// ros::Publisher vel_pub;

// void IMUCallback(sensor_msgs::Imu msg)
// {
//     if (msg.orientation_covariance[0] < 0) //如果协方差矩阵第一个值0，则四元数不存在，返回
//         return;

//     //四元数对象
//     tf::Quaternion quaternion(msg.orientation.x, msg.orientation.y, msg.orientation.z, msg.orientation.w); //

//     double roll, pitch, yaw; //欧拉角
//     //四元数转换成欧拉角
//     tf::Matrix3x3(quaternion).getRPY(roll, pitch, yaw); //将quaternion转换成tf3x3矩阵对象，再getRPY得到欧拉角

//     //转换后是弧度值，转换为角度值
//     roll = roll * 180 / M_PI;
//     pitch = pitch * 180 / M_PI;
//     yaw = yaw * 180 / M_PI;

//     ROS_INFO("滚转=%0.f 俯仰=%0.f 旋转=%0.f", roll, pitch, yaw);

//     //定义目标朝向角
//     double target_yaw = 90;
//     double diff_angle = target_yaw - yaw; //差值

//     geometry_msgs::Twist vel_cmd;
//     vel_cmd.angular.z = diff_angle * 0.01; // 0.01为差值与旋转速度的比例关系

//     vel_cmd.linear.x = 0.1;

//     vel_pub.publish(vel_cmd);
// }

// int main(int argc, char *argv[])
// {
//     setlocale(LC_ALL, "");
//     ros::init(argc, argv, "imu_node");
//     ros::NodeHandle nh;
//     // ros::Subscriber imuSub = nh.subscribe("/imu/imu_data", 1000, IMUCallback);
//     ros::Subscriber imuSub = nh.subscribe("/wit/imu", 1000, IMUCallback);
//     vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);

//     ros::spin();

//     return 0;
// }
