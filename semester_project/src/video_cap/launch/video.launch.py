from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, OpaqueFunction
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
from launch.conditions import UnlessCondition, IfCondition


def generate_launch_description():
    
    image_show = Node(
        package="video_cap",
        executable="image_show",
        name="image_show_node"
    )

    plate_reader = Node(
        package="video_cap",
        executable="licence_plate_recognition",
        name="plate_reader_node"
    )

    video_capture = Node(
        package="video_cap",
        executable="video_capture",
        name="video_capture_node"
    )

    return LaunchDescription(
        [
        #image_show,
        plate_reader,
        video_capture,
        ]
    )