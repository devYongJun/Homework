#pragma once
#include <string>
#include <GameServerNet\TCPListener.h>

// 설명 :
class TCPSession;
class GameServerCore
{
public:
	// constrcuter destructer
	GameServerCore();
	~GameServerCore();

	// delete Function
	GameServerCore(const GameServerCore& _Other) = delete;
	GameServerCore(GameServerCore&& _Other) noexcept = delete;
	GameServerCore& operator=(const GameServerCore& _Other) = delete;
	GameServerCore& operator=(GameServerCore&& _Other) noexcept = delete;

	static inline int GetServerPort()
	{
		return ServerPort;
	}
	static inline std::string GetDBHost()
	{
		return DBHost;
	}
	static inline int GetDBPort()
	{
		return DBPort;
	}
	static inline std::string GetDBName()
	{
		return DBName;
	}
	static inline std::string GetDBUser()
	{
		return DBUser;
	}
	static inline std::string GetDBPW()
	{
		return DBPW;
	}

	template<typename UserGameType, typename ... Types>
	static void Start(Types ... args)
	{
		CoreInit();

		UserGameType NewUserServer = UserGameType(args...);
		NewUserServer.UserStart();
		// 실제 기동을 위한.
		CoreRun();

		CoreEnd();
	}

protected:
	static int ServerPort;
	static std::string DBHost;
	static int DBPort;
	static std::string DBName;
	static std::string DBUser;
	static std::string DBPW;

	void SetAcceptCallBack(const std::function<void(std::shared_ptr<TCPSession>)>& _CallBack);


private:
	static bool CoreInit();
	static bool CoreRun();
	static bool CoreEnd();

	static TCPListener Listener_;
	static std::function<void(std::shared_ptr<TCPSession>)> AcceptCallBack_;
};

