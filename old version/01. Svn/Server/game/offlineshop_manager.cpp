#include <boost/algorithm/string/predicate.hpp>
#include <cctype>
#include "stdafx.h"
#include "../../libgame/include/grid.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
#include "offlineshop.h"
#include "desc.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "questmanager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "locale_service.h"
#include "desc_client.h"
#include "group_text_parse_tree.h"
#include "offlineshop_manager.h"
#include "p2p.h"
#include "entity.h"
#include "sectree_manager.h"

std::set<DWORD> s_set_dwOfflineShop;

COfflineShopManager::COfflineShopManager()
{
}

COfflineShopManager::~COfflineShopManager()
{
}

LPOFFLINESHOP COfflineShopManager::CreateOfflineShop(LPCHARACTER npc, DWORD dwOwnerPID)
{
	if (FindOfflineShop(npc->GetVID()))
		return NULL;

	LPOFFLINESHOP pkOfflineShop = M2_NEW COfflineShop;
	pkOfflineShop->SetOfflineShopNPC(npc);

	m_map_pkOfflineShopByNPC.insert(TShopMap::value_type(npc->GetVID(), pkOfflineShop));
	m_Map_pkOfflineShopByNPC2.insert(TOfflineShopMap::value_type(dwOwnerPID, npc->GetVID()));
	return pkOfflineShop;
}

LPOFFLINESHOP COfflineShopManager::FindOfflineShop(DWORD dwVID)
{
	TShopMap::iterator it = m_map_pkOfflineShopByNPC.find(dwVID);

	if (it == m_map_pkOfflineShopByNPC.end())
		return NULL;

	return it->second;
}

DWORD COfflineShopManager::FindMyOfflineShop(DWORD dwPID)
{
	TOfflineShopMap::iterator it = m_Map_pkOfflineShopByNPC2.find(dwPID);
	if (m_Map_pkOfflineShopByNPC2.end() == it)
		return 0;

	return it->second;
}

bool COfflineShopManager::MapCheck(DWORD mapIndex, DWORD empire)
{
	if (empire == 1)
		if (mapIndex == 1 || mapIndex == 3)
			return true;

	if (empire == 2)
		if (mapIndex == 21 || mapIndex == 23)
			return true;

	if (empire == 3)
		if (mapIndex == 41 || mapIndex == 43)
			return true;

	return false;
}

bool COfflineShopManager::ChannelCheck(DWORD dwChannel)
{
	if (dwChannel == 1)
		return true;
	
	return false;
}

bool COfflineShopManager::SearchOfflineShop(LPCHARACTER ch)
{
	if (!ch)
		return false;

	LPCHARACTER npc;
	npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));

	if(npc)
		return true;
	else
		return false;
}

void COfflineShopManager::ResetOfflineShopStatus(LPCHARACTER ch)
{
	if (ch->IsAffectFlag(AFF_SHOPOWNER))
	{
		//sys_log(0, "COfflineShopManager::ResetOfflineShopStatus Affect found checking... Name: [%s]", ch->GetName());

		if (!SearchOfflineShop(ch))
		{
			ch->RemoveAffect(AFFECT_SHOPOWNER);
			//sys_log(0, "COfflineShopManager::ResetOfflineShopStatus shop wasn't exist removed shopowner affect name: [%s]", ch->GetName());
		}
	}
}

bool COfflineShopManager::HaveOfflineShopOnAccount(DWORD aID)
{
	if (s_set_dwOfflineShop.find(aID) != s_set_dwOfflineShop.end())
		return true;
	
	return false;
}

void COfflineShopManager::InsertOfflineShopToAccount(DWORD aID)
{
	s_set_dwOfflineShop.insert(aID);
}

void COfflineShopManager::DeleteOfflineShopOnAccount(DWORD aID)
{	
	itertype(s_set_dwOfflineShop) iter = s_set_dwOfflineShop.find(aID);
	
	if (iter != s_set_dwOfflineShop.end())
	{
		s_set_dwOfflineShop.erase(iter);
	}
	else
	{
		sys_err("DeleteOfflineShopOnAccount can't find Account ID: %d possible bug!");
	}
}

bool COfflineShopManager::StartShopping(LPCHARACTER pkChr, LPCHARACTER pkChrShopKeeper)
{
	if(!pkChr || pkChrShopKeeper)
		return false;
	
	if (pkChr->GetOfflineShopOwner() == pkChrShopKeeper)
		return false;

	if (pkChrShopKeeper->IsPC())
		return false;

	sys_log(0, "OFFLINE_SHOP: START: %s", pkChr->GetName());
	return true;
}

void COfflineShopManager::StopShopping(LPCHARACTER ch)
{
	if(!ch)
		return;
	
	LPOFFLINESHOP pkOfflineShop;

	if (!(pkOfflineShop = ch->GetOfflineShop()))
		return;

	pkOfflineShop->RemoveGuest(ch);
	sys_log(0, "OFFLINE_SHOP: END: %s", ch->GetName());
}

void COfflineShopManager::Buy(LPCHARACTER ch, BYTE pos)
{
	if(!ch)
		return;

	if (!ch->GetOfflineShop())
		return;

	if (!ch->GetOfflineShopOwner())
		return;

	if (DISTANCE_APPROX(ch->GetX() - ch->GetOfflineShopOwner()->GetX(), ch->GetY() - ch->GetOfflineShopOwner()->GetY()) > 1500)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상점과의 거리가 너무 멀어 물건을 살 수 없습니다."));
		return;
	}

	LPOFFLINESHOP pkOfflineShop = ch->GetOfflineShop();

	if (!pkOfflineShop)
		return;

	int ret = pkOfflineShop->Buy(ch, pos);

	// The result is not equal to SHOP_SUBHEADER_GC_OK, send the error to the character.
	if (SHOP_SUBHEADER_GC_OK != ret)
	{
		TPacketGCShop pack;
		pack.header = HEADER_GC_OFFLINE_SHOP;
		pack.subheader	= ret;
		pack.size	= sizeof(TPacketGCShop);

		ch->GetDesc()->Packet(&pack, sizeof(pack));
	}
}

void COfflineShopManager::PutItem(LPCHARACTER ch, TItemPos item_pos, int iPrice)
{
	if (!ch || iPrice <= 0 || iPrice > 999999999)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	ch->SetMyOfflineShopTime();

	quest::PC * pPC = quest::CQuestManager::Instance().GetPC(ch->GetPlayerID());

	if (pPC->IsRunning())
		return;

	if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetOfflineShop() || ch->GetMyShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
		return;
	}

	if (!COfflineShopManager::instance().MapCheck(ch->GetMapIndex(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that map!"));
		return;
	}

	if (!COfflineShopManager::instance().ChannelCheck(g_bChannel))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that channel!"));
		return;
	}

	if (!ch->IsAffectFlag(AFF_SHOPOWNER))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You need to have active offline shop to do that!"));
		return;
	}

	LPCHARACTER npc;
	npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));

	if (!npc)
		return;

	if (DISTANCE_APPROX(ch->GetX() - npc->GetX(), ch->GetY() - npc->GetY()) > 1500)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have to get near to your shop to do it!"));
		return;
	}

	LPOFFLINESHOP pkOfflineShop;
	pkOfflineShop = FindOfflineShop(FindMyOfflineShop(ch->GetPlayerID()));

	if (!pkOfflineShop)
		return;

	pkOfflineShop->PutItem(ch, item_pos, iPrice);
}

void COfflineShopManager::DestroyOfflineShop(LPCHARACTER ch, DWORD dwVID, bool pcMode)
{
	if (pcMode)
	{
		if (!ch)
			return;

		quest::PC * pPC = quest::CQuestManager::Instance().GetPC(ch->GetPlayerID());

		if (pPC->IsRunning())
			return;

		if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetOfflineShop())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
			return;
		}

		if (!COfflineShopManager::instance().MapCheck(ch->GetMapIndex(), ch->GetEmpire()))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that map!"));
			return;
		}

		if (!COfflineShopManager::instance().ChannelCheck(g_bChannel))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that channel!"));
			return;
		}

		if (!ch->IsAffectFlag(AFF_SHOPOWNER))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Seems you don't have an offline shop to destroy? ..."));
			return;
		}

		LPCHARACTER npc;
		npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));

		if (!npc)
			return;

		LPOFFLINESHOP pkOfflineShop;
		pkOfflineShop = FindOfflineShop(FindMyOfflineShop(ch->GetPlayerID()));

		if (!pkOfflineShop)
			return;

		if (DISTANCE_APPROX(ch->GetX() - npc->GetX(), ch->GetY() - npc->GetY()) > 1500)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have to get near to your shop to do it!"));
			return;
		}

		//LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, "", "DESTROY");
		pkOfflineShop->GiveBackMyItems(ch);
		pkOfflineShop->Destroy(npc);

		m_map_pkOfflineShopByNPC.erase(npc->GetVID());
		m_Map_pkOfflineShopByNPC2.erase(npc->GetOfflineShopRealOwner());
		DeleteOfflineShopOnAccount(ch->GetDesc()->GetAccountTable().id);
		M2_DELETE(pkOfflineShop);
		ch->RemoveAffect(AFFECT_SHOPOWNER);

		// COUNTER_UPDATE
		TPacketUpdateOfflineShopsCount pCount;
		pCount.bIncrease = false;
		db_clientdesc->DBPacket(HEADER_GD_UPDATE_OFFLINESHOP_COUNT, 0, &pCount, sizeof(pCount));
		// COUNTER_UPDATE
	}
	else
	{
		LPCHARACTER npc = CHARACTER_MANAGER::instance().Find(dwVID);
		LPOFFLINESHOP pkOfflineShop = FindOfflineShop(dwVID);

		if (!npc)
		{
			sys_err("COfflineShopManager::DestroyOfflineShop CRITICAL ERROR! NPC NOT FOUND! VID: %d", dwVID);
			return;
		}

		if (!pkOfflineShop)
		{
			sys_err("COfflineShopManager::DestroyOfflineShop CRITICAL ERROR! pkOfflineShop NOT FOUND! VID: %d", dwVID);
			return;
		}

		m_map_pkOfflineShopByNPC.erase(npc->GetVID());
		m_Map_pkOfflineShopByNPC2.erase(npc->GetOfflineShopRealOwner());
		//LogManager::instance().OfflineShopLog(npc->GetOfflineShopRealOwnerAccountID(), "", "DESTROY");
		DeleteOfflineShopOnAccount(npc->GetOfflineShopRealOwnerAccountID());
		M2_DELETE(pkOfflineShop);

		// COUNTER_UPDATE
		TPacketUpdateOfflineShopsCount pCount;
		pCount.bIncrease = false;
		db_clientdesc->DBPacket(HEADER_GD_UPDATE_OFFLINESHOP_COUNT, 0, &pCount, sizeof(pCount));
		// COUNTER_UPDATE
	}
}

void COfflineShopManager::FetchMyItems(LPCHARACTER ch)
{
	if (!ch)
		return;

	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery),
		"SELECT pos, count, vnum, socket0, socket1, socket2, "
		"attrtype0, attrvalue0, attrtype1, attrvalue1,"
		"attrtype2, attrvalue2, attrtype3, attrvalue3,"
		"attrtype4, attrvalue4, attrtype5, attrvalue5,"
		"attrtype6, attrvalue6 "
		"FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());

	std::auto_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Nothing found."));
		return;
	}

	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		TPlayerItem item;

		str_to_number(item.pos, row[0]);
		str_to_number(item.count, row[1]);
		str_to_number(item.vnum, row[2]);

		for (int i = 0, n = 3; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(item.alSockets[i], row[n]);

		for (int i = 0, iStartAttributeType = 6, iStartAttributeValue = ITEM_ATTRIBUTE_MAX_NUM; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
		{
			str_to_number(item.aAttr[i].bType, row[iStartAttributeType]);
			str_to_number(item.aAttr[i].sValue, row[iStartAttributeValue]);
		}

		LPITEM pItem = ITEM_MANAGER::instance().CreateItem(item.vnum, item.count);

		if (pItem)
		{
			pItem->SetSockets(item.alSockets);
			pItem->SetAttributes(item.aAttr);

			int cell;
			cell = ch->GetEmptyInventory(pItem->GetSize());

			if (cell != -1)
			{
				pItem->AddToCharacter(ch, TItemPos(INVENTORY, cell));
			}
			else
			{
				pItem->AddToGround(ch->GetMapIndex(), ch->GetXYZ());
				pItem->StartDestroyEvent();
				pItem->SetOwnership(ch, 60);
			}

			// release DB
			DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u AND pos = %d AND vnum = %d LIMIT 1", get_table_postfix(), ch->GetPlayerID(), item.pos, item.vnum);
			
			/* log for web */
			LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pItem->GetName(), "FETCH");
			/* end log for web */
		}
	}
}

bool COfflineShopManager::WithdrawAllMoney(LPCHARACTER ch)
{
	if (!ch)
		return false;

	std::auto_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT gold_offlineshop FROM player.player WHERE id = %u", ch->GetPlayerID()));

	if (pMsg->Get()->uiNumRows == 0)
		return false;

	DWORD dwBankMoney = 0, dwMoneyToGet = 0;
	bool bGiveLittleMoney = false; // for prevent bug GOLD_MAX in the bank

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwBankMoney, row[0]);

	if (dwBankMoney <= 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bank text 1"));
		return false;
	}

	if (dwBankMoney > GOLD_MAX)
	{
		bGiveLittleMoney = true;
		dwMoneyToGet = 1000000000; // 1.000.000.000 golds
	}
	else
	{
		dwMoneyToGet = dwBankMoney;
	}
	
	bool isOverFlow = ch->GetGold() + dwMoneyToGet > GOLD_MAX - 1 ? true : false;

	if (isOverFlow)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bank text 2"));
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bank text 3"));
		return false;
	}

	ch->PointChange(POINT_GOLD, dwMoneyToGet, false);
	DBManager::instance().DirectQuery("UPDATE player.player SET gold_offlineshop = gold_offlineshop - %u WHERE id = %u", dwMoneyToGet, ch->GetPlayerID());

	if (bGiveLittleMoney)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bank text 4"));
		// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bank text 5"));
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bank text 6 %u"), dwMoneyToGet);
	
	char pszConv[32];
	sprintf(pszConv, "%d", dwMoneyToGet);

	LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pszConv, "FETCH_MONEY");

	return true;
}