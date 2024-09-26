#! /usr/bin/env python

import rospy
import geometry_msgs.msg as geometry_msgs
from yahboom_web_savemap_interfaces.srv import WebSaveMap, WebSaveMapResponse, WebSaveMapRequest  
import json
import sqlite3
import uuid
import subprocess
import datetime
import hashlib




class SaveMapService:
    def __init__(self):
        rospy.init_node('add_two_ints_server')
        self.res = WebSaveMapResponse()

        # move_base_flex exe path client
        self.service = rospy.Service('yahboomAppSaveMap', WebSaveMap, self.handle_save_map)
        rospy.loginfo("save map server")
        

    def run_shellcommand(self, *args):
        '''run the provided command and return its stdout'''
        args = sum([(arg if type(arg) == list else [arg]) for arg in args], [])
        print(args)
        possess = subprocess.Popen(args, stdout=subprocess.PIPE)
        return possess


    def handle_save_map(self, request):
        map_name = request.mapname
        map_path = "/home/yahboom/rplidar_ws/src/yahboomcar_nav/maps/" + map_name
        now = datetime.datetime.now()
        str_time = now.strftime("%Y-%m-%d %H:%M:%S.%f")
        map_namestr = str_time + map_name
        map_id = hashlib.md5(map_namestr.encode()).hexdigest()
        self.res.response = request.mapname
        try:
            conn = sqlite3.connect("/home/yahboom/rplidar_ws/src/yahboomcar_nav/data/xgo.db")
            c = conn.cursor()
            c.execute("INSERT INTO xgo_map (map_name, map_id, map_path) VALUES (?, ?, ?)",(map_name, map_id, map_path))
            #self.run_shellcommand('ros2', 'run', 'nav2_map_server', 'map_saver_cli','-f', map_path, '--ros-args', '-p', 'save_map_timeout:=10000')
            self.run_shellcommand('rosrun', 'map_server', 'map_saver','-f', map_path)
        except sqlite3.IntegrityError as e:
            re_data = {"msg": str(e)}
            response.response = re_data
            #self.get_logger().info('Incoming request\nmapname: %s' % (re_data))  # CHANGE
            return self.res     
                                                        # CHANGE
        #self.get_logger().info('Incoming request\nmapname: %s' % (request.mapname))  # CHANGE

        return self.res
        


    def spin(self):
        rospy.spin()



if __name__ == '__main__':
    save_map_server = SaveMapService()
    save_map_server.spin()
