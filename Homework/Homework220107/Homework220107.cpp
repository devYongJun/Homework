#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 특정 문자로 string 분리
vector<string> SplitString(const string& str, const char delimiter)
{
	vector<string> retVector;
	string temp;

	for (char c : str)
	{
		if (c == delimiter)
		{
			retVector.push_back(temp);
			temp = "";
			continue;
		}
		temp += c;
	}

	if (temp.empty() == false)
	{
		retVector.push_back(temp);
	}
	return retVector;
}

// string이 숫자인지 체크 
bool IsNumberString(const string& str)
{
	for (char s : str)
	{
		if (isdigit(s) == false)
		{
			return false;
		}
	}
	return true;
}

// 유효한 IPv4 범위 검사
bool ValidRangeIp(const string& str)
{
	int num = stoi(str);
	return (num >= 0 && num <= 255);
}

// 유효한 포트범위 검사
bool ValidRangePort(const string& str)
{
	int num = stoi(str);
	return (num >= 0 && num < 65535);
}

// 특정 char 개수 리턴
int GetCharCount(const string& srcIp, const char findChar)
{
	int count = 0;

	for (char c : srcIp)
	{
		if (c == findChar)
		{
			count++;
		}
	}
	return count;
}

// 유효한 IPv4주소인지 체크
bool ValidIp(const string& src)
{
	if (GetCharCount(src, '.') != 3)
	{
		return false;
	}
	vector<string> split = SplitString(src, ':');
	string ip = split[0];

	vector<string> numbers = SplitString(ip, '.');
	for (string num : numbers)
	{
		if (IsNumberString(num) == false)
		{
			return false;
		}
		if (ValidRangeIp(num) == false)
		{
			return false;
		}
	}
	return true;
}



// IPv4와 Port번호를 받는다 
int main()
{
	string Ip;
	string Port;
	
	while (Ip.empty())
	{
		cout << "Ip주소를 입력해주세요" << endl;
		string input;
		cin >> input;

		if (ValidIp(input))
		{
			vector<string> splits = SplitString(input, ':');
			Ip = splits[0];

			if (splits.size() > 1)
			{
				string tempPort = splits[1];

				if (IsNumberString(tempPort) == false)
				{
					break;
				}
				if (ValidRangePort(tempPort) == false)
				{
					break;
				}
				Port = tempPort;
			}
		}
	}

	while (Port.empty())
	{
		cout << "Port번호를 입력해주세요" << endl;
		string inputPort;
		cin >> inputPort;

		if (IsNumberString(inputPort) == false)
		{
			continue;
		}
		if (ValidRangePort(inputPort) == false)
		{
			continue;
		}
		Port = inputPort;

	}

	cout << "Ip : " << Ip << endl;
	cout << "Port : " << Port << endl;
}
