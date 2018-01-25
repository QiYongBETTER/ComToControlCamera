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
	int portnumber;
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
		if (PORT[3] == '1')
		{
			portnumber = 1;
		}
		else if (PORT[3] == '2')
		{
			portnumber = 2;
		}
		else if (PORT[3] == '3')
		{
			portnumber = 3;
		}
		else if (PORT[3] == '4')
		{
			portnumber = 4;
		}
		else if (PORT[3] == '5')
		{
			portnumber = 5;
		}
		else if (PORT[3] == '6')
		{
			portnumber = 6;
		}
		else if (PORT[3] == '7')
		{
			portnumber = 7;
		}
		else if (PORT[3] == '8')
		{
			portnumber = 8;
		}
		else if (PORT[3] == '9')
		{
			portnumber = 9;
		}
		else if (PORT[3] == '10')
		{
			portnumber = 10;
		}
		else if (PORT[3] == '11')
		{
			portnumber = 11;
		}
		else if (PORT[3] == '12')
		{
			portnumber = 12;
		}
		else if (PORT[3] == '13')
		{
			portnumber = 13;
		}
		else if (PORT[3] == '14')
		{
			portnumber = 14;
		}
		else if (PORT[3] == '15')
		{
			portnumber = 15;
		}
		else
		{
			cout << "error" << endl;
		}
	}
//以下读取串口信息
	CSerialPort mySerialPort;

	//也可以只调用这两个函数
	/*mySerialPort.InitPort(5);
	mySerialPort.OpenListenThread();*/
	
	if (!mySerialPort.InitPort(portnumber))//此处写入串口号
	{
		std::cout << "initPort fail !" << std::endl;
	}
	else
	{
		std::cout << "initPort success !" << std::endl;
	}

	if (!mySerialPort.OpenListenThread())
	{
		std::cout << "OpenListenThread fail !" << std::endl;
	}
	else
	{
		std::cout << "OpenListenThread success !" << std::endl;
	}
/*	if (!mySerialPort.CloseListenTread())
	{
		std::cout << "CloseListenTread fail !" << std::endl;
	}
	else
	{
		std::cout << "CloseListenTread success !" << std::endl;
	}
	*/

	while (1){
		Sleep(10);

		if (mySerialPort.button1)
		{
			cout << "buttion1" << endl;
			mySerialPort.button1 = !mySerialPort.button1;
		}

		if (mySerialPort.button2)
		{
			cout << "buttion2" << endl;
			mySerialPort.button2 = !mySerialPort.button2;
		}

		if (mySerialPort.button3)
		{
			cout << "buttion3" << endl;
			mySerialPort.button3 = !mySerialPort.button3;
		}

		if (mySerialPort.button4)
		{
			cout << "buttion4" << endl;
			mySerialPort.button4 = !mySerialPort.button4;
		}

		if (mySerialPort.button5)
		{
			cout << "buttion5" << endl;
			mySerialPort.button5 = !mySerialPort.button5;
		}
	}

	
	int temp;
	std::cin >> temp;//作用为不使main函数结束
	//cout << "字节数 = " << mySerialPort.GetBytesInCOM() << endl;
	//char data;
	//cout << mySerialPort.ReadChar(data)<<endl;
	return 0;

}
