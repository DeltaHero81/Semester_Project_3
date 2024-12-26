import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

class ImageShow(Node):
    def __init__(self):
        super().__init__("image_show")
        self.sub = self.create_subscription(
            Image,
            "current_frame",
            self.msgCallback,
            20)
        self.bridge = CvBridge()
    
    def msgCallback(self, imsg):
        self.get_logger().info('Receiving video frame')
        current_frame = self.bridge.imgmsg_to_cv2(imsg)
        cv2.imshow("camera", current_frame)
        cv2.waitKey(1)

def main():
    rclpy.init()
    image_show = ImageShow()
    rclpy.spin(image_show)
    image_show.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()