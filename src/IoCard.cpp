#include <hal_iocard/IoCard.h>

#define  deviceDescription  L"PCIe-1730,BID#0"

IoCard::IoCard(eCardMode eMode)
{
    _Ret = Success;
    _eSelMode = NOT_SELECTED;
    _u16OutMask = 0x00;

    if ((eMode&INPUT) == INPUT)
    {
        _pInstantDiCtrl = InstantDiCtrl_Create();
        _eSelMode = _eSelMode | INPUT;
    }
    if ((eMode&OUTPUT) == OUTPUT)
    {
        _pInstantDoCtrl = InstantDoCtrl_Create();
        _eSelMode = _eSelMode | OUTPUT;
    }
}

IoCard::~IoCard()
{
    if ((_eSelMode&INPUT) == INPUT)
    {
        InstantDiCtrl_Dispose(_pInstantDiCtrl);
    }
    if ((_eSelMode&OUTPUT) == OUTPUT)
    {
        InstantDoCtrl_Dispose(_pInstantDoCtrl);
    }
}

bool IoCard::vWritePort(int32 i32PortNum, uint8_t u8PinMask)
{
  if ((_eSelMode&OUTPUT) == OUTPUT)
    {
        _Ret = Success;
        wchar_t enumString[256];
        byte dataBuffer[64] = {0};  // the first element is used for start port
        do
        {
            DeviceInformation devInfo;
            devInfo.DeviceNumber = -1;
            devInfo.DeviceMode = ModeWrite;
            devInfo.ModuleIndex = 0;
            wcscpy(devInfo.Description, deviceDescription);
            _Ret = InstantDoCtrl_setSelectedDevice(_pInstantDoCtrl, &devInfo);
            CHK_RESULT(_Ret);
            _Ret = InstantDoCtrl_LoadProfile(_pInstantDoCtrl, _pProfilePath);
            CHK_RESULT(_Ret);
            dataBuffer[0] = u8PinMask;
            _Ret = InstantDoCtrl_WriteAny(_pInstantDoCtrl, i32PortNum,
                                         1, dataBuffer);
            CHK_RESULT(_Ret);
        }while (FALSE);

        if (BioFailed(_Ret))
        {
            AdxEnumToString(L"ErrorCode", static_cast<int32>(_Ret),
                                                             256,
                                                             enumString);
            printf("Some error occurred. The last error code is 0x%X. [%ls]\n",
                    _Ret, enumString);
            return false;
        }
    }
    else
    {
      return false;
    }
    return true;
}

uint8_t IoCard::u8ReadPort(int32 readPort)
{
    if ((_eSelMode&INPUT) == INPUT)
    {
        int i;
        byte dataBuffer[64] = {0};
        do
        {
            DeviceInformation devInfo;
            devInfo.DeviceNumber = -1;
            devInfo.DeviceMode = ModeWrite;
            devInfo.ModuleIndex = 0;
            wcscpy(devInfo.Description, deviceDescription);
            _Ret = InstantDiCtrl_setSelectedDevice(_pInstantDiCtrl, &devInfo);
            CHK_RESULT(_Ret);
            _Ret = InstantDiCtrl_LoadProfile(_pInstantDiCtrl, _pProfilePath);
            CHK_RESULT(_Ret);
            _Ret = InstantDiCtrl_ReadAny(_pInstantDiCtrl, readPort,
                                         1, dataBuffer);
            CHK_RESULT(_Ret);
        } while (FALSE);
        return dataBuffer[0];
    }
    else
    {
        // Wrong mode selected
    }
}

void IoCard::vSetPinMask(uint8_t u8PinNum, bool bState)
{
    uint16_t u16PinVal = 0x0001;
    if (bState)
    {
        _u16OutMask |= (u16PinVal << u8PinNum);
    }
    else
    {
        _u16OutMask &= ~(u16PinVal << u8PinNum);
    }
}

uint8_t IoCard::u8GetPinMask(uint8_t u8Port)
{
    if (u8Port == PORT_0)
    {
        return (_u16OutMask&(0x00ff));
    }
    if (u8Port == PORT_1)
    {
        return (_u16OutMask>>8)&(0x00ff);
    }
}
