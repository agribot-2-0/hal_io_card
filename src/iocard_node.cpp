#include <hal_iocard/IoCard.h>

#include <ros/ros.h>
#include <ros/console.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "iocard");
    IoCard abc;
    
    return 0;
}