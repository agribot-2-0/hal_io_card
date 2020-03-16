#include <hal_iocard/IoCard.h>

#define  deviceDescription  L"PCIe-1730,BID#8"

IoCard::IoCard()
{
    this->profilePath = L"../DemoDevice.xml";
    this->ret = Success;
    // Step 1: Create a instantDoCtrl for DO function.
    this->instantDoCtrl = InstantDoCtrl_Create();
}

IoCard::~IoCard()
{
}

void IoCard::vWriteOutputs(int32 startPort, int i, uint8_t outputMask)
{
    ret = Success;
    wchar_t enumString[256];
    byte dataBuffer[64] = {0}; //the first element is used for start port
    int32 portCount = 1;
    do
    {
        // Step 2: Select a device by device number or device description and specify the access mode.
        // in this example we use ModeWrite mode so that we can fully control the device, including configuring, sampling, etc.
        DeviceInformation devInfo;
        devInfo.DeviceNumber = -1;
        devInfo.DeviceMode = ModeWrite;
        devInfo.ModuleIndex = 0;
        wcscpy(devInfo.Description, deviceDescription);
        ret = InstantDoCtrl_setSelectedDevice(instantDoCtrl, &devInfo);
        CHK_RESULT(ret)
        ret = InstantDoCtrl_LoadProfile(instantDoCtrl, profilePath);//Loads a profile to initialize the device.
        CHK_RESULT(ret);
        dataBuffer[startPort - startPort] = outputMask;
        ret = InstantDoCtrl_WriteAny(instantDoCtrl, startPort, portCount, dataBuffer);
        CHK_RESULT(ret);
        printf("\n DO output completed !");
        }while (FALSE);
        
    if (BioFailed(ret))
    {
        AdxEnumToString(L"ErrorCode", (int32)ret, 256, enumString);
        printf("Some error occurred. And the last error code is 0x%X. [%ls]\n", ret, enumString);
    }
    //Close device and release any allocated resource.
    InstantDoCtrl_Dispose(this->instantDoCtrl);
}

uint8_t IoCard::u8ReadInputs()
{
    int32    startPort = 0;
    int32    portCount = 1;
    int i;
    InstantDiCtrl *instantDiCtrl = InstantDiCtrl_Create();
    byte dataBuffer[64] = {0}; //the first element is used for start port
	do
	{
		// Step 2: select a device by device number or device description and specify the access mode.
		// in this example we use ModeWrite mode so that we can fully control the device, including configuring, sampling, etc.
		DeviceInformation devInfo;
		devInfo.DeviceNumber = -1;
		devInfo.DeviceMode = ModeWrite;
		devInfo.ModuleIndex = 0;
		wcscpy(devInfo.Description, deviceDescription);
		ret = InstantDiCtrl_setSelectedDevice(instantDiCtrl, &devInfo);
		CHK_RESULT(ret);
      ret = InstantDiCtrl_LoadProfile(instantDiCtrl, profilePath);//Loads a profile to initialize the device.
      CHK_RESULT(ret);

		// Step 3: Read DI ports' status and show.
		printf(" Reading ports' status is in progress, any key to quit !\n\n");
		do 
		{
			ret = InstantDiCtrl_ReadAny(instantDiCtrl, startPort, portCount, dataBuffer);
  			CHK_RESULT(ret);
			//Show ports' status
			for (i = startPort; i < startPort + portCount; ++i)
			{
				printf(" DI port %d status is: 0x%X\n\n", i, dataBuffer[i - startPort]);
            //printf(" DI port %d status is: 0x%X\n\n", i, data);//for 'InstantDiCtrl_ReadBit'
			}
			SLEEP(1);
		} while (!kbhit());
	} while (FALSE);

  	// Step 4: Close device and release any allocated resource.
	InstantDiCtrl_Dispose(instantDiCtrl);
}