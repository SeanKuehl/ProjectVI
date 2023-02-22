/*
9 Receiving transmissions on Server
2 Receiving transmissions on Client

2 Sending transmissions on Server
2 Sending transmissions on Client

Total transmissions on Server = 11
Total transmissions on Client = 4

Total transmissions : 15
*/

#include <windows.networking.sockets.h>
#include <iostream>

#include <fstream>
#include <chrono>
#include <ctime>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

struct StorageTypes 
{ 
	unsigned int size = 0;
	float* pData;
};
StorageTypes RxData[7];

chrono::time_point<chrono::system_clock> start, stop;
chrono::duration<double> elapsed_seconds;

void UpdateData(unsigned int, float);
float CalcAvg(unsigned int);

int main()
{
	WSADATA wsaData;
	SOCKET ServerSocket, ConnectionSocket;
	char RxBuffer[128] = {};
	sockaddr_in SvrAddr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == SOCKET_ERROR)
		return -1;

	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27001);
	bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));

	if (ServerSocket == SOCKET_ERROR)
		return -1;

	listen(ServerSocket, 1);
	cout << "Waiting for client connection\n" << endl;
	ConnectionSocket = SOCKET_ERROR;
	ConnectionSocket = accept(ServerSocket, NULL, NULL);

	if (ConnectionSocket == SOCKET_ERROR)
		return -1;

	cout << "Connection Established" << endl;

	while (RxBuffer[0] != '*')
	{
		float fValue = 0;
		memset(RxBuffer, 0, sizeof(RxBuffer));
		start = chrono::system_clock::now();
		recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0); 
		stop = chrono::system_clock::now();
		elapsed_seconds += stop - start;

		start = chrono::system_clock::now();
		send(ConnectionSocket, "ACK", sizeof("ACK"), 0); 
		stop = chrono::system_clock::now();
		elapsed_seconds += stop - start;

		if (strcmp(RxBuffer, "ACCELERATION BODY X") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(0, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(0);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "ACCELERATION BODY Y") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(1, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(1);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "ACCELERATION BODY Z") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(2, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(2);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "TOTAL WEIGHT") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(3, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(3);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "PLANE ALTITUDE") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0); 
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(4, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(4);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "ALTITUDE INDICATOR PICTH DEGREES") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(5, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(5);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "ALTITUDE INDICATOR BANK DEGREES") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(6, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(6);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "ELAPSED FLIGHT TIME") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(7, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(7);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else if (strcmp(RxBuffer, "FUEL ONBOARD") == 0)
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			size_t result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = (float)atof(RxBuffer);
			start = chrono::system_clock::now();
			UpdateData(8, fValue);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;


			auto startTime = chrono::high_resolution_clock::now();

			fValue = CalcAvg(8);
			auto stopTime = chrono::high_resolution_clock::now();
		}
		else
		{
			memset(RxBuffer, 0, sizeof(RxBuffer));
			start = chrono::system_clock::now();
			recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			stop = chrono::system_clock::now();
			elapsed_seconds += stop - start;
			fValue = 0.0;
		}

		char Tx[128];
		sprintf_s(Tx, "%f", fValue);
		start = chrono::system_clock::now();
		send(ConnectionSocket, Tx, sizeof(Tx), 0);
		stop = chrono::system_clock::now();
		elapsed_seconds += stop - start;

		ofstream MyFile("DataCommsServerLog.txt");

		// Write to the file
		MyFile << "Average time per transmission on Server (including sends & recieves):\n" << (elapsed_seconds.count() / 11.0);	//there are 11 transmissions
		MyFile << "\nTotal time of all transmissions on Server (including sends & recieves):\n" << (elapsed_seconds.count());

		// Close the file
		MyFile.close();

		ofstream logFile("CalculateAverageOfDataLog.txt");

		logFile << "Average time per calculation: " << (elapsed_seconds.count() / 7);
		logFile << "Total calculations: 7";
		logFile << "Data type: Float";
		logFile.close();

		
    
		ofstream MyUpdateFile("UpdateDataLogtxt.txt");

		// Write to the file
		MyUpdateFile << "Average elapsed UpdateData time: " << (elapsed_seconds.count() / 7.0);	//there are 7 updates
    // Close the file
		MyUpdateFile.close();


	}

	closesocket(ConnectionSocket);	//closes incoming socket
	closesocket(ServerSocket);	    //closes server socket	
	WSACleanup();					//frees Winsock resources

	return 1;
}

void UpdateData(unsigned int uiIndex, float value)
{
	if (RxData[uiIndex].size == 0)
	{
		RxData[uiIndex].pData = new float[1];
		RxData[uiIndex].pData[0] = value;
		RxData[uiIndex].size = 1;
	}
	else
	{
		float* pNewData = new float[RxData[uiIndex].size + 1];
		for (unsigned int x = 0; x < RxData[uiIndex].size; x++)
			pNewData[x] = RxData[uiIndex].pData[x];

		pNewData[RxData[uiIndex].size] = value;
		delete[] RxData[uiIndex].pData;
		RxData[uiIndex].pData = pNewData;
		RxData[uiIndex].size++;
	}
}

float CalcAvg(unsigned int uiIndex)
{
	float Avg = 0;
	for (unsigned int x = 0; x < RxData[uiIndex].size; x++)
		Avg += RxData[uiIndex].pData[x];

	Avg = Avg / RxData[uiIndex].size;
	return Avg;
}