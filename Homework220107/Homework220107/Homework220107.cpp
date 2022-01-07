#include <iostream>
#include <string>

int ExtractPort(const std::string& str)
{
	int findIdx = str.find(":");

	if (-1 == str.find(":"))
	{
		return -1;
	}

	std::string strPort = str.substr(findIdx + 1, str.size() - findIdx - 1);
	if (strPort.size() == 0)
	{
		return -1;
	}
	return stoi(strPort);
}

// string이 숫자인지 체크 
bool IsNumberString(const std::string& str)
{
	for (auto s : str)
	{
		if (std::isdigit(s) == false)
		{
			return false;
		}
	}
	return true;
}

// 유효한 범위체크
bool IsRangeIp4(const std::string& str)
{
	int num = std::stoi(str);
	return (num >= 0 && num <= 255);
}

// 유효한 IPv4인지 체크
// 문자나 공백은 지운후 ip값 수정
bool IsValidIp4(std::string& strIp)
{
	int index = 0;
	std::string ipNumbers[4];

	for (char i : strIp)
	{	
		if (i == '.')
		{
			index++;
			continue;
		}
		else if (i == ':')
		{
			break;
		}
		else if (std::isdigit(i) == false)
		{
			continue;
		}
		else if (index > 3)
		{
			std::cout << "!오류 : 유효한 아이피가 아닙니다" << std::endl;
			return false;
		}
		ipNumbers[index] += i;
	}

	for (std::string str : ipNumbers)
	{
		if (str.size() == 0)
		{
			std::cout << "오류 : 유효한 아이피가 아닙니다" << std::endl;
			return false;
		}
		if (IsNumberString(str) == false)
		{
			std::cout << "오류 : 아이피가 숫자가 아닙니다" << std::endl;
			return false;
		}
		if (IsRangeIp4(str) == false)
		{
			std::cout << "오류 : 아이피가 0~255 범위가 아닙니다" << std::endl;
			return false;
		}
	}

	strIp = "";
	for (int i = 0; i < 4; i++)
	{
		strIp += ipNumbers[i];
		if (i < 3)
		{
			strIp += ".";
		}
	}
	
	return true;
}

// 유효한 포트인지 체크
bool IsValidPort(int port)
{
	if (port < 0 || port > 65535)
	{
		std::cout << "오류 : 포트가 0~65535 범위가 아닙니다" << std::endl;
		return false;
	}
	return true;
}

int main()
{
	std::string Ip;
	int Port = -1;

	while (true)
	{
		std::cout << "Ip주소를 입력해주세요" << std::endl;
		std::cin >> Ip;

		if ("Q" == Ip
			|| "q" == Ip)
		{
			Ip = "127.0.0.1";
		}

		Port = ExtractPort(Ip);

		if (IsValidIp4(Ip))
		{	
			break;
		}
	}
	
	while (Port == -1)
	{
		std::cout << "Port번호를 입력해주세요" << std::endl;
		std::cin >> Port;

		if (IsValidPort(Port))
		{
			break;
		}
		Port = -1;
	}

	std::cout << "Ip : " << Ip << std::endl;
	std::cout << "Port : " << Port << std::endl;
}
