#! /usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import UInt8MultiArray
from copy import copy
import can
import os

try:
	os.system('sudo ip link set can0 type can bitrate 500000')
	os.system('sudo ifconfig can0 up')
	can0 = can.interface.Bus(channel = 'can0', bustype = 'socketcan_ctypes')
except OSError:
	
	exit()

class can_c(Node):
	node_name = 'can_tx_node'
	topic_name = 'can_tx'
	
	def __init__(self)->None:
		super().__init__(self.node_name)
		self.sub = self.create_subscription(UInt8MultiArray, self.topic_name, self.callback, 10)
		self.tim = self.create_timer(0.001, self.tim_callback)
		
	def callback(self, msg):
		can_tx_msg = can.Message(arbitration_id=0x1, data=msg.data, extended_id=False)
		can0.send(can_tx_msg)
  
	def tim_callback(self):
		can_msg = can0.recv(0.005)
		if can_msg is not None and (can_msg.arbitration_id == 2):
			m = can_msg.data[0] << 24 + can_msg.data[1] << 16 + can_msg.data[2] << 8 + can_msg.data[3]
			if can_msg.data[0] >>7 == 1:
				m &= 0x7fffffff
				m *= -1
			self.get_logger().info('%d'  %(m))
			pass

def main(args=None):
	rclpy.init(args=args)
	c = can_c()
	
	try:
		rclpy.spin(c)
	except KeyboardInterrupt:
		pass
	finally:
		c.destroy_node()


if __name__ == '__main__':
	main()