/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_msgs/IoCardIn.h>
#include <agribot_msgs/IoCardOut.h>
#include <hal_iocard/IoCard.h>
#include <ros/console.h>
#include <ros/ros.h>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

class IoCardNode
{
  static constexpr uint8_t ROUTER_FEEDBACK_PIN = 0;
  static constexpr uint8_t PWR_RELAY_PIN = 2;

  std::unique_ptr<IoCard> io_card_handler_;
  std::unique_ptr<ros::NodeHandle> nh_;
  ros::ServiceServer io_out_service_;
  ros::ServiceServer io_in_service_;

  void registerServices(void);
  bool ioControlOut(agribot_msgs::IoCardOut::Request& req,    // NOLINT(runtime/references)
                    agribot_msgs::IoCardOut::Response& res);  // NOLINT(runtime/references)
  bool ioControlIn(agribot_msgs::IoCardIn::Request& req,      // NOLINT(runtime/references)
                   agribot_msgs::IoCardIn::Response& res);    // NOLINT(runtime/references)

public:
  IoCardNode(int argc, char** argv, const char* node_name);
  void execute(void);
};
