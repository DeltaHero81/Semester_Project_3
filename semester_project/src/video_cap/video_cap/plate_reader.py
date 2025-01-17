import os

# Import ros2 libraties
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge

# Import the needed libraries
import cv2
import numpy as np
import easyocr

# Import additional py file (like a header file)
import util


class LicencePlateRecognition(Node):
    def __init__(self):
        super().__init__("licence_plate_recognition")

        # INITIALIZATION
        #///////////////////////////////////////////////////////////////////////////////////
        # Define constants with the available files for the YOLOv3-416 model
        self.model_cfg_path = os.path.join('home', 'vboxuser', 'semester_project', 'src', 'video_cap', 
                                           'yolov3', 'model', 'cfg', 'darknet-yolov3.cfg')
        self.model_weights_path = os.path.join('home', 'vboxuser', 'semester_project', 'src', 'video_cap', 
                                               'yolov3', 'model', 'weights', 'model.weights')
        self.class_names_path = os.path.join('home', 'vboxuser', 'semester_project', 'src', 'video_cap', 
                                             'yolov3', 'model', 'class.names')

        # load class names
        with open(self.class_names_path, 'r') as f:
            class_names = [j[:-1] for j in f.readlines() if len(j) > 2]
            f.close()

        # load model
        self.net = cv2.dnn.readNetFromDarknet(self.model_cfg_path, self.model_weights_path)
        #///////////////////////////////////////////////////////////////////////////////////

        self.sub_image = self.create_subscription(Image, 
                                            "current_frame",
                                            self.imagemsgCallback,
                                            20)
        self.bridge = CvBridge()

        self.sub_plate = self.create_subscription(String,
                                            "plate_number",
                                            self.platemsgCallback,
                                            20)

    def platemsgCallback(self, plate):
        self.get_logger().info("Main plate: %s" % plate)
        self.plate_number = plate
        

    def imagemsgCallback(self, imsg):
        self.get_logger().info('Receiving video frame')
        current_frame = self.bridge.imgmsg_to_cv2(imsg)

        # Take the height and the width of the frame
        self.H, self.W, _ = current_frame.shape

        # convert frame
        self.blob = cv2.dnn.blobFromImage(current_frame, 1 / 255, (416, 416), (0, 0, 0), True)

        # Set the input for the network
        self.net.setInput(self.blob)

        # Get detections
        self.detections = util.get_outputs(self.net)

        # bboxes, class_ids, confidences
        self.bboxes = []
        self.class_ids = []
        self.scores = []

        if self.detections:
            self.get_logger().info("Successful detection has happened")
            for detection in self.detections:
                # [x1, x2, x3, x4, x5, x6, ..., x85]
                self.bbox = detection[:4]

                self.xc, self.yc, self.w, self.h = self.bbox
                self.bbox = [int(self.xc * self.W), int(self.yc * self.H), int(self.w * self.W), int(self.h * self.H)]

                bbox_confidence = detection[4]

                self.class_id = np.argmax(detection[5:])
                self.score = np.amax(detection[5:])

                self.bboxes.append(self.bbox)
                self.class_ids.append(self.class_id)
                self.scores.append(self.score)

            # apply nms
            self.bboxes, self.class_ids, self.scores = util.NMS(self.bboxes, self.class_ids, self.scores)

            # plot
            self.reader = easyocr.Reader(['en'])

            for bbox in enumerate(self.bboxes):
                self.xc, self.yc, self.w, self.h = bbox

                # Draw a rectangle around the licence plate
                self.frame = cv2.rectangle(self.frame,
                                    (int(self.xc - (self.w / 2)), int(self.yc - (self.h / 2))),
                                    (int(self.xc + (self.w / 2)), int(self.yc + (self.h / 2))),
                                    (0, 255, 0),
                                    10)
                    
                # Change the picture to gray to get rid of the unneeded colour channels
                license_plate_gray = cv2.cvtColor(self.frame, cv2.COLOR_BGR2GRAY)

                # Change the picture to pure black and white regarding the gray value to make the text reader's job easier
                _, license_plate_thresh = cv2.threshold(license_plate_gray, 128, 255, cv2.THRESH_BINARY_INV)

                # Read the text detected on the picture
                self.output = self.reader.readtext(license_plate_thresh)

                # For every output generated by the text reader, write out its text and how confident the algorithm is
                for out in self.output:
                    self.text_bbox, self.text, self.text_score = out
                    if self.text_score > 0.7:
                        self.get_logger().info("The read text is: %s and its confidence is: %f" % (self.text, self.text_score))
                        
                        if self.text == self.plate_number:
                            self.get_logger().info("Number plate is correct")
                        else:
                            self.get_logger().info("Number plate is incorrect")
                
                self.output = 0


def main(args=None):
    rclpy.init(args=args)
    licence_plate_recognition = LicencePlateRecognition()
    rclpy.spin(licence_plate_recognition)
    licence_plate_recognition.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()