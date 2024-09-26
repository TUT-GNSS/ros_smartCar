#! /usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
from sensor_msgs.msg import NavSatFix

def GPSCallback(msg):
	rospy.loginfo("latitude:%0.6f, longitude:%0.6f,altitude:%0.6f", msg.latitude, msg.latitude,msg.altitude)	
def GPS_subscriber():
	rospy.init_node('GPS_subscriber', anonymous=True)# ROS节点初始化
	rospy.Subscriber("/fix", NavSatFix, GPSCallback)
	rospy.spin()# 循环等待回调函数

if __name__ == '__main__': 
	GPS_subscriber()
