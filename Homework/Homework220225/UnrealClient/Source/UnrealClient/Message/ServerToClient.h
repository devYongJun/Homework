#pragma once
#include "GameServerMessage.h"

class LoginResultMessage : public GameServerMessage                    
{                                                               
public:                                                         
	EGameServerCode Code;
                                                                
public:                                                         
    LoginResultMessage()                                               
        : GameServerMessage(MessageId::LoginResult)                    
        , Code()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~LoginResultMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return DataSizeCheck(Code);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer.WriteEnum(Code);

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer.ReadEnum(Code);

    }                                                           
};                                                              

class ServerDestroyMessage : public GameServerMessage                    
{                                                               
public:                                                         
                                                                
public:                                                         
    ServerDestroyMessage()                                               
        : GameServerMessage(MessageId::ServerDestroy)                    
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~ServerDestroyMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 0;    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            

    }                                                           
};                                                              

class MonsterUpdateMessage : public GameServerMessage                    
{                                                               
public:                                                         
	int ObjectID;
	int MonsterType;
	EMonsterUpdateType UpdateType;
	FVector Pos;
	FVector Dir;
                                                                
public:                                                         
    MonsterUpdateMessage()                                               
        : GameServerMessage(MessageId::MonsterUpdate)                    
        , ObjectID()
        , MonsterType()
        , UpdateType()
        , Pos()
        , Dir()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~MonsterUpdateMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return DataSizeCheck(ObjectID) + DataSizeCheck(MonsterType) + DataSizeCheck(UpdateType) + DataSizeCheck(Pos) + DataSizeCheck(Dir);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << ObjectID;
        _Serializer << MonsterType;
        _Serializer.WriteEnum(UpdateType);
        _Serializer << Pos;
        _Serializer << Dir;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> ObjectID;
        _Serializer >> MonsterType;
        _Serializer.ReadEnum(UpdateType);
        _Serializer >> Pos;
        _Serializer >> Dir;

    }                                                           
};                                                              

class TestMessage : public GameServerMessage                    
{                                                               
public:                                                         
	int asr;
                                                                
public:                                                         
    TestMessage()                                               
        : GameServerMessage(MessageId::Test)                    
        , asr()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~TestMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return DataSizeCheck(asr);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << asr;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> asr;

    }                                                           
};                                                              

rializer >> asr;

    }                                                           
}; 