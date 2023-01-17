#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

unsigned int GetSize();

int main()
{
	WSADATA wsaData;
	SOCKET ClientSocket;
	sockaddr_in SvrAddr;
	unsigned int uiSize = 0;
	vector<string> ParamNames;
	char Rx[128];

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(27001);
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));

	uiSize = GetSize(); //this is the number of lines in the datafile
	//in a loop, open the file, read one line, close it, repeat
	for (unsigned int l = 0; l < uiSize; l++)
	{
		//load in the text file and read it
		string strInput;
		ifstream ifs("DataFile.txt");
		for (unsigned int iStart = 0; iStart < l; iStart++)
			getline(ifs, strInput);

		getline(ifs, strInput);
		if (l > 0) //if line is greater than 0, handle the values
		{
			size_t offset, preOffset;
			offset = preOffset = 0;
			unsigned int iParamIndex = 0;
			//while (offset != std::string::npos)
			//read everything after the date and parse the rest of the stuff
			while(iParamIndex != 8)
			{
				//Use proper TCP format for sending and receiving
				offset = strInput.find_first_of(',', preOffset+1);
				string strTx = strInput.substr(preOffset+1, offset - (preOffset+1));
				//send paramname (eg. "ACCELERATION BODY X")
				send(ClientSocket, ParamNames[iParamIndex].c_str(), (int)ParamNames[iParamIndex].length(), 0);
				//receives ACK from server
				recv(ClientSocket, Rx, sizeof(Rx), 0);
				send(ClientSocket, strTx.c_str(), (int)strTx.length(), 0);
				recv(ClientSocket, Rx, sizeof(Rx), 0);
				cout << ParamNames[iParamIndex] << " Avg: " << Rx << endl;
				preOffset = offset;
				iParamIndex++;
			}
		}
		else //if line is the first line, handle the param names
		{
			//put the timestamp in vector
			ParamNames.push_back("TIME STAMP");
			size_t offset, preOffset;
			offset = 0;
			preOffset = -1;
			while (offset != std::string::npos)
			{
				offset = strInput.find_first_of(',', preOffset + 1);
				string newParam = strInput.substr(preOffset + 1, offset - (preOffset + 1));
				ParamNames.push_back(newParam); //add the param names to ParamNames vector
				preOffset = offset;
			}
		}
		ifs.close();
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 1;
}

//reading from the file to determine size before reading
//from file? That could cause a drag.

// Open the txt input data file and read line by line, incrementing the uiSize variable containing the number of lines of data per each iteration
unsigned int GetSize()
{
	string strInput;
	unsigned int uiSize = 0;
	ifstream ifs("DataFile.txt");
	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			getline(ifs, strInput);
			uiSize++;
		}
	}

	return uiSize;
}