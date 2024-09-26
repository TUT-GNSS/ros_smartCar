#!/usr/bin/env python
# encoding: utf-8
import sys
import math
import rospy
import random
import threading
from math import pi
from time import sleep
from sensor_msgs.msg import Imu, MagneticField, JointState
from Rosmaster_Lib import Rosmaster
from geometry_msgs.msg import Twist
from std_msgs.msg import String, Float32, Int32, Bool
from yahboomcar_bringup.cfg import PIDparamConfig

class yahboomcar_driver:
    def __init__(self):
        rospy.on_shutdown(self.cancel)
        # 弧度转角度
        # Radians turn angle
        self.RA2DE = 180 / pi
        self.car = Rosmaster()
        self.imu_link = rospy.get_param("~imu_link", "imu_link")
        self.car.create_receive_threading()
        self.car.set_pwm_servo(1,120)
	#订阅者
	#Subscriber
        self.sub_cmd_vel = rospy.Subscriber('cmd_vel', Twist, self.cmd_vel_callback, queue_size=100)
        self.sub_cmd_vel = rospy.Subscriber('servo', Float32, self.servo_callback, queue_size=100)
        self.sub_RGBLight = rospy.Subscriber("RGBLight", Int32, self.RGBLightcallback, queue_size=100)
        self.sub_Buzzer = rospy.Subscriber("Buzzer", Bool, self.Buzzercallback, queue_size=100)
	#发布者
	#Publisher
        self.EdiPublisher = rospy.Publisher('edition', Float32, queue_size=100)
        self.volPublisher = rospy.Publisher('voltage', Float32, queue_size=100)
        self.staPublisher = rospy.Publisher('joint_states', JointState, queue_size=100)
        self.velPublisher = rospy.Publisher("/pub_vel", Twist, queue_size=100)
        self.imuPublisher = rospy.Publisher("/pub_imu", Imu, queue_size=100)
        self.magPublisher = rospy.Publisher("/pub_mag", MagneticField, queue_size=100)
        self.Servo_angelPublisher = rospy.Publisher("/pub_servo_angel", Float32, queue_size=100)


    def cancel(self):
        self.velPublisher.unregister()
        self.imuPublisher.unregister()
        self.EdiPublisher.unregister()
        self.volPublisher.unregister()
        self.staPublisher.unregister()
        self.magPublisher.unregister()
        self.Servo_angelPublisher.unregister()
        self.sub_cmd_vel.unregister()
        self.sub_RGBLight.unregister()
        self.sub_Buzzer.unregister()
        # Always stop the robot when shutting down the node
        rospy.loginfo("Close the robot...")
        rospy.sleep(1)

    def pub_data(self):
        # 发布小车速度、陀螺仪数据、电池电压
        ## Publish the speed of the car, gyroscope data, and battery voltage
        while not rospy.is_shutdown():
            sleep(0.05)
            imu = Imu()
            twist = Twist()
            battery = Float32()
            edition = Float32()
            servo = Float32()
            mag = MagneticField()
           #获取数据信息
           #get data
            edition.data = self.car.get_version()
            battery.data = self.car.get_battery_voltage()
            ax, ay, az = self.car.get_accelerometer_data()
            gx, gy, gz = self.car.get_gyroscope_data()
            mx, my, mz = self.car.get_magnetometer_data()
            vx, vy, angular = self.car.get_motion_data()
            servo.data = self.car.get_uart_servo_angle(1) #读取1号舵机的角度
            # 发布陀螺仪的数据
            # Publish gyroscope data
            imu.header.stamp = rospy.Time.now()
            imu.header.frame_id = self.imu_link
            imu.linear_acceleration.x = ax
            imu.linear_acceleration.y = ay
            imu.linear_acceleration.z = az
            imu.angular_velocity.x = gx
            imu.angular_velocity.y = gy
            imu.angular_velocity.z = gz
            mag.header.stamp = rospy.Time.now()
            mag.header.frame_id = self.imu_link
            mag.magnetic_field.x = mx
            mag.magnetic_field.y = my
            mag.magnetic_field.z = mz

            # 小车运动当前的线速度和角速度
            #  the current linear vel and angular vel of the car
            twist.linear.x = vx
            twist.linear.y = vy
            twist.angular.z = angular
 

            self.velPublisher.publish(twist)
            self.imuPublisher.publish(imu)
            self.magPublisher.publish(mag)
            self.volPublisher.publish(battery)
            self.EdiPublisher.publish(edition)           
            self.Servo_angelPublisher.publish(servo) 
            #打印数据信息
            #print data information
            #print("ax: %.5f, ay: %.5f, az: %.5f" % (ax, ay, az))
            #print("gx: %.5f, gy: %.5f, gz: %.5f" % (gx, gy, gz))
            #print("mx: %.5f, my: %.5f, mz: %.5f" % (mx, my, mz))
            #rospy.loginfo("battery: {}".format(battery))
            #rospy.loginfo("vx: {}, vy: {}, angular: {}".format(twist.linear.x, twist.linear.y, twist.angular.z))
            #rospy.loginfo("angel: {}".format(servo.data))
           
    def RGBLightcallback(self, msg):
        # 流水灯控制，服务端回调函数 RGBLight control
        '''
        effect=[0, 6]，0：停止灯效，1：流水灯，2：跑马灯，3：呼吸灯，4：渐变灯，5：星光点点，6：电量显示
        speed=[1, 10]，数值越小速度变化越快。
        '''
        if not isinstance(msg, Int32): return
        # print ("RGBLight: ", msg.data)
        for i in range(3): self.car.set_colorful_effect(msg.data, 6, parm=1)
        print("lights is on!")

    def Buzzercallback(self, msg):
        # 蜂鸣器控制  Buzzer control
        if not isinstance(msg, Bool): return
        # print ("Buzzer: ", msg.data)
        if msg.data:
            for i in range(3): 
                self.car.set_beep(1)
                print("beep on!")
        else:
            for i in range(3): 
                self.car.set_beep(0)
                print("beep off!")

    def cmd_vel_callback(self, msg):
        # 小车运动速度控制，订阅者回调函数 vx表示x方向线速度，vy表示y方向线速度，vz表示角速度
        # Car motion control, subscriber callback function
        if not isinstance(msg, Twist): return
        # 下发线速度和角速度
        # Issue linear vel and angular vel
        vx = msg.linear.x
        vy = msg.linear.y
        angular = msg.angular.z
        self.car.set_car_motion(vx, vy, angular)
        rospy.loginfo("vx: {}, vy: {}, angular: {}".format(vx, vx, angular))

    def servo_callback(self, msg):
        # 舵机控制，订阅者回调函数 1表示第一个舵机，angel表示舵机转动角度
        # Servo control, subscriber callback function 1 means the first servo, angel means the rotation angle of the servo
        angel = msg.data
        self.car.set_pwm_servo(1,angel)
        rospy.loginfo("angel: {}".format(angel))



if __name__ == '__main__':
    rospy.init_node("driver_node", anonymous=False)
    try:
        driver = yahboomcar_driver()
        driver.pub_data()
        rospy.spin()
    except:
        rospy.loginfo("Final!!!")
