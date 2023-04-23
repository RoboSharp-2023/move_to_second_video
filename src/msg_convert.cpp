#include <memory>
#include <algorithm>
#include <utility>
#include <vector>
#include <array>
#include <chrono>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/u_int8_multi_array.hpp>
#include <sensor_msgs/msg/joy.hpp>

using namespace std;
using namespace std::chrono_literals;
using std::placeholders::_1;

class msg_conv : public rclcpp::Node {
	rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_sub;
	rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr can_tx;
	rclcpp::TimerBase::SharedPtr tim;
	sensor_msgs::msg::Joy::SharedPtr recv_msg;

	void sub_callback(const sensor_msgs::msg::Joy::SharedPtr recv_msg_) {
		recv_msg = recv_msg_;

	}

	void timer_callback() {
		float angle1 = recv_msg->axes[7];
		float angle2 = recv_msg->axes[0];
		float rise   = recv_msg->axes[4];

		RCLCPP_INFO(this->get_logger(), "%f %f %f", recv_msg->axes[7], recv_msg->axes[0], recv_msg->axes[4]);

		std_msgs::msg::UInt8MultiArray msg;
		msg.data.resize(8);
		constexpr float coef = 127.0;

		msg.data[0] = (uint8_t)(angle1);
		msg.data[1] = (uint8_t)(angle2*coef);
		msg.data[2] = (uint8_t)(rise*coef);

		can_tx->publish(msg);
	}

public:
	msg_conv(): Node("msg_conv") {
		recv_msg = std::make_shared<sensor_msgs::msg::Joy>();
		recv_msg->axes.resize(8);
		joy_sub = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, std::bind(&msg_conv::sub_callback, this, _1));
		can_tx  = this->create_publisher<std_msgs::msg::UInt8MultiArray>("can_tx", 10);
		tim     = this->create_wall_timer(1ms, std::bind(&msg_conv::timer_callback, this));
	}
};

int main(int argc, char*argv[]) {
	rclcpp::init(argc, argv);
	rclcpp::spin(make_shared<msg_conv>());
	rclcpp::shutdown();
	return 0;
}
