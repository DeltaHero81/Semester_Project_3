import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

class VideoCapture(Node):
    def __init__(self):
        super().__init__("video_capture")

        self.capture = cv2.VideoCapture(0)

        self.bridge = CvBridge()

        self.pub = self.create_publisher(Image, "current_frame", 20)

        self.i = 0.0
        self.frequency = 0.02

        self.timer = self.create_timer(self.frequency, self.timerCallback)


    def timerCallback(self):
        success, frame = self.capture.read()
        
        if success:
            image_msg = self.bridge.cv2_to_imgmsg(frame)
            self.pub.publish(image_msg)

def main():
    rclpy.init()
    video_capture = VideoCapture()
    rclpy.spin(video_capture)
    video_capture.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()