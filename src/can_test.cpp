#include <memory>
#include <algorithm>
#include <utility>
#include <vector>
#include <array>
#include <chrono>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/u_int8_multi_array.hpp>

using namespace std::chrono_literals;

class can_test: public rclcpp::Node {
    rclcpp::TimerBase::SharedPtr tim;
    rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr pub;

    void timer_callback() {
        RCLCPP_INFO(this->get_logger(), "timer");
        std_msgs::msg::UInt8MultiArray msg;

        msg.data.resize(8);

        msg.data[0] = 0x19;

        pub->publish(msg);
    }

public:
    can_test(): Node("can_tx_test") {
        tim = this->create_wall_timer(100ms, std::bind(&can_test::timer_callback, this));
        pub = this->create_publisher<std_msgs::msg::UInt8MultiArray>("can_tx", 10);
    }
};

int main(int argc, char*argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<can_test>());
    rclcpp::shutdown();
    return 0;
}