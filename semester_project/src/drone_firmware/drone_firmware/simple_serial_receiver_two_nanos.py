#!/usr/bin/env python3
import serial
import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class SimpleSerialReceiver(Node):
    def __init__(self):
        super().__init__("simple_serial_receiver_two_nanos")

        self.declare_parameter("port1", "/dev/ttyUSB0")
        self.declare_parameter("port2", "/dev/ttyUSB1")
        self.declare_parameter("baudrate", 115200)

        self.port1_ = self.get_parameter("port1").value
        self.port2_ = self.get_parameter("port2").value
        self.baudrate_ = self.get_parameter("baudrate").value

        self.pub_ = self.create_publisher(String, "serial_receiver", 10)
        self.arduino1_ = serial.Serial(port=self.port1_, baudrate=self.baudrate_, timeout=0.1)
        self.arduino2_ = serial.Serial(port=self.port2_, baudrate=self.baudrate_, timeout=0.1)

        self.frequency_ = 0.01
        self.timer_ = self.create_timer(self.frequency_, self.timerCallback)

    def timerCallback(self):
        if rclpy.ok() and (self.arduino1_.is_open or self.arduino2_.is_open) :
            data1 = self.arduino1_.readline()
            data2 = self.arduino2_.readline()

            try:
                data1.decode("utf-8")
                data2.decode("utf-8")
            except:
                return

            msg1 = String()
            msg2 = String()
            msg1.data = str(data1)
            self.pub_.publish(msg1)
            msg2.data = str(data2)
            self.pub_.publish(msg2)


def main():
    rclpy.init()

    simple_serial_receiver = SimpleSerialReceiver()
    rclpy.spin(simple_serial_receiver)
    
    simple_serial_receiver.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()