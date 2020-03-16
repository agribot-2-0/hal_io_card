#include <hal_iocard/IoCard.h>

#include <ros/ros.h>
#include <ros/console.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "iocard");
    uint8_t out = 0x01;
    IoCard apiTest;
    //apiTest.vWriteOutputs(0x00,0x01, out);
    uint8_t u8Status = apiTest.u8ReadInputs();
    
    return 0;
}