/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#include <agribot_msgs/IoCardIn.h>
#include <agribot_msgs/IoCardOut.h>
#include <hal_iocard/IoCard.h>
#include <ros/console.h>
#include <ros/ros.h>

IoCard g_IoControl(OUTPUT | INPUT);

bool bControlOut(agribot_msgs::IoCardOut::Request& req,   // NOLINT(runtime/references)
                 agribot_msgs::IoCardOut::Response& res)  // NOLINT(runtime/references)
{
  g_IoControl.vSetPinMask(req.pin, req.state);
  res.success = true;
  if (req.pin <= 7)
  {
    if (!g_IoControl.vWritePort(PORT_0, g_IoControl.u8GetPinMask(PORT_0)))
    {
      res.success = false;
      return true;
    }
  }

  if ((req.pin >= 8) && (req.pin <= 15))
  {
    if (!g_IoControl.vWritePort(PORT_1, g_IoControl.u8GetPinMask(PORT_1)))
    {
      res.success = false;
      return true;
    }
  }
  ROS_INFO("Writing pin DO%d with state %d", (uint16_t)req.pin, (uint8_t)req.state);

  return true;
}

bool bControlIn(agribot_msgs::IoCardIn::Request& req,   // NOLINT(runtime/references)
                agribot_msgs::IoCardIn::Response& res)  // NOLINT(runtime/references)
{
  uint8_t u8ReadPin = 0x01;
  uint32_t u32PortState = 0x00;
  res.success = false;
  if (req.pin <= 7)
  {
    u32PortState = g_IoControl.u8ReadPort(PORT_0);
    u8ReadPin = u8ReadPin << req.pin;
  }
  if ((req.pin >= 8) && (req.pin <= 15))
  {
    u32PortState = g_IoControl.u8ReadPort(PORT_1);
    u8ReadPin = u8ReadPin << (req.pin - PORT1_START_PIN);
  }

  if ((u32PortState & u8ReadPin) == u8ReadPin)
  {
    res.state = req.ON;
    res.success = true;
  }
  else
  {
    res.state = req.OFF;
    res.success = true;
  }

  ROS_INFO("Reading pin DI%d. The state is %d", (uint16_t)req.pin, (uint8_t)res.state);
  return true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "iocard");
  ros::NodeHandle n;
  ros::Rate loop_rate(0.1);
  constexpr uint8_t router_feedback_pin = 0;
  constexpr uint8_t pwr_relay_pin = 2;

  ros::ServiceServer service_in = n.advertiseService("io_card/in", bControlIn);
  ros::ServiceServer service_out = n.advertiseService("io_card/out", bControlOut);

  g_IoControl.vSetPinMask(router_feedback_pin, 1);  // set 'router feedback' pin to state 1
  g_IoControl.vSetPinMask(pwr_relay_pin, 1);        // set 'pwr relay' pin to state 1
  g_IoControl.vWritePort(PORT_0, g_IoControl.u8GetPinMask(PORT_0));

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
