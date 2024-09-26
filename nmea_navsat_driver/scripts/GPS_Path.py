#! /usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
from sensor_msgs.msg import NavSatFix
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Path

EARTH_RADIUS = 6378.137


class transbot_driver:
	
	def __init__(self):
		rospy.on_shutdown(self.cancel)
		self.GPS_Data=rospy.Subscriber("/fix", NavSatFix, self.GPSCallback)
		self.latitude = 0
		self.longitude = 0
		self.altitude = 0
	def GPSCallback(self,msg):
		self.latitude = msg.latitude
		self.longitude = msg.longitude
		self.altitude = msg.altitude
		rospy.loginfo("latitude:%0.6f, longitude:%0.6f,altitude:%0.6f", msg.latitude, msg.longitude,msg.altitude)
	def cancel(self):
		self.GPS_Data.unregister()
	def PubData(self):
		#sleep(0.05)
		while not rospy.is_shutdown():
			#rospy.loginfo("latitude:%0.6f, longitude:%0.6f,altitude:%0.6f", self.latitude, self.longitude,self.altitude)
			print("latitude:%0.6f",self.latitude)
			print("longitude:%0.6f",self.longitude)
			print("altitude:%0.6f",self.altitude)
			
		 

if __name__ == '__main__':
	rospy.init_node('GPS_subscriber', anonymous=True)# ROS节点初始化	
	gps = transbot_driver()
	try:
		gps.PubData()
		rospy.spin()	
	except:
		rospy.loginfo("Final!!!")
