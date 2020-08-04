/*
 * Copyright (C) 2020 Agribot. All rights reserved.
 */

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <hal_iocard/bdaq/compatibility.h>
#include <hal_iocard/bdaq/bdaqctrl.h>
#include <iostream>

#define PORT_0 0
#define PORT_1 1
#define PORT1_START_PIN 8

typedef unsigned char byte;
enum eCardMode
{
  NOT_SELECTED,
  INPUT,
  OUTPUT
};
inline eCardMode operator|(eCardMode a, eCardMode b)
{
  return static_cast<eCardMode>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

class IoCard
{
 private:
  InstantDoCtrl* _pInstantDoCtrl;
  InstantDiCtrl* _pInstantDiCtrl;
  ErrorCode _Ret;
  const wchar_t* _pProfilePath = L"../DemoDevice.xml";
  eCardMode _eSelMode;
  uint16_t _u16OutMask;

 public:
  explicit IoCard(eCardMode Mode);
  ~IoCard();
  bool vWritePort(int32 i32PortNum, uint8_t u8PinMask);
  uint8_t u8ReadPort(int32 readPort);
  void vSetPinMask(uint8_t u8PinNum, bool bState);
  uint8_t u8GetPinMask(uint8_t u8Port);
};
