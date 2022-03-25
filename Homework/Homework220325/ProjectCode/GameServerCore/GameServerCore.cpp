#include "PreCompile.h"
#include "GameServerCore.h"
#include <GameServerBase\GameServerDebug.h>
#include <GameServerBase\GameServerDirectory.h>
#include <sstream>
#include <tinyxml2.h>
#include <GameServerNet\ServerHelper.h>
#include <conio.h>

#include "DBQueue.h"
#include "NetQueue.h"

int GameServerCore::ServerPort;
std::string GameServerCore::DBHost;
int GameServerCore::DBPort;
std::string GameServerCore::DBName;
std::string GameServerCore::DBUser;
std::string GameServerCore::DBPW;

/// <summary>
/// 엑셉트 관련
/// </summary>
TCPListener GameServerCore::Listener_;
std::function<void(std::shared_ptr<TCPSession>)> GameServerCore::AcceptCallBack_;


GameServerCore::GameServerCore() 
{
}

GameServerCore::~GameServerCore() 
{
}

bool GameServerCore::CoreInit()
{
	GameServerDebug::Initialize();

	GameServerDebug::LogInfo("Server Init");


	GameServerDirectory Dir;
	Dir.MoveParent("ProjectCode");
	std::stringstream xmlStream;
	xmlStream << Dir.PathToPlusFileName("ServerConfig.xml");
	tinyxml2::XMLDocument Doc;

	Doc.LoadFile(xmlStream.str().c_str());

	if (true == Doc.Error())
	{
		GameServerDebug::LogErrorAssert("Server Config Xml Open Error\n");
		return false;
	}

	tinyxml2::XMLElement* Root = Doc.FirstChildElement("ServerApp");
	if (nullptr == Root)
	{
		return false;
	}

	{
		tinyxml2::XMLElement* ServerStart = Root->FirstChildElement("ServerStart");
		ServerPort = nullptr != ServerStart->FindAttribute("Port") ? std::stoi(ServerStart->FindAttribute("Port")->Value()) : -1;

		GameServerDebug::LogInfo("Server Config Info");

		{
			std::string Info = "ServerPort : " + std::to_string(ServerPort);
			GameServerDebug::LogInfo(Info);
		}
	}

	{
		// <DataBase Host = "127.0.0.1" Port = "3306" Name = "userver2" User = "root" PW = "1234" / >
		tinyxml2::XMLElement* ServerStart = Root->FirstChildElement("DataBase");
		DBHost = nullptr != ServerStart->FindAttribute("Host") ? ServerStart->FindAttribute("Host")->Value() : "";
		DBUser = nullptr != ServerStart->FindAttribute("User")->Value() ? ServerStart->FindAttribute("User")->Value() : "";
		DBPW = nullptr != ServerStart->FindAttribute("PW")->Value() ? ServerStart->FindAttribute("PW")->Value() : "";
		DBName = nullptr != ServerStart->FindAttribute("Name")->Value() ? ServerStart->FindAttribute("Name")->Value() : "";
		DBPort = nullptr != ServerStart->FindAttribute("Port")->Value() ? std::stoi(ServerStart->FindAttribute("Port")->Value()) : -1;

		GameServerDebug::LogInfo("DB Config Info");


		{
			std::string DBInfo = "DBHost : " + DBHost;
			GameServerDebug::LogInfo(DBInfo);
		}

		{
			std::string DBInfo = "DBUser : " + DBUser;
			GameServerDebug::LogInfo(DBInfo);
		}

		{
			std::string DBInfo = "DBPW : " + DBPW;
			GameServerDebug::LogInfo(DBInfo);
		}

		{
			std::string DBInfo = "DBName : " + DBName;
			GameServerDebug::LogInfo(DBInfo);
		}

		{
			std::string DBInfo = "DBPort : " + std::to_string(DBPort);
			GameServerDebug::LogInfo(DBInfo);
		}

		GameServerDebug::LogInfo("Server Config OK");
	}


	GameServerDebug::LogInfo("Server Config OK");

	DBQueue::Init();
	GameServerDebug::LogInfo("DB Thread Init OK");

	NetQueue::Init();
	GameServerDebug::LogInfo("Net Thread Init OK");

	ServerHelper::StartEngineStartUp();
	GameServerDebug::LogInfo("ServerHelper And GameServerDebug Init OK");

	return true;
}

bool GameServerCore::CoreEnd()
{
	DBQueue::Destroy();
	NetQueue::Destroy();
	GameServerDebug::Destroy();

	return true;
}

void GameServerCore::SetAcceptCallBack(const std::function<void(std::shared_ptr<TCPSession>)>& _CallBack)
{
	AcceptCallBack_ = _CallBack;
}

bool GameServerCore::CoreRun() 
{
	// 액셉트 콜백도 다 처리가 세팅을 해줬을거고.
	if (nullptr == AcceptCallBack_)
	{
		GameServerDebug::LogErrorAssert("AcceptCallBack_ is null");
		return false;
	}

	Listener_.Initialize(IPEndPoint(IPAddress::Parse("0.0.0.0"), ServerPort), AcceptCallBack_);

	Listener_.BindQueue(NetQueue::GetQueue());
	Listener_.StartAccept(10);

	while (true)
	{
		if ('`' == _getch())
		{
			return true;
		}
	}
	
}