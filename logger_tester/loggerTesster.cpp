// logger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
//#include <stringapiset.h>
#include <windows.h>
#include "libLogger.h"
using namespace std;
int main()
{
	CLogger* logger = CLogger::CreateLogger(L"test.log");
	const char* instruction = "\nChoose from the options:"
								"\n\t1. To quit the test"
								"\n\t2. To procceed with the test"
								"\n\t3. To force dump the precceeding log";						
	const char* instruction_2 = "\n type in the file path:\n\t";
	unsigned int capLogItem = 1024;
	wchar_t* logItem = (wchar_t*)malloc(capLogItem * sizeof(wchar_t));
	while (1)
	{
		cout << instruction << endl;
		int command;
		cin >> command;
		if (1 == command)
			break;
		else if (2 == command)
		{
			cout << instruction_2;
			char path[_MAX_PATH] = { 0 };
			cin >> path;
			//read the file content in line, and then write it to the log file
			cout << "read the file content from " << path
				<< " in line, and then write it to the log file:" << endl;

			ifstream ifs(path);
			
			while (!ifs.eof())
			{
				string strLine;
				getline(ifs, strLine);
				strLine += "\r\n";
				const char* cLine = strLine.c_str();
				unsigned int nc = strLine.length();
				int capReq = MultiByteToWideChar(CP_ACP
					, MB_PRECOMPOSED
					, cLine
					, nc
					, NULL
					, 0);
				if (capLogItem < capReq)
				{
					capLogItem = capReq;
					logItem = (wchar_t *)realloc(logItem, capLogItem * sizeof(wchar_t));
				}
				MultiByteToWideChar(CP_ACP
					, MB_PRECOMPOSED
					, cLine
					, nc
					, logItem
					, capLogItem);
				//wprintf(logItem);
				logger->Logout(logItem, capReq);
			}
		}
		else
		{
			cout << "force to dump the log..." << endl;
			logger->Dump();
		}
	}
	free(logItem);
	CLogger::DeleteLogger(logger);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
