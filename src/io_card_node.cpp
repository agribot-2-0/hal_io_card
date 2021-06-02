/*
 * Copyright (C) 2020-2021 Agribot. All rights reserved.
 */

#include <hal_iocard/IoCardNode.h>

int main(int argc, char** argv)
{
  try
  {
    IoCardNode io_card_node(argc, argv, "hal_io_card");
    io_card_node.execute();
  }
  catch (const std::exception& ex)
  {
    ROS_ERROR_STREAM("[HalIoCard] " << ex.what());
  }

  return 0;
}
