#include <hal_iocard/IoCard.h>

#include <ros/ros.h>
#include <ros/console.h>
#include <agribot_msgs/IoCard.h>

IoCard g_IoControl(OUTPUT | INPUT);

bool bControlIO(agribot_msgs::IoCard::Request &req,
                agribot_msgs::IoCard::Response &res)
{
    res.port_state = 0x00;
    ROS_INFO("Writing port 0x0%x with mask 0x0%x",
             (uint8_t)req.sel_out_port, (uint8_t)req.out_port_mask);

    if ((uint8_t)req.out_port_mask != req.NONE)
    {
        if (!g_IoControl.vWritePort((uint8_t)req.sel_out_port,
                                    (uint8_t)req.out_port_mask))
            return false;
    }

    if ((uint8_t)req.sel_read_port != req.NONE)
    {
        res.port_state = g_IoControl.u8ReadPort((uint8_t)req.sel_read_port);
    }
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "iocard");
    ros::NodeHandle n;
    ros::Rate loop_rate(1);

    ros::ServiceServer service =
        n.advertiseService("hal_io_card/control", bControlIO);
    while (ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
