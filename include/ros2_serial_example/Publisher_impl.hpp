#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string__rosidl_typesupport_fastrtps_cpp.hpp>

#include <fastcdr/Cdr.h>
#include <fastcdr/FastCdr.h>

#include "ros2_serial_example/publisher.hpp"

template<typename T>
class Publisher_impl : public Publisher
{
public:
    explicit Publisher_impl(const std::shared_ptr<rclcpp::Node> & node, const std::string & name)
    {
        pub = node->create_publisher<T>(name);
    }

    void dispatch(char data_buffer[], ssize_t length) override
    {
        // string topic
        eprosima::fastcdr::FastBuffer cdrbuffer(data_buffer, length);
        eprosima::fastcdr::Cdr cdrdes(cdrbuffer);
        auto msg = std::make_shared<T>();
        std_msgs::msg::typesupport_fastrtps_cpp::cdr_deserialize(cdrdes, *(msg.get()));
        pub->publish(msg);
    }

private:
    std::shared_ptr<rclcpp::Publisher<T>> pub;
};