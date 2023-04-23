from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription(
        [
            Node(
                package='gakurobo_angle',
                executable='msg_convert',
                name='msg_converter',
                output='screen'
            ),
            
            Node(
                package='joy',
                executable='joy_node',
                name='joy',
            )
        ]
    )
