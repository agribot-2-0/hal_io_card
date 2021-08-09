/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_iocard/IoCard.h>

#include <memory>

constexpr uint8_t ROUTER_FEEDBACK_PIN = 0;
constexpr uint8_t PWR_RELAY_PIN = 2;

int main(int argc, char** argv)
{
  auto io_card_handler_ = std::make_unique<IoCard>(OUTPUT | INPUT);

  io_card_handler_->vSetPinMask(ROUTER_FEEDBACK_PIN, 1);  // set 'router feedback' pin to state 1
  io_card_handler_->vSetPinMask(PWR_RELAY_PIN, 1);        // set 'pwr relay' pin to state 1
  io_card_handler_->vWritePort(PORT_0, io_card_handler_->u8GetPinMask(PORT_0));

  return 0;
}
