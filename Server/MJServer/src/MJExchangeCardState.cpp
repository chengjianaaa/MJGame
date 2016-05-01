#include "MJExchangeCardState.h"
#include "MJRoomPlayer.h"
#include "LogManager.h"
#include <cassert>
// wait state 
void CMJWaitExchangeCardState::onWaitEnd( bool bTimeOut )
{
	auto pSitRoom =  (ISitableRoom*)m_pRoom ;
	if ( bTimeOut )
	{
		VEC_WAIT_IDX vWait ( m_vWaitIdxs ) ;
		for ( auto ref : vWait )
		{
			auto pp = (CMJRoomPlayer*)pSitRoom->getPlayerByIdx(ref.nIdx) ;

			stExchangeCardActionItem* p = new stExchangeCardActionItem ;
			p->nActIdx = pp->getIdx() ;
			p->nActType = 0 ;
			p->nExePrio = 0 ;

			for ( uint8_t nIdx = 0 ; nIdx < 3 ; ++nIdx )
			{
				p->vExchangeCard[nIdx] = pp->getCardByIdx(nIdx) ;
				pp->removeCard( p->vExchangeCard[nIdx] ) ;
			}
			responeWaitAct(p->nActIdx,p) ;
		}
	}
	else
	{
		auto pTargeState = (CMJDoExchangeCardState*)m_pRoom->getRoomStateByID(eRoomState_DoExchangeCards) ;
		pTargeState->setExecuteTime(eTime_DoExchangeCard) ;
		pTargeState->setExecuteActs(m_vActList) ;
		m_pRoom->goToState(pTargeState) ;
	}
}

bool CMJWaitExchangeCardState::onMsg(Json::Value& prealMsg ,uint16_t nMsgType, eMsgPort eSenderPort , uint32_t nSessionID)
{
	if ( MSG_PLAYER_CHOSED_EXCHANGE != nMsgType )
	{
		return false ;
	}

	Json::Value vCards = prealMsg["cards"] ;
	if ( vCards.size() != 3 )
	{
		CLogMgr::SharedLogMgr()->PrintLog("please chose 3 cards to exchange") ;
		return true ;
	}

	auto pSitRoom =  (ISitableRoom*)m_pRoom ;
	auto pp = (CMJRoomPlayer*)pSitRoom->getSitdownPlayerBySessionID(nSessionID) ;
	if ( pp == nullptr )
	{
		CLogMgr::SharedLogMgr()->PrintLog("%u sessionid , not sit in this room ",nSessionID) ;
		return true ; 
	}

	if ( isIdxInWaitList(pp->getIdx()) == false )
	{
		CLogMgr::SharedLogMgr()->ErrorLog("session id = %u not in the wait act list" ,nSessionID ) ;
		return true ;
	}

	stExchangeCardActionItem* p = new stExchangeCardActionItem ;
	p->nActIdx = pp->getIdx() ;
	p->nActType = 0 ;
	p->nExePrio = 0 ;

	for ( uint8_t nIdx = 0 ; nIdx < 3 ; ++nIdx )
	{
		p->vExchangeCard[nIdx] = vCards[nIdx].asUInt();
		if ( ! pp->removeCard( p->vExchangeCard[nIdx] ) )
		{
			CLogMgr::SharedLogMgr()->ErrorLog("you do not have this card = %u , cannot used it to exchange",p->vExchangeCard[nIdx]) ;
			delete p ;
			p = nullptr ;
			return true ;
		}
	}
	
	responeWaitAct(p->nActIdx,p) ;

	return true ;
}

// do state 
void CMJDoExchangeCardState::enterState(IRoom* pRoom)
{
	uint8_t vExchangeCard[4][3] = { 0 } ;
	uint8_t vNewCard[4][3] = { 0 } ;
	for ( auto tt : m_vActList )
	{
		memcpy(vExchangeCard[tt->nActIdx],((stExchangeCardActionItem*)tt)->vExchangeCard,sizeof(uint8_t)*3) ;
	}

	uint8_t nMode = rand() % 4 ;  // 0 shun , 1 ni , 2 dui 
	switch ( nMode )
	{
	case 0 :
		{
			for ( uint8_t nIdx = 0 ; nIdx < 4 ; ++nIdx )
			{
				uint8_t nTargetIdx = nIdx + 1 ;
				nTargetIdx = nTargetIdx % 4 ;
				memcpy(vNewCard[nIdx],vExchangeCard[nTargetIdx],sizeof(uint8_t)*3) ;
			}
			
		}
		break;
	case 1:
		{
			for ( uint8_t nIdx = 4 ; nIdx > 0 ; --nIdx )
			{
				uint8_t nTargetIdx = nIdx - 1 ;
				nTargetIdx = nTargetIdx % 4 ;
				uint8_t nOri = nIdx % 4 ;
				memcpy(vNewCard[nOri],vExchangeCard[nTargetIdx],sizeof(uint8_t)*3) ;
			}
		}
		break;
	case 2:
		{
			for ( uint8_t nIdx = 0 ; nIdx < 4 ; nIdx += 2 )
			{
				uint8_t nTargetIdx = nIdx + 1 ;
				nTargetIdx = nTargetIdx % 4 ;
				memcpy(vNewCard[nIdx],vExchangeCard[nTargetIdx],sizeof(uint8_t)*3) ;
			}
		}
		break ;
	default:
		break;
	}

	// do exchange 
	Json::Value msg ;
	msg["mode"] = nMode ;

	Json::Value arrayCards ;
	for ( uint8_t nidx = 0 ; nidx < 4 ; ++nidx )
	{
		auto pP = (CMJRoomPlayer*)((ISitableRoom*)pRoom)->getPlayerByIdx(nidx) ;
		assert(pP && "why pp is null ?");

		Json::Value pPlayer ;
		Json::Value vCards;
		for ( uint8_t cardidx = 0 ; cardidx < 3 ; ++cardidx )
		{
			vCards[(uint32_t)cardidx] = (uint8_t)vNewCard[nidx][cardidx];
			pP->addCard((uint8_t)vNewCard[nidx][cardidx]);
		}
		pPlayer["idx"] = nidx;
		pPlayer["cards"] = vCards;

		arrayCards[(uint32_t)nidx] = pPlayer ;
	}

	msg["result"] = arrayCards ;
	pRoom->sendRoomMsg(msg,MSG_ROOM_FINISH_EXCHANGE) ;

	IExecuingState::enterState(pRoom) ;
}

void CMJDoExchangeCardState::onExecuteOver()
{
	auto ppState = (IWaitingState*)m_pRoom->getRoomStateByID(eRoomState_WaitDecideQue) ;
	for ( uint8_t nIdx = 0 ; nIdx < 4 ; ++nIdx )
	{
		ppState->addWaitingTarget(nIdx) ;
	}
	ppState->setWaitTime(eTime_WaitDecideQue) ;
	m_pRoom->goToState(ppState) ;
}