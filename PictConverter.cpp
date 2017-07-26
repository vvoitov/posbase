#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool GetLineFromFile(ifstream &stream, string &str, bool addN = true)
{
	bool res = false;
	if (!stream.eof())
	{
		getline(stream, str);
		if ( addN && (stream.peek() != EOF) )
			str.append("\n");
		res = true;
	}
	return res;
}

int CompareStrings(string &str1, string &str2)
{
	int res = 0;
	int pos = str1.size() > str2.size() ? str2.size() : str1.size();
	for (int i = 0; i < pos, res == 0; ++i)
	{
		if ( (str1[i] == '_') || (str2[i] == '_') )
		{
			if (str1[i] != str2[i])
			{
				if (str1[i] == '_')
					res = -(i+1);
				else
					res = i+1;

			}
			continue;
		}
		if (str1[i] < str2[i])
			res = -(i+1);
		else if (str1[i] > str2[i])
			res = i+1;
	}
	return res;
}

int main()
{
	vector<string> excludedProjects;
	string tempString;
	// Read file of excluded projects
	string folderName = "*.bmp";
	ofstream outFile;
	ifstream inputFile;
	string curStr, prevStr, compStr;
	WIN32_FIND_DATAA tempData;
	LPWIN32_FIND_DATAA FindFileData = &tempData; 
	HANDLE hFind = INVALID_HANDLE_VALUE; 

	hFind = FindFirstFileA(folderName.c_str(), FindFileData);
	bool found;
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			vector<string> outString;
			found = false;
			if ( strstr(FindFileData->cFileName, ".bmp") )
				found = true;
			if (found)
			{
				cout << FindFileData->cFileName << endl;
				string fullFilename = FindFileData->cFileName;
				inputFile.open(fullFilename.c_str());
				inputFile.seekg(0);

				string outFilename = string(fullFilename.begin(), fullFilename.end() - 4);
				outFilename.append(".txt");
				outFile.open(outFilename.c_str());
				outFile.clear();

				unsigned char c;

				size_t pos = 0;
				while(!inputFile.eof() && pos < 0x3E)
				{
					c = inputFile.get();
					++pos;
				}
				while(!inputFile.eof())
				{
					pos = 0;
					string tempStr = "\"";
					while(!inputFile.eof() && pos < 16)
					{
						c = inputFile.get();
						c = ~c;
						tempStr.append("\\x", 2);
						unsigned char temp = c >> 4;
						if (temp < 10)
							tempStr.push_back(temp + '0');
						else
							tempStr.push_back(temp + 0x41 - 10);
						temp = c & 0x0F;
						if (temp < 10)
							tempStr.push_back(temp + '0');
						else
							tempStr.push_back(temp + 0x41 - 10);
						++pos;
					}
					tempStr.append("\"\n", 2);
					outString.push_back(tempStr);
				}

				vector<string>::reverse_iterator rit;
				for ( rit = outString.rbegin(); rit < outString.rend(); ++rit)
				{
					outFile.write((*rit).c_str(), (*rit).size());
				}
				inputFile.close();
				outFile.close();
			}
		}while(FindNextFileA(hFind, FindFileData) != 0);
	}
	FindClose(hFind);

	return 0;
}
