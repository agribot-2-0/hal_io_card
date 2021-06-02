/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_iocard/IoCardNode.h>

IoCardNode::IoCardNode(int argc, char** argv, const char* node_name)
{
  // init ros
  ros::init(argc, argv, node_name);
  nh_ = std::make_unique<ros::NodeHandle>();
  io_card_handler_ = std::make_unique<IoCard>(OUTPUT | INPUT);

  registerServices();

  io_card_handler_->vSetPinMask(ROUTER_FEEDBACK_PIN, 1);  // set 'router feedback' pin to state 1
  io_card_handler_->vSetPinMask(PWR_RELAY_PIN, 1);        // set 'pwr relay' pin to state 1
  io_card_handler_->vWritePort(PORT_0, io_card_handler_->u8GetPinMask(PORT_0));
}

void IoCardNode::execute(void)
{
  ros::spin();
}

void IoCardNode::registerServices(void)
{
  io_out_service_ = nh_->advertiseService("io_card/out", &IoCardNode::ioControlOut, this);
  io_in_service_ = nh_->advertiseService("io_card/in", &IoCardNode::ioControlIn, this);
}

bool IoCardNode::ioControlOut(agribot_msgs::IoCardOut::Request& req,   // NOLINT(runtime/references)
                              agribot_msgs::IoCardOut::Response& res)  // NOLINT(runtime/references)
{
  io_card_handler_->vSetPinMask(req.pin, req.state);
  res.success = true;
  if (req.pin <= 7)
  {
    if (!io_card_handler_->vWritePort(PORT_0, io_card_handler_->u8GetPinMask(PORT_0)))
    {
      res.success = false;
      return true;
    }
  }

  if ((req.pin >= 8) && (req.pin <= 15))
  {
    if (!io_card_handler_->vWritePort(PORT_1, io_card_handler_->u8GetPinMask(PORT_1)))
    {
      res.success = false;
      return true;
    }
  }
  ROS_INFO("[HalIoCard] Writing pin DO%d with state %d", (uint16_t)req.pin, (uint8_t)req.state);

  return true;
}

bool IoCardNode::ioControlIn(agribot_msgs::IoCardIn::Request& req,   // NOLINT(runtime/references)
                             agribot_msgs::IoCardIn::Response& res)  // NOLINT(runtime/references)
{
  uint8_t read_pin = 0x01;
  uint32_t port_state = 0x00;
  res.success = false;
  if (req.pin <= 7)
  {
    port_state = io_card_handler_->u8ReadPort(PORT_0);
    read_pin = read_pin << req.pin;
  }
  if ((req.pin >= 8) && (req.pin <= 15))
  {
    port_state = io_card_handler_->u8ReadPort(PORT_1);
    read_pin = read_pin << (req.pin - PORT1_START_PIN);
  }

  if ((port_state & read_pin) == read_pin)
  {
    res.state = req.ON;
    res.success = true;
  }
  else
  {
    res.state = req.OFF;
    res.success = true;
  }

  ROS_INFO("[HalIoCard] Reading pin DI%d. The state is %d", (uint16_t)req.pin, (uint8_t)res.state);
  return true;
}
