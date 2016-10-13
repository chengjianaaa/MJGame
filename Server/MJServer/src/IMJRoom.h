#pragma once
#include "IGameRoom.h"
class IMJPlayer;
struct stEnterRoomData;
class IMJRoomState;
class IMJRoom
	:public IGameRoom
{
public:
	typedef std::map<uint16_t, IMJRoomState*>	MAP_ID_ROOM_STATE;
public:
	~IMJRoom();
	bool init(IRoomManager* pRoomMgr, stBaseRoomConfig* pConfig, uint32_t nRoomID, Json::Value& vJsValue) override;
	bool onPlayerEnter(stEnterRoomData* pEnterRoomPlayer)override;
	bool onPlayerApplyLeave(uint32_t nPlayerUID)override;
	bool isRoomFull()override;

	void roomItemDetailVisitor(Json::Value& vOutJsValue) override;
	uint32_t getRoomID()override;
	uint8_t getRoomType() override;
	void update(float fDelta) override;
	bool onMessage(stMsg* prealMsg, eMsgPort eSenderPort, uint32_t nPlayerSessionID)override;
	bool onMsg(Json::Value& prealMsg, uint16_t nMsgType, eMsgPort eSenderPort, uint32_t nSessionID) override;

	void sendRoomMsg(Json::Value& prealMsg, uint16_t nMsgType );
	void sendMsgToPlayer(Json::Value& prealMsg, uint16_t nMsgType, uint32_t nSessionID );
	bool sitdown(IMJPlayer* pPlayer , uint8_t nIdx );
	bool standup( uint32_t nUID );
	IMJPlayer* getMJPlayerBySessionID(uint32_t nSessionid );
	IMJPlayer* getMJPlayerByUID( uint32_t nUID );

	virtual void startGame() { }
	virtual void willStartGame() { }
	virtual void gameEnd(){}
	virtual void onGameDidEnd(){}
	virtual bool canStartGame();
	virtual float getStateDuringForState( uint32_t nState );

	void goToState(IMJRoomState* pTargetState, Json::Value* jsValue = nullptr);
	void goToState(uint16_t nStateID, Json::Value* jsValue = nullptr);
	uint8_t getBankerIdx();
	// mj function ;
	void onWaitPlayerAct( uint8_t nIdx , bool& isCanPass );
	uint8_t getAutoChuCardWhenWaitActTimeout(uint8_t nIdx);
	uint8_t getAutoChuCardWhenWaitChuTimeout(uint8_t nIdx);
	void onPlayerMo( uint8_t nIdx );
	void onPlayerPeng( uint8_t nIdx , uint8_t nCard , uint8_t nInvokeIdx );
	void onPlayerMingGang( uint8_t nIdx, uint8_t nCard, uint8_t nInvokeIdx );
	void onPlayerAnGang( uint8_t nIdx, uint8_t nCard );
	void onPlayerBuGang(uint8_t nIdx, uint8_t nCard );
	void onPlayerHu(std::vector<uint8_t>& vHuIdx, uint8_t nCard, uint8_t nInvokeIdx);
	void onPlayerChu(uint8_t nIdx, uint8_t nCard);
	bool isAnyPlayerPengOrHuThisCard( uint8_t nInvokeIdx , uint8_t nCard );
	void onAskForPengOrHuThisCard(uint8_t nInvokeIdx, uint8_t nCard, std::vector<uint8_t>& vMustWaitCandinates ); // if have hu ,peng will not contain in ,peng candinate is not must wait ;
	bool isAnyPlayerRobotGang(uint8_t nInvokeIdx, uint8_t nCard);
	void onAskForRobotGang(uint8_t nInvokeIdx, uint8_t nCard,std::vector<uint8_t>& vCandinates );
	uint8_t getNextActPlayerIdx( uint8_t nCurActIdx );
	bool isGameOver();
	bool isCanGoOnMoPai();
protected:
	bool addRoomState(IMJRoomState* pState);
protected:
	IMJPlayer* m_vMJPlayers[4];
	MAP_ID_ROOM_STATE m_vRoomStates;
	IMJRoomState* m_pCurState;
};