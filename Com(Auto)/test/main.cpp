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
	cout << "��ȷ�ϴ��ں�С��10" << endl;
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
	//	 Console::WriteLine("The following serial ports were found:");         //ע������ʾ��Ϣ

	// Display each port name to the console.
	for each(String^ port in serialPorts)
	{
		//	Console::WriteLine(port);  ��ʾ���ں�
		char* PORT = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(port);
		
		for (i = 1; i<10 ; i++)   // �ж�10���µ�COM��
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
						cout << "������Զ�ʶ��ɹ�" << endl;
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
					}//�ж��Ƿ���6
				}//����6
					mySerialPort.CloseListenTread();
			}//ÿ������if�ж�

		}//ÿ������ѭ����ѯ�ж�

	
	}//��ѯ���ҵ�ǰ��������
	cout << "ʶ��ʧ��" << endl;
	cout << "���ڼ��ȷ�ϴ��������Ƿ�װ��ɺ��ԣ�" << endl;
	cout << "���°β�USB�������������� " << endl;

	int temp;
	std::cin >> temp;//����Ϊ��ʹmain��������
	//cout << "�ֽ��� = " << mySerialPort.GetBytesInCOM() << endl;
	//char data;
	//cout << mySerialPort.ReadChar(data)<<endl;
	return 0;

}
