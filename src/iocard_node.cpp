#include <hal_iocard/IoCard.h>

#include <ros/ros.h>
#include <ros/console.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "iocard");
    uint8_t pinMask = 0x00;
    IoCard outTest(OUTPUT);
    outTest.vWritePort(PORT0, pinMask);
    
    IoCard inTest(INPUT);
    uint8_t u8Status = inTest.u8ReadPort(PORT0);
    std::cout << (int)u8Status << std::endl;

    //Note: Object can be also initialzie as input and output
    // simultaneously IoCard IOTest(OUTPUT | INPUT);
    return 0;
}