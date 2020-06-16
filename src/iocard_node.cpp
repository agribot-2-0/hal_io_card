#include <hal_iocard/IoCard.h>

#include <ros/ros.h>
#include <ros/console.h>
#include <std_msgs/UInt8.h>

IoCard g_outTest(OUTPUT);

void setPortVal0(const std_msgs::UInt8::ConstPtr& msg)
{
    g_outTest.vWritePort(PORT0, (uint8_t)msg->data);
    ROS_INFO("DO0: 0x%X", (uint8_t)msg->data);
}

void setPortVal1(const std_msgs::UInt8::ConstPtr& msg)
{
    g_outTest.vWritePort(PORT1, (uint8_t)msg->data);
    ROS_INFO("DO1: 0x%X", (uint8_t)msg->data);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "iocard");
    ros::NodeHandle n;
    uint8_t pinMask = 0x00;
    
    // <--- READ INPUT EXAMPLE --->
    // IoCard inTest(INPUT);
    // uint8_t u8Status = inTest.u8ReadPort(PORT0);
    // std::cout << static_cast<int>(u8Status) << std::endl;
    // Note: Object can be also initialzied as input and output
    // simultaneously: IoCard IOTest(OUTPUT | INPUT);
    // <-------------------------->

    ros::Rate loop_rate(1);
    ros::Subscriber sub_port0 = n.subscribe("hal_iocard/DO0", 1000, setPortVal0);
    ros::Subscriber sub_port1 = n.subscribe("hal_iocard/DO1", 1000, setPortVal1);

    while (ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
