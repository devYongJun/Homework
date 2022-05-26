﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameInstance.h"
#include "Sockets.h"
#include "SocketSubSystem.h"
#include "Networking.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "IPAddress.h"
#include "../Message/MessageConverter.h"
#include "../Message/Handler/ThreadHandlerServerDestroyMessage.h"
#include "../UnrealClient.h"

/// TcpRecvThread

UnrealRecvThread::UnrealRecvThread(ISocketSubsystem* _SocketSubSystem, FSocket* _RecvSocket, TQueue<std::shared_ptr<GameServerMessage>>* _MessageQueue)
{
	if (nullptr == _RecvSocket)
	{
		UE_LOG(ClientLog, Error, TEXT("RecvSocket Error"));
	}

	SocketSubSystem_ = _SocketSubSystem;

	RecvSocket_ = _RecvSocket;
	MessageQueue_ = _MessageQueue;
	IsAppClose_ = true;
}

bool UnrealRecvThread::FunctionTest() 
{
	return false;
}

uint32 UnrealRecvThread::Run() 
{
	UE_LOG(ClientLog, Log, TEXT("Recv Start"));

	while (IsAppClose_)
	{
		std::vector<uint8> RecvData;
		RecvData.resize(1024);
		int32 RecvDataSize_ = 0;

		if (false == RecvSocket_->Recv(&RecvData[0], RecvData.size(), RecvDataSize_))
		{
			auto Result = RecvSocket_->GetConnectionState();

			switch (Result)
			{
			case SCS_NotConnected:
				break;
			case SCS_Connected:
			{
				ServerDestroyMessage SDM;
				GameServerSerializer Sr;
				SDM.Serialize(Sr);
				MessageConverter Converter = MessageConverter(Sr.GetData());
				MessageQueue_->Enqueue(Converter.GetServerMessage());
				return 0;
			}
			case SCS_ConnectionError:
				break;
			default:
				break;
			}

			break;
		}

		MessageConverter Converter = MessageConverter(RecvData);
		MessageQueue_->Enqueue(Converter.GetServerMessage());
	}

	return 0;
}

void UnrealRecvThread::Close() 
{
	IsAppClose_ = false;
}

/// UDPRecvThread


UnrealUDPRecvThread::UnrealUDPRecvThread(ISocketSubsystem* _SocketSubSystem, FSocket* _RecvSocket, TQueue<std::shared_ptr<GameServerMessage>>* _MessageQueue)
{
	if (nullptr == _RecvSocket)
	{
		UE_LOG(ClientLog, Error, TEXT("RecvSocket Error"));
	}

	SocketSubSystem_ = _SocketSubSystem;
	RecvSocket_ = _RecvSocket;
	MessageQueue_ = _MessageQueue;
	IsAppClose_ = true;
}

bool UnrealUDPRecvThread::FunctionTest()
{
	return false;
}

uint32 UnrealUDPRecvThread::Run()
{
	UE_LOG(ClientLog, Log, TEXT("Recv Start"));

	FIPv4Endpoint ConnectAddress_ = FIPv4Endpoint();

	TSharedRef<FInternetAddr> Ref = ConnectAddress_.ToInternetAddr();

	int32 RecvByte;

	std::vector<uint8> RecvData;
	RecvData.resize(1024);
	int32 RecvDataSize_ = 0;

	while (IsAppClose_)
	{
		if (false == RecvSocket_->RecvFrom(&RecvData[0], RecvData.size(), RecvByte, Ref.Get(), ESocketReceiveFlags::Type::None))
		{
			ESocketErrors Error2 = SocketSubSystem_->GetLastErrorCode();
 			FString Debug = RecvSocket_->GetDescription();

			UE_LOG(ClientLog, Error, TEXT("%d"), Error2);

			break;
		}

		MessageConverter Converter = MessageConverter(RecvData);
		MessageQueue_->Enqueue(Converter.GetServerMessage());
	}

	return 0;
}

void UnrealUDPRecvThread::Close()
{
	IsAppClose_ = false;
}

UClientGameInstance::UClientGameInstance()
{
	ClientMode_ = false;
	LoginProcess_ = false;
	RankWindowMode = ESlateVisibility::Hidden;
}

UClientGameInstance::~UClientGameInstance() 
{
	if (nullptr != RecvThread_)
	{
		RecvThread_->Exit();
		RecvThread_ = nullptr;
	}
}

void UClientGameInstance::Close() 
{
	if (nullptr != RecvThread_)
	{
		RecvThread_->Close();
	}

	if (nullptr == TCPSocket_)
	{
		return;
	}


	LoginProcess_ = false;
	TCPSocket_->Close();
	TCPSocket_ = nullptr;

	if (nullptr == UDPSocket_)
	{
		return;
	}
	UDPSocket_->Close();
	UDPSocket_ = nullptr;
}

bool UClientGameInstance::ServerConnect(const FString& _IPString, const FString& _PORTString)
{
	if (false == IsThreadCheck())
	{
		return false;
	}

	Close();

	SocketSubSystem_ = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	TCPSocket_ = SocketSubSystem_->CreateSocket(NAME_Stream, TEXT("Test"), false);

	if (nullptr == TCPSocket_)
	{
		UE_LOG(ClientLog, Error, TEXT("if (nullptr == NewSocket)"));
		return false;
	}

	TCPSocket_->SetNoDelay(true);

	FIPv4Address::Parse(_IPString, ConnectAddress_);
	Port_ = static_cast<uint16>(FCString::Atoi(*_PORTString));
	EndPoint_ = FIPv4Endpoint(ConnectAddress_, Port_);

	if (false == TCPSocket_->Connect(EndPoint_.ToInternetAddr().Get()))
	{
		SocketSubSystem_ = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		FString ErrorText = SocketSubSystem_->GetSocketError(SocketSubSystem_->GetLastErrorCode());
		UE_LOG(ClientLog, Error, TEXT("Server Error : %s"), *ErrorText);
		TCPSocket_->Close();
		TCPSocket_ = nullptr;
		return false;
	}

	RecvThread_ = new UnrealRecvThread(SocketSubSystem_, TCPSocket_, &MessageQueue_);
	ThreadRunalbe_ = FRunnableThread::Create(RecvThread_, TEXT("Recv Thread"));

	return true;
}

bool UClientGameInstance::ServerUDPConnect(const FString& _PORTString, uint64 _ServerSendUDP)
{
	if (nullptr == SocketSubSystem_)
	{
		return true;
	}

	if (nullptr != UDPSocket_)
	{
		return true;
	}

	ServerSendUDPPort_ = _ServerSendUDP;

	ServerSendUDPEndPoint_ = FIPv4Endpoint(ConnectAddress_, ServerSendUDPPort_);


	FIPv4Address::Parse(TEXT("0.0.0.0"), UDPAddress_);

	ClientRecvUDPPort_ = static_cast<uint16>(FCString::Atoi(*_PORTString));
	ClientRecvUDPEndPoint_ = FIPv4Endpoint(UDPAddress_, ClientRecvUDPPort_);

	UDPSocket_ = SocketSubSystem_->CreateSocket(NAME_DGram, TEXT("Test"));

	if (nullptr == UDPSocket_)
	{
		UE_LOG(ClientLog, Error, TEXT("UDP Socket Create Error"));
		return false;
	}

	while (false == UDPSocket_->Bind(ClientRecvUDPEndPoint_.ToInternetAddr().Get()))
	{
		ClientRecvUDPEndPoint_ = FIPv4Endpoint(ConnectAddress_, ++ClientRecvUDPPort_);
	}

	UDPRecvThread_ = new UnrealUDPRecvThread(SocketSubSystem_, UDPSocket_, &MessageQueue_);
	UDPThreadRunalbe_ = FRunnableThread::Create(UDPRecvThread_, TEXT("Recv Thread"));

	return true;
}

bool UClientGameInstance::IsThreadCheck() 
{
	if (false == FPlatformProcess::SupportsMultithreading())
	{
		UE_LOG(ClientLog, Error, TEXT("Is Not Support Multithreading"));
		return false;
	}

	return true;
}

bool UClientGameInstance::Send(const std::vector<uint8>& _Data, int _Size)
{
	if (nullptr == TCPSocket_)
	{
		return false;
	}

	if (0 == _Data.size())
	{
		return false;
	}

	int32 DataSendSize = 0;

	return TCPSocket_->Send(&_Data[0], _Size, DataSendSize);
}

bool UClientGameInstance::SendTo(const std::vector<uint8>& _Data)
{
	if (nullptr == UDPSocket_)
	{
		return false;
	}

	if (0 == _Data.size())
	{
		return false;
	}

	int32 DataSendSize = 0;

	return UDPSocket_->SendTo(&_Data[0], _Data.size(), DataSendSize, ServerSendUDPEndPoint_.ToInternetAddr().Get());

}

void UClientGameInstance::FinishDestroy()
{
	Close();

	Super::FinishDestroy();
}


void UClientGameInstance::PushClientMessage(std::shared_ptr<GameServerMessage> _Message) 
{
	if (false == ClientMode_)
	{
		return;
	}

	MessageQueue_.Enqueue(_Message);
}

std::shared_ptr<GameServerMessage> UClientGameInstance::PopMessage()
{
	std::shared_ptr<GameServerMessage> _Message;
	MessageQueue_.Dequeue(_Message);

	return _Message;
}

bool UClientGameInstance::IsEmptyMessage() 
{
	return MessageQueue_.IsEmpty();
}