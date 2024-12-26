#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import serial


class SimpleSerialTransmitter(Node):
    def __init__(self):
        super().__init__("simple_serial_transmitter_two_nanos")

        self.declare_parameter("port1", "/dev/ttyUSB0")
        self.declare_parameter("port2", "/dev/ttyUSB1")
        self.declare_parameter("baudrate", 115200)

        self.port1_ = self.get_parameter("port1").value
        self.port2_ = self.get_parameter("port2").value
        self.baudrate_ = self.get_parameter("baudrate").value

        self.sub_ = self.create_subscription(String, "serial_transmitter", self.msgCallback, 10)
        self.sub_
        self.arduino1_ = serial.Serial(port=self.port1_, baudrate=self.baudrate_, timeout=0.1)
        self.arduino2_ = serial.Serial(port=self.port2_, baudrate=self.baudrate_, timeout=0.1)

    def msgCallback(self, msg):
        self.get_logger().info("New message received, publishing on serial: %s and %s" % (self.arduino1_.name, self.arduino2_.name))
        self.arduino1_.write(msg.data.encode("utf-8"))
        self.arduino2_.write(msg.data.encode("utf-8"))


def main():
    rclpy.init()

    simple_serial_transmitter = SimpleSerialTransmitter()
    rclpy.spin(simple_serial_transmitter)
    
    simple_serial_transmitter.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()