#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <hal_iocard/compatibility.h>
#include <hal_iocard/bdaqctrl.h>

typedef unsigned char byte;

class IoCard
{
private:
    InstantDoCtrl *instantDoCtrl;
    ErrorCode ret;
    const wchar_t* profilePath;
public:
    IoCard();
    ~IoCard();
    void vWriteOutputs(int32 startPort, int i, uint8_t outputMask);
    uint8_t u8ReadInputs();
};