#pragma once
#include "ISitableRoom.h"
#include <json/json.h>
#include "MJDefine.h"
class CMJRoom
	:public ISitableRoom
{
public:
	CMJRoom();
	bool onFirstBeCreated(IRoomManager* pRoomMgr,stBaseRoomConfig* pConfig, uint32_t nRoomID , Json::Value& vJsValue)override;
	void serializationFromDB(IRoomManager* pRoomMgr,stBaseRoomConfig* pConfig,uint32_t nRoomID , Json::Value& vJsValue )override;
	void willSerializtionToDB(Json::Value& vOutJsValue)override;
	void roomItemDetailVisitor(Json::Value& vOutJsValue)override;
	void prepareState()override ;
	void roomInfoVisitor(Json::Value& vOutJsValue)override ;
	void sendRoomPlayersInfo(uint32_t nSessionID)override ;
	void setBankerIdx(uint8_t nIdx ){ m_nBankerIdx = nIdx ;}
	uint8_t getBankerIdx(){ return m_nBankerIdx ;}
	uint32_t getBaseBet(); // ji chu di zhu ;
	
	void onGameWillBegin()override ;
	void onGameDidEnd()override ;
	void onPlayerWillStandUp( ISitableRoomPlayer* pPlayer )override ;
	uint32_t getLeastCoinNeedForCurrentGameRound(ISitableRoomPlayer* pp)override ;
	uint8_t getRoomType()override{ return eRoom_MJ ;}
	void prepareCards()override;
	uint32_t coinNeededToSitDown()override;
	void caculateGameResult();
	void onPlayerHuPai(uint8_t nActIdx , uint8_t nCardNumber , uint8_t nInvokerIdx , eMJActType eCardFrom );
	void onPlayerGangPai( uint8_t nActIdx , bool isBuGang );
	bool checkPlayersNeedTheCard( uint8_t nCardNumber ,std::vector<uint8_t> nNeedCardPlayerIdxs, uint8_t nExptPlayerIdx );
	void onPlayerGiveCardToTable( uint8_t nIdx ,eMJActType etype, uint8_t nCardNumber ,eMJActType eCardFrom );  // normal chu pai , or bu gang ;
	uint8_t getLeftCardCnt();
	uint8_t getNextActPlayerIdx();
	void onPlayerMoPai( uint8_t nIdx );
protected:
	ISitableRoomPlayer* doCreateSitableRoomPlayer() override;
protected:
	uint8_t m_nBankerIdx ;
	uint32_t m_nBaseBet ;

	Json::Value m_arrPlayers ;
};