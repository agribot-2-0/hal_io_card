#include <hal_iocard/IoCard.h>

#define  deviceDescription  L"PCIe-1730,BID#8"

IoCard::IoCard(eCardMode Mode)
{
    this->profilePath = L"../DemoDevice.xml";
    this->ret = Success;
    this->eSelMode = NOT_SELECTED;

    if((Mode&INPUT) == INPUT)
    {
        this->instantDiCtrl = InstantDiCtrl_Create();    
        std::cout << "IN init!" << std::endl;
        this->eSelMode = this->eSelMode | INPUT;
    }
    if((Mode&OUTPUT) == OUTPUT)
    {
        this->instantDoCtrl = InstantDoCtrl_Create();
        std::cout << "OUT init!" << std::endl;
        this->eSelMode = this->eSelMode | OUTPUT;
    }
}

IoCard::~IoCard()
{
    if((this->eSelMode&INPUT) == INPUT)
    {
        std::cout << "Close in!" << std::endl;
    	InstantDiCtrl_Dispose(instantDiCtrl);
    }
    if((this->eSelMode&OUTPUT) == OUTPUT)
    {
        std::cout << "Close out!" << std::endl;
        InstantDoCtrl_Dispose(this->instantDoCtrl);
    }
}

void IoCard::vWritePort(int32 i32PortNum, uint8_t u8PinMask)
{
    if(this->eSelMode == OUTPUT)
    {
        ret = Success;
        wchar_t enumString[256];
        byte dataBuffer[64] = {0}; //the first element is used for start port
        do
        {
            DeviceInformation devInfo;
            devInfo.DeviceNumber = -1;
            devInfo.DeviceMode = ModeWrite;
            devInfo.ModuleIndex = 0;
            wcscpy(devInfo.Description, deviceDescription);
            ret = InstantDoCtrl_setSelectedDevice(instantDoCtrl, &devInfo);
            CHK_RESULT(ret)
            ret = InstantDoCtrl_LoadProfile(instantDoCtrl, profilePath);
            CHK_RESULT(ret);
            dataBuffer[i32PortNum] = u8PinMask;
            ret = InstantDoCtrl_WriteAny(instantDoCtrl, i32PortNum, 1, dataBuffer);
            CHK_RESULT(ret);
        }while (FALSE);

        if (BioFailed(ret))
        {
            AdxEnumToString(L"ErrorCode", (int32)ret, 256, enumString);
            printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", ret, enumString);
        }
    }
    else
    {
        //Wrong mode
    }
}

uint8_t IoCard::u8ReadPort(int32 readPort)
{
    if(this->eSelMode == INPUT)
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
	        ret = InstantDiCtrl_setSelectedDevice(instantDiCtrl, &devInfo);
	        CHK_RESULT(ret);
            ret = InstantDiCtrl_LoadProfile(instantDiCtrl, profilePath);//Loads a profile to initialize the device.
            CHK_RESULT(ret);
	        printf(" Reading ports' status is in progress, any key to quit !\n\n");
	        ret = InstantDiCtrl_ReadAny(instantDiCtrl, readPort, 1, dataBuffer);
  	        CHK_RESULT(ret);
	    } while (FALSE);
        return dataBuffer[readPort];
    }
    else
    {
        //Wrong mode selected
    }
}