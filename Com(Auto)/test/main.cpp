#include <iostream>  
#include <cassert>  
#include "Com.h"
#include "stdafx.h"
#include <string>
#using <System.dll>
using namespace System;
using namespace System::IO::Ports;
using namespace System::ComponentModel;
using namespace std;

int main()
{
	CSerialPort mySerialPort;
	//int portnumber=0;
	int i;
	int j;
	cout << "请确认串口号小于10" << endl;
	array<String^>^ serialPorts = nullptr;
	try
	{
		// Get a list of serial port names.
		serialPorts = SerialPort::GetPortNames();
	}
	catch (Win32Exception^ ex)
	{
		Console::WriteLine(ex->Message);
	}
	//	 Console::WriteLine("The following serial ports were found:");         //注释了提示信息

	// Display each port name to the console.
	for each(String^ port in serialPorts)
	{
		//	Console::WriteLine(port);  显示串口号
		char* PORT = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(port);
		
		for (i = 1; i<10 ; i++)   // 判断10以下的COM口
		{
			if (PORT[3] == i + '0'&& PORT[4] == NULL)
			{
				mySerialPort.InitPort(i);
				mySerialPort.OpenListenThread();
				for (j = 0; j < 100; j++)
				//while (1)
				{
					Sleep(5);
					if (mySerialPort.button6)
					{
						mySerialPort.button6 = !mySerialPort.button6;
						cout << "相机端自动识别成功" << endl;
						cout << "COM " << i << endl;
						while (1)
						{
							Sleep(10);

							if (mySerialPort.button1)
							{
								cout << "button1" << endl;
								mySerialPort.button1 = !mySerialPort.button1;
							}

							if (mySerialPort.button2)
							{
								cout << "button2" << endl;
								mySerialPort.button2 = !mySerialPort.button2;
							}

							if (mySerialPort.button3)
							{
								cout << "button3" << endl;
								mySerialPort.button3 = !mySerialPort.button3;
							}

							if (mySerialPort.button4)
							{
								cout << "button4" << endl;
								mySerialPort.button4 = !mySerialPort.button4;
							}

							if (mySerialPort.button5)
							{
								cout << "button5" << endl;
								mySerialPort.button5 = !mySerialPort.button5;
							}
						}
						break;
					}//判断是否有6
				}//接收6
					mySerialPort.CloseListenTread();
			}//每个串口if判断

		}//每个串口循环轮询判断

	
	}//轮询查找当前工作串口
	cout << "识别失败" << endl;
	cout << "请在检查确认串口驱动是否安装完成后尝试：" << endl;
	cout << "重新拔插USB或重新启动程序 " << endl;

	int temp;
	std::cin >> temp;//作用为不使main函数结束
	//cout << "字节数 = " << mySerialPort.GetBytesInCOM() << endl;
	//char data;
	//cout << mySerialPort.ReadChar(data)<<endl;
	return 0;

}
