#pragma once
#include "ServerCommon.h"
#include "IRoom.h"
class IPeerCard ;
class ISitableRoomPlayer
{
public:
	uint8_t getIdx(){return m_nIdx ;}
	void setIdx( uint8_t nIdx){ m_nIdx = nIdx ;}
	virtual void doSitdown(uint8_t nIdx ){ m_nIdx = nIdx ;}
	virtual void willStandUp(){}
	virtual void onGameEnd() ;
	virtual void onGameBegin(){ ++nPlayTimes ; setState(eRoomPeer_CanAct) ;}
	bool isHaveState( uint32_t nStateFlag )
	{
		return ( m_nState & nStateFlag ) == nStateFlag ;
	}

	void removeState( uint32_t nStateFlag );

	void addState( uint32_t nStateFlag ) ;

	void setState( uint32_t nStateFlag );

	uint32_t getState(){ return m_nState ; }
	uint32_t getCoin(){ return nCoin ;}
	void setCoin(int32_t nNewCoin);
	uint32_t getUserUID(){ return nUserUID ; }
	uint32_t getSessionID(){ return nSessionID ; }
	virtual void reset(IRoom::stStandPlayer* pPlayer);
	bool isHaveHalo();
	void switchPeerCard(ISitableRoomPlayer* pPlayer );
	bool isDelayStandUp(){ return m_isDelayStandUp ;}
	void delayStandUp(){ m_isDelayStandUp = true ; }
	uint32_t getMaxWinTimes(){ return nMaxFanShu ; }
	uint32_t getPlayTimes(){ return nPlayTimes ;}
	uint32_t getMaxWinCardType(){ return nMaxFangXingType ;}
	void setTempHaloWeight( uint16_t nTempHalo ){ nTempHaloWeight = nTempHalo ; }
	uint8_t getHaloWeight(){ return nNewPlayerHaloWeight; }
	virtual int32_t getGameOffset() = 0 ;
	virtual IPeerCard* getPeerCard() = 0 ;
	int32_t getTotalGameOffset(){ return nTotalGameOffset ;}
	void setSessionID( uint32_t nNewSessionID ){ nSessionID = nNewSessionID ; m_isDelayStandUp = false ;}
	uint16_t getTotalHaloWeight(){ return nNewPlayerHaloWeight + nTempHaloWeight; }
	void setMaxWinTimes(uint32_t nWinTimesFan ){ nMaxFanShu = nWinTimesFan; }
	void setMaxWinCardType( uint32_t ncardType) { nMaxFangXingType = ncardType ; } 
private:
	bool m_isDelayStandUp;
	uint8_t m_nIdx ;
	uint32_t m_nState ;
	uint8_t m_nHaloState ; // 0 not invoke , 1 in effect , 2 not in effect ;

	uint32_t nCoin ;
	uint32_t nUserUID ;
	uint32_t nSessionID ;
	uint8_t nNewPlayerHaloWeight;
	uint16_t nTempHaloWeight ;
	uint32_t nPlayTimes ;
	uint32_t nMaxFanShu ;
	uint32_t nMaxFangXingType ;
	int32_t nTotalGameOffset ;
};