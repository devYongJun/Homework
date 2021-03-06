#pragma once
#include "GameServerMessage.h"
#include "MessageIdEnum.h"
#include "ContentsEnum.h"
#include "ContentsStructure.h"

class LoginMessage : public GameServerMessage                    
{                                                               
public:                                                         
	std::string ID;
	std::string PW;
                                                                
public:                                                         
    LoginMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::Login))                    
        , ID()
        , PW()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~LoginMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(ID) + DataSizeCheck(PW);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << ID;
        _Serializer << PW;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> ID;
        _Serializer >> PW;

    }                                                           
};                                                              

class CreateCharacterMessage : public GameServerMessage                    
{                                                               
public:                                                         
	std::string NickName;
                                                                
public:                                                         
    CreateCharacterMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::CreateCharacter))                    
        , NickName()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~CreateCharacterMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(NickName);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << NickName;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> NickName;

    }                                                           
};                                                              

class SelectCharacterMessage : public GameServerMessage                    
{                                                               
public:                                                         
	std::string NickName;
                                                                
public:                                                         
    SelectCharacterMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::SelectCharacter))                    
        , NickName()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~SelectCharacterMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(NickName);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << NickName;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> NickName;

    }                                                           
};                                                              

class ClientToReadyMessage : public GameServerMessage                    
{                                                               
public:                                                         
	uint64_t ObjectIndex;
	uint64_t ThreadIndex;
	uint64_t SectionIndex;
                                                                
public:                                                         
    ClientToReadyMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::ClientToReady))                    
        , ObjectIndex()
        , ThreadIndex()
        , SectionIndex()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~ClientToReadyMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(ObjectIndex) + DataSizeCheck(ThreadIndex) + DataSizeCheck(SectionIndex);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << ObjectIndex;
        _Serializer << ThreadIndex;
        _Serializer << SectionIndex;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> ObjectIndex;
        _Serializer >> ThreadIndex;
        _Serializer >> SectionIndex;

    }                                                           
};                                                              

class UDPConnectResultMessage : public GameServerMessage                    
{                                                               
public:                                                         
	uint64_t ObjectIndex;
	uint64_t ThreadIndex;
	uint64_t SectionIndex;
                                                                
public:                                                         
    UDPConnectResultMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::UDPConnectResult))                    
        , ObjectIndex()
        , ThreadIndex()
        , SectionIndex()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~UDPConnectResultMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(ObjectIndex) + DataSizeCheck(ThreadIndex) + DataSizeCheck(SectionIndex);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << ObjectIndex;
        _Serializer << ThreadIndex;
        _Serializer << SectionIndex;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> ObjectIndex;
        _Serializer >> ThreadIndex;
        _Serializer >> SectionIndex;

    }                                                           
};                                                              

class LevelMoveReplyMessage : public GameServerMessage                    
{                                                               
public:                                                         
	uint64_t ObjectIndex;
	uint64_t ThreadIndex;
	uint64_t SectionIndex;
                                                                
public:                                                         
    LevelMoveReplyMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::LevelMoveReply))                    
        , ObjectIndex()
        , ThreadIndex()
        , SectionIndex()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~LevelMoveReplyMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(ObjectIndex) + DataSizeCheck(ThreadIndex) + DataSizeCheck(SectionIndex);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << ObjectIndex;
        _Serializer << ThreadIndex;
        _Serializer << SectionIndex;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> ObjectIndex;
        _Serializer >> ThreadIndex;
        _Serializer >> SectionIndex;

    }                                                           
};                                                              

class RankRequestMessage : public GameServerMessage                    
{                                                               
public:                                                         
	std::string NickName;
                                                                
public:                                                         
    RankRequestMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::RankRequest))                    
        , NickName()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~RankRequestMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(NickName);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << NickName;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> NickName;

    }                                                           
};                                                              

class BigMessage : public GameServerMessage                    
{                                                               
public:                                                         
	std::vector<int> Arr;
                                                                
public:                                                         
    BigMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::Big))                    
        , Arr()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~BigMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(Arr);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer.WriteVector( Arr);

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer.ReadVector( Arr);

    }                                                           
};                                                              

class AttackMessage : public GameServerMessage                    
{                                                               
public:                                                         
	uint64_t ObjectIndex;
	uint64_t ThreadIndex;
	uint64_t SectionIndex;
	FVector4 Pos;
	int Radius;
                                                                
public:                                                         
    AttackMessage()                                               
        : GameServerMessage(static_cast<uint32_t>(MessageId::Attack))                    
        , ObjectIndex()
        , ThreadIndex()
        , SectionIndex()
        , Pos()
        , Radius()
    {                                                           
                                                                
    }                                                           
                                                                
    virtual ~AttackMessage() {}                                   
                                                                
    virtual int SizeCheck()                                     
    {                                                           
		return 8 + DataSizeCheck(ObjectIndex) + DataSizeCheck(ThreadIndex) + DataSizeCheck(SectionIndex) + DataSizeCheck(Pos) + DataSizeCheck(Radius);
    }                                                           
                                                                
    void Serialize(GameServerSerializer& _Serializer)           
    {                                                           
        GameServerMessage::Serialize(_Serializer);              
        _Serializer << ObjectIndex;
        _Serializer << ThreadIndex;
        _Serializer << SectionIndex;
        _Serializer << Pos;
        _Serializer << Radius;

    }                                                           
                                                                
    void DeSerialize(GameServerSerializer& _Serializer)         
    {                                                           
        GameServerMessage::DeSerialize(_Serializer);            
        _Serializer >> ObjectIndex;
        _Serializer >> ThreadIndex;
        _Serializer >> SectionIndex;
        _Serializer >> Pos;
        _Serializer >> Radius;

    }                                                           
};                                                              

