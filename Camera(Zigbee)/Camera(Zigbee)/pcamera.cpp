#include "pcamera.h"

PCamera::PCamera()
{


}

bool PCamera::ConnectToCamera(unsigned int camFlag)
{
		BusManager busMgr;
		unsigned int numCameras;
		m_Error = busMgr.GetNumOfCameras(&numCameras);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		if (numCameras < 1)
		{
			printf("Insufficient number of cameras... exiting\n");
			return -1;
		}
		// Connect to a camera
		PGRGuid guid;
		m_Error = busMgr.GetCameraFromIndex(camFlag, &guid);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		m_Error = m_Cam.Connect(&guid);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		//check the permission of current directory
		FILE* tempFile = fopen("test.txt", "w+");
		if (tempFile == NULL)
		{
			printf("Failed to create file in current folder.  Please check permissions.\n");
			return -1;
		}
		fclose(tempFile);
		remove("test.txt");
		// Get the camera information
		CameraInfo camInfo;
		m_Error = m_Cam.GetCameraInfo(&camInfo);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		PrintCameraInfo(&camInfo);
		// Check for external trigger support
		TriggerModeInfo triggerModeInfo;
		m_Error = m_Cam.GetTriggerModeInfo(&triggerModeInfo);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		if (triggerModeInfo.present != true)
		{
			printf("Camera does not support external trigger! Exiting...\n");
			return -1;
		}
		// Get current trigger settings
		m_Error = m_Cam.GetTriggerMode(&m_TriggerMode);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}

		// Set camera to trigger mode
		m_TriggerMode.onOff = true;
		m_TriggerMode.mode = 0;
		m_TriggerMode.parameter = 0;
		m_TriggerMode.source = 0; // Triggering the camera externally using source
		m_TriggerMode.polarity = 1; // Specifies the trigger signal polarity as an active high (rising edge) signal
		m_Error = m_Cam.SetTriggerMode(&m_TriggerMode);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		// Get the camera configuration
		FC2Config config;
		m_Error = m_Cam.GetConfiguration(&config);
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}

		// Camera is ready, start capturing images
		////////////////初始化相机参数/////////////////////////
		//关闭相机自动曝光
		Property prop; //Declare a Property struct.
		prop.type = SHUTTER; //Define the property to adjust.
		prop.onOff = true; //Ensure the property is on.
		prop.autoManualMode = false; //Ensure auto-adjust mode is off.
		prop.absControl = true; //Ensure the property is set up to use absolute value control.
		prop.absValue = 3; //Set the absolute value.
		m_Error = m_Cam.SetProperty(&prop); //Set the property.
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		//关掉自动增益模式
		prop.type = GAIN; //Define the property to adjust.
		prop.onOff = true; //Ensure the property is on.
		prop.autoManualMode = false; //Ensure auto-adjust mode is off.
		m_Error = m_Cam.SetProperty(&prop); //Set the property.
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		prop.type = BRIGHTNESS; //Define the property to adjust.
		prop.onOff = true; //Ensure the property is on.
		prop.autoManualMode = false; //Ensure auto-adjust mode is off.
		m_Error = m_Cam.SetProperty(&prop); //Set the property.
		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		
		//相机开始拍摄
			m_Error = m_Cam.StartCapture();


		if (m_Error != PGRERROR_OK)
		{
			PrintError(m_Error);
			return -1;
		}
		return true;
}

bool PCamera::DisconnectCamera()
{
    printf( "\nStopping grabbing images...\n" );

    // Turn trigger mode off.
    m_TriggerMode.onOff = false;
    m_Error = m_Cam.SetTriggerMode( &m_TriggerMode );
    if (m_Error != PGRERROR_OK)
    {
        PrintError( m_Error );
        return -1;
    }

    // Stop capturing images
    m_Error = m_Cam.StopCapture();
    if (m_Error != PGRERROR_OK)
    {
        PrintError( m_Error );
        return -1;
    }

    // Disconnect the camera
    m_Error = m_Cam.Disconnect();
    if (m_Error != PGRERROR_OK)
    {
        PrintError( m_Error );
        return -1;
    }

    return 1;
}


void PCamera::PrintError( Error error )
{
    error.PrintErrorTrace();
}

void PCamera::PrintCameraInfo( CameraInfo* pCamInfo )
{
    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        pCamInfo->serialNumber,
        pCamInfo->modelName,
        pCamInfo->vendorName,
        pCamInfo->sensorInfo,
        pCamInfo->sensorResolution,
        pCamInfo->firmwareVersion,
        pCamInfo->firmwareBuildTime );
}

bool PCamera::FireSoftwareTrigger( Camera* pCam )
{
    const unsigned int k_softwareTrigger = 0x62C;
    const unsigned int k_fireVal = 0x80000000;
    Error m_Error;

    m_Error = pCam->WriteRegister( k_softwareTrigger, k_fireVal );
    if (m_Error != PGRERROR_OK)
    {
        PrintError( m_Error );
        return false;
    }

    return true;
}

void PCamera::GrabAPicture(Image& pgImage)
{
    // Fire software trigger
    bool retVal = FireSoftwareTrigger( &m_Cam );
    if ( !retVal )
    {
        printf("\nError firing software trigger!\n");
        return;
    }
    // Retrieve an image
    m_Error = m_Cam.RetrieveBuffer( &pgImage );
    if (m_Error != PGRERROR_OK)
    {
        PrintError( m_Error );
        return;
    }
    return;
}

bool PCamera::SetExposureTime(float ms)
{
	// Setting shutter exposure time
	Property prop; //Declare a Property struct.
	prop.type = SHUTTER; //Define the property to adjust.
	prop.onOff = true; //Ensure the property is on.
	prop.autoManualMode = false; //Ensure auto-adjust mode is off.
	prop.absControl = true; //Ensure the property is set up to use absolute value control.
	prop.absValue = ms; //Set the absolute value.
	m_Error = m_Cam.SetProperty(&prop); //Set the property.
	if (m_Error != PGRERROR_OK)
	{
		PrintError(m_Error);
		return -1;
	}
	return 1;
}
