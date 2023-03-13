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
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

chrono::time_point<chrono::system_clock> start, stop;
chrono::duration<double> elapsed_seconds;

int idBase = 1;
int GetUniqueID();

int main()
{
	WSADATA wsaData;
	SOCKET ClientSocket;
	sockaddr_in SvrAddr;
	unsigned int uiSize = 0;
	vector<string> ParamNames;
	char Rx[128];

	int planeID = GetUniqueID();

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(27001);
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));

	string strInput;
	unsigned int l = 0;


	ifstream ifs("DataFile.txt");
	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			getline(ifs, strInput);

			if (l > 0) //if line is greater than 0, handle the values
			{
				size_t offset, preOffset;
				offset = preOffset = 0;
				unsigned int iParamIndex = 0;
				//while (offset != std::string::npos)
				//read everything after the date and parse the rest of the stuff
				while (iParamIndex != 10)
				{
					//Use proper TCP format for sending and receiving
					offset = strInput.find_first_of(',', preOffset + 1);
					string strTx = strInput.substr(preOffset + 1, offset - (preOffset + 1));
					//send paramname (eg. "ACCELERATION BODY X")
					start = chrono::system_clock::now();

					string packet = packetize(ParamNames[iParamIndex]);
					send(ClientSocket, packet.c_str(), (int)packet.length(), 0);

					stop = chrono::system_clock::now();
					elapsed_seconds += stop - start;
					//receives ACK from server
					start = chrono::system_clock::now();
					recv(ClientSocket, Rx, sizeof(Rx), 0);
					stop = chrono::system_clock::now();
					elapsed_seconds += stop - start;
					start = chrono::system_clock::now();

					packet = packetize(strTx);
					send(ClientSocket, packet.c_str(), (int)packet.length(), 0);
					stop = chrono::system_clock::now();
					elapsed_seconds += stop - start;
					start = chrono::system_clock::now();
					recv(ClientSocket, Rx, sizeof(Rx), 0);
					stop = chrono::system_clock::now();
					elapsed_seconds += stop - start;
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

			l++;

			ofstream MyFile("DataCommsClientLog.txt");

			// Write to the file
			MyFile << "Average time per transmission on Client (including sends & recieves):\n" << (elapsed_seconds.count() / 4.0);	//there are 4 transmissions
			MyFile << "\nTotal time of all transmissions on Client (including sends & recieves):\n" << (elapsed_seconds.count());
			// Close the file
			MyFile.close();
		}

	}
	ifs.close();

	/*

	//create filename with timestamp for logging
	time_t rawtime;
	time(&rawtime);
	string filename = ctime(&rawtime);
	filename.pop_back();
	filename += "-FileIO.txt";
	//replace colons in filename
	filename.replace(filename.begin() + 16, filename.end() - 18, "-");
	filename.replace(filename.begin() + 13, filename.end() - 21, "-");

	//calculate time spent initializing client
	int elapsedtime = chrono::duration_cast<chrono::microseconds>(endclient - startclient).count();

	//logging
	ofstream file(filename);

	if (file.is_open())
	{
		file << "Client initialization in microseconds: " << elapsedtime << endl;
		file.close();
	}
	else {
		// show message:
		std::cout << "Error opening file";
	}

	*/

	closesocket(ClientSocket);
	WSACleanup();

	return 1;
}

//reading from the file to determine size before reading
//from file? That could cause a drag.

int GetUniqueID()
{
	return idBase++;
}

string packetize(string s)
{
	int id = GetUniqueID();
	string packet = id + "|" + s;
	return packet;
}