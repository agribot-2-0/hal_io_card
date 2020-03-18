#include <hal_iocard/IoCard.h>

#define  deviceDescription  L"PCIe-1730,BID#8"

IoCard::IoCard(eCardMode eMode)
{
    this->Ret = Success;
    this->eSelMode = NOT_SELECTED;

    if ((eMode&INPUT) == INPUT)
    {
        this->pInstantDiCtrl = InstantDiCtrl_Create();
        this->eSelMode = this->eSelMode | INPUT;
    }
    if ((eMode&OUTPUT) == OUTPUT)
    {
        this->pInstantDoCtrl = InstantDoCtrl_Create();
        this->eSelMode = this->eSelMode | OUTPUT;
    }
}

IoCard::~IoCard()
{
    if ((this->eSelMode&INPUT) == INPUT)
    {
        InstantDiCtrl_Dispose(pInstantDiCtrl);
    }
    if ((this->eSelMode&OUTPUT) == OUTPUT)
    {
        InstantDoCtrl_Dispose(this->pInstantDoCtrl);
    }
}

void IoCard::vWritePort(int32 i32PortNum, uint8_t u8PinMask)
{
    if (this->eSelMode == OUTPUT)
    {
        Ret = Success;
        wchar_t enumString[256];
        byte dataBuffer[64] = {0};  // the first element is used for start port
        do
        {
            DeviceInformation devInfo;
            devInfo.DeviceNumber = -1;
            devInfo.DeviceMode = ModeWrite;
            devInfo.ModuleIndex = 0;
            wcscpy(devInfo.Description, deviceDescription);
            Ret = InstantDoCtrl_setSelectedDevice(pInstantDoCtrl, &devInfo);
            CHK_RESULT(Ret);
            Ret = InstantDoCtrl_LoadProfile(pInstantDoCtrl, pProfilePath);
            CHK_RESULT(Ret);
            dataBuffer[i32PortNum] = u8PinMask;
            Ret = InstantDoCtrl_WriteAny(pInstantDoCtrl, i32PortNum,
                                         1, dataBuffer);
            CHK_RESULT(Ret);
        }while (FALSE);

        if (BioFailed(Ret))
        {
            AdxEnumToString(L"ErrorCode", static_cast<int32>(Ret),
                                                             256,
                                                             enumString);
            printf("Some error occurred. The last error code is 0x%X. [%ls]\n",
                    Ret, enumString);
        }
    }
    else
    {
        // Wrong mode
    }
}

uint8_t IoCard::u8ReadPort(int32 readPort)
{
    if (this->eSelMode == INPUT)
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
            Ret = InstantDiCtrl_setSelectedDevice(pInstantDiCtrl, &devInfo);
            CHK_RESULT(Ret);
            Ret = InstantDiCtrl_LoadProfile(pInstantDiCtrl, pProfilePath);
            CHK_RESULT(Ret);
            Ret = InstantDiCtrl_ReadAny(pInstantDiCtrl, readPort,
                                        1, dataBuffer);
            CHK_RESULT(Ret);
        } while (FALSE);
        return dataBuffer[readPort];
    }
    else
    {
        // Wrong mode selected
    }
}
