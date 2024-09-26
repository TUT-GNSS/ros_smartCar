#!/usr/bin/env python
# coding:utf-8
import rospy
from sensor_msgs.msg import LaserScan
from laser_geometry import LaserProjection
import numpy as np
from time import sleep

class scan_compression:
    def __init__(self):
        self.laserProj = LaserProjection()
        self.laserSub = rospy.Subscriber("/scan", LaserScan, self.laserCallback)
        self.pub = rospy.Publisher("/scan_compress", LaserScan, queue_size=1000)

    def laserCallback(self, data):
        laser_scan = LaserScan()
        laser_scan.header = data.header
        laser_scan.header.frame_id = 'laser_compress'
        laser_scan.angle_min = data.angle_min
        laser_scan.angle_max = data.angle_max
        laser_scan.angle_increment = data.angle_increment
        laser_scan.time_increment = data.time_increment
        laser_scan.scan_time = data.scan_time
        laser_scan.range_min = data.range_min
        laser_scan.range_max = data.range_max
        for i in range(len(np.array(data.ranges))):
            if i % 3 == 0:
                laser_scan.ranges.append(data.ranges[i])
                laser_scan.intensities.append(data.intensities[i])
        self.pub.publish(laser_scan)

if __name__ == '__main__':
    rospy.init_node('scan_compression', anonymous=False)
    pt2img = scan_compression()
    rospy.spin()

