/*
 * file        : offlineshop_manager.cpp
 * author      : Bahadýr Bozdað || Cosby || Kioshima
 */

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

LPOFFLINESHOP COfflineShopManager::CreateOfflineShop(LPCHARACTER npc, DWORD dwOwnerPID, bool bBoot)
{
	if (FindOfflineShop(npc->GetVID()))
		return NULL;

	LPOFFLINESHOP pkOfflineShop = M2_NEW COfflineShop;
	pkOfflineShop->SetOfflineShopNPC(npc);

	if (bBoot == true)
		pkOfflineShop->CreateTable(dwOwnerPID);

	m_map_pkOfflineShopByNPC.insert(TShopMap::value_type(npc->GetVID(), pkOfflineShop));
	m_Map_pkOfflineShopByNPC2.insert(TOfflineShopMap::value_type(dwOwnerPID, npc->GetVID()));
	return pkOfflineShop;
}

LPOFFLINESHOP COfflineShopManager::FindOfflineShop(DWORD dwVID)
{
	auto it = m_map_pkOfflineShopByNPC.find(dwVID);

	if (it == m_map_pkOfflineShopByNPC.end())
		return NULL;

	return it->second;
}

DWORD COfflineShopManager::FindMyOfflineShop(DWORD dwPID)
{
	auto it = m_Map_pkOfflineShopByNPC2.find(dwPID);
	if (m_Map_pkOfflineShopByNPC2.end() == it)
		return 0;

	return it->second;
}

bool COfflineShopManager::MapCheck(DWORD mapIndex)
{
	if (mapIndex == 1 || mapIndex == 3 || mapIndex == 21 || mapIndex == 23 || mapIndex == 41 || mapIndex == 43)
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
		s_set_dwOfflineShop.erase(iter);
	else
		sys_err("DeleteOfflineShopOnAccount can't find Account ID: %d possible bug!");
}

#ifdef ENABLE_MARKET_CONTROL_SYSTEM
int COfflineShopManager::HaveMarketControlGold(DWORD dwVnum)
{
	auto it = m_map_MarketControl.find(dwVnum);

	if (m_map_MarketControl.end() == it)
		return 0;

	return it->second;
}

void COfflineShopManager::InsertMarketControl(DWORD dwVnum, int iGold)
{
	m_map_MarketControl.insert(TMarketControl::value_type(dwVnum, iGold));
}

void COfflineShopManager::DeleteMarketControl(DWORD dwVnum)
{
	auto it = m_map_MarketControl.find(dwVnum);

	if (m_map_MarketControl.end() == it)
		return;

	m_map_MarketControl.erase(dwVnum);
}
#endif

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
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»óÁ¡°úÀÇ °Å¸®°¡ ³Ê¹« ¸Ö¾î ¹°°ÇÀ» »ì ¼ö ¾ø½À´Ï´Ù."));
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

		if (ch->GetMapIndex() != npc->GetMapIndex())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Tezgâhýný kapatmak için ayný mapte olmalýsýn.");
			return;
		}

		LPOFFLINESHOP pkOfflineShop;
		pkOfflineShop = FindOfflineShop(FindMyOfflineShop(ch->GetPlayerID()));

		if (!pkOfflineShop)
			return;

		/*if (DISTANCE_APPROX(ch->GetX() - npc->GetX(), ch->GetY() - npc->GetY()) > 1500)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have to get near to your shop to do it!"));
			return;
		}*/

		//LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, "", "DESTROY");
		pkOfflineShop->Destroy(npc);

		m_map_pkOfflineShopByNPC.erase(npc->GetVID());
		m_Map_pkOfflineShopByNPC2.erase(npc->GetOfflineShopRealOwner());
		DeleteOfflineShopOnAccount(ch->GetDesc()->GetAccountTable().id);
		M2_DELETE(pkOfflineShop);
		ch->RemoveAffect(AFFECT_SHOPOWNER);
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

		pkOfflineShop->Destroy(npc);
		m_map_pkOfflineShopByNPC.erase(npc->GetVID());
		m_Map_pkOfflineShopByNPC2.erase(npc->GetOfflineShopRealOwner());
		//LogManager::instance().OfflineShopLog(npc->GetOfflineShopRealOwnerAccountID(), "", "DESTROY");
		DeleteOfflineShopOnAccount(npc->GetOfflineShopRealOwnerAccountID());
		M2_DELETE(pkOfflineShop);
	}
}

void COfflineShopManager::UnsoldItems(LPCHARACTER ch)
{
	TPacketGCShop pack;
	pack.header			= HEADER_GC_OFFLINE_SHOP;
	pack.subheader		= SHOP_SUBHEADER_GC_UPDATE_ITEM2;

	TPacketGCOfflineShopStart pack2;
	memset(&pack2, 0, sizeof(pack2));

	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery),
		"SELECT pos, count, vnum, price, price2, price_type, evolution, socket0, socket1, socket2, socket3, "
		"attrtype0, attrvalue0, "
		"attrtype1, attrvalue1, "
		"attrtype2, attrvalue2, "
		"attrtype3, attrvalue3, "
		"attrtype4, attrvalue4, "
		"attrtype5, attrvalue5, "
		"attrtype6, attrvalue6 "
		"FROM %soffline_shop_item WHERE owner_id = %u and sold = 0", get_table_postfix(), ch->GetPlayerID());

	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows > 0)
	{
		for (int i = 0; i < mysql_num_rows(pMsg->Get()->pSQLResult); ++i)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			BYTE cur = 0;

			BYTE bPos = 0;
			str_to_number(bPos, row[cur++]);

			str_to_number(pack2.items[bPos].count, row[cur++]);
			str_to_number(pack2.items[bPos].vnum, row[cur++]);
			str_to_number(pack2.items[bPos].price, row[cur++]);
			str_to_number(pack2.items[bPos].price2, row[cur++]);
			str_to_number(pack2.items[bPos].price_type, row[cur++]);
			str_to_number(pack2.items[bPos].evolution, row[cur++]);
			
			for (BYTE j = 0; j < ITEM_SOCKET_MAX_NUM; j++)
				str_to_number(pack2.items[bPos].alSockets[j], row[cur++]);

			for (BYTE n = 0; n < ITEM_ATTRIBUTE_MAX_NUM; n++)
			{
				str_to_number(pack2.items[bPos].aAttr[n].bType, row[cur++]);
				str_to_number(pack2.items[bPos].aAttr[n].sValue, row[cur++]);
			}
		}
	}

	pack.size = sizeof(pack) + sizeof(pack2);

	if (ch->GetDesc())
	{
		ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCOfflineShopStart));
	}
}

void COfflineShopManager::FetchMyItems(LPCHARACTER ch, BYTE bPos)
{
	if (!ch || bPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM || ch->GetOfflineShopOwner() || ch->GetOfflineShop())
		return;

	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery),
		"SELECT pos, count, vnum, evolution, socket0, socket1, socket2, socket3, "
		"attrtype0, attrvalue0, attrtype1, attrvalue1,"
		"attrtype2, attrvalue2, attrtype3, attrvalue3,"
		"attrtype4, attrvalue4, attrtype5, attrvalue5,"
		"attrtype6, attrvalue6 "
		"FROM %soffline_shop_item WHERE owner_id = %u and pos = %d and sold = 0", get_table_postfix(), ch->GetPlayerID(), bPos);

	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Nothing found."));
		return;
	}

	TPlayerItem item;
	for (int i = 0; i < mysql_num_rows(pMsg->Get()->pSQLResult); ++i)
	{
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		BYTE cur = 0;

		str_to_number(item.pos, row[cur++]);
		str_to_number(item.count, row[cur++]);
		str_to_number(item.vnum, row[cur++]);
		str_to_number(item.evolution, row[cur++]);

		for (BYTE n = 0; n < ITEM_SOCKET_MAX_NUM; n++)
			str_to_number(item.alSockets[n], row[cur++]);

		for (BYTE j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; j++)
		{
			str_to_number(item.aAttr[j].bType, row[cur++]);
			str_to_number(item.aAttr[j].sValue, row[cur++]);
		}
	}

	LPITEM pItem = ITEM_MANAGER::instance().CreateItem(item.vnum, item.count);

	if (pItem)
	{
		pItem->SetSockets(item.alSockets);
		pItem->SetAttributes(item.aAttr);
		pItem->SetEvolution(item.evolution);

		int cell;
#ifdef ENABLE_SPECIAL_STORAGE
		if (pItem->IsUpgradeItem())
			cell = ch->GetEmptyUpgradeInventory(pItem);
		else if (pItem->IsBook())
			cell = ch->GetEmptyBookInventory(pItem);
		else if (pItem->IsStone())
			cell = ch->GetEmptyStoneInventory(pItem);
#endif
		else
			cell = ch->GetEmptyInventory(pItem->GetSize());

		if (cell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Envanterinde boþ yer yok.");
			return;
		}

#ifdef ENABLE_SPECIAL_STORAGE
		if (pItem->IsUpgradeItem())
			pItem->AddToCharacter(ch, TItemPos(UPGRADE_INVENTORY, cell));
		else if (pItem->IsBook())
			pItem->AddToCharacter(ch, TItemPos(BOOK_INVENTORY, cell));
		else if (pItem->IsStone())
			pItem->AddToCharacter(ch, TItemPos(STONE_INVENTORY, cell));
#endif
		else
			pItem->AddToCharacter(ch, TItemPos(INVENTORY, cell));
	}

	DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u AND pos = %d AND sold = 0", get_table_postfix(), ch->GetPlayerID(), bPos);
}

void COfflineShopManager::RefreshPrice(LPCHARACTER ch)
{
	if (!ch)
		return;

	std::auto_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery("SELECT gold_offlineshop, bar1_offlineshop, bar2_offlineshop, bar3_offlineshop FROM %splayer WHERE id = %u", get_table_postfix(), ch->GetPlayerID()));
	
	TPacketGCShop p;
	p.header	= HEADER_GC_OFFLINE_SHOP;
	p.subheader = SHOP_SUBHEADER_GC_REFRESH_PRICE;

	TPacketGCOfflineShopPrice p2;
	if (pMsg->Get()->uiNumRows == 0)
		memset(&p2, 0, sizeof(p2));
	else
	{
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		str_to_number(p2.llMoney, row[0]);
		str_to_number(p2.dwBar1, row[1]);
		str_to_number(p2.dwBar2, row[2]);
		str_to_number(p2.dwBar3, row[3]);
	}

	p.size = sizeof(p) + sizeof(p2);

	if (ch->GetDesc())
	{
		ch->GetDesc()->BufferedPacket(&p, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&p2, sizeof(TPacketGCOfflineShopPrice));
	}
}

bool COfflineShopManager::WithdrawMoney(LPCHARACTER ch, DWORD dwRequiredMoney)
{
	if (!ch)
		return false;

	if (dwRequiredMoney <= 0)
		return false;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT gold_offlineshop FROM player.player WHERE id = %u", ch->GetPlayerID()));

	if (pMsg->Get()->uiNumRows == 0)
		return false;

	DWORD dwCurrentMoney = 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwCurrentMoney, row[0]);

	if (dwCurrentMoney <= 0)
		return false;

	if (dwRequiredMoney > dwCurrentMoney)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Bankadaki tahsilatýndan fazla deðer giremezsin.");
		return false;
	}

	if (ch->GetGold() + dwRequiredMoney > GOLD_MAX - 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Çok fazla yang taþýyorsun, maximumu yang miktarýný geçemezsin.");
		return false;
	}

	ch->PointChange(POINT_GOLD, dwRequiredMoney, false);
	DBManager::instance().DirectQuery("UPDATE player.player SET gold_offlineshop = gold_offlineshop - %u WHERE id = %u", dwRequiredMoney, ch->GetPlayerID());

	ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Tezgâh bankasýndan (%u) yang alýndý.", dwRequiredMoney);

	char pszConv[32];
	sprintf(pszConv, "%d", dwRequiredMoney);

	LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pszConv, "FETCH_MONEY");

	return true;
}

bool COfflineShopManager::WithdrawBar1(LPCHARACTER ch, DWORD dwRequiredBar)
{
	if (!ch)
		return false;

	if (dwRequiredBar <= 0)
		return false;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT bar1_offlineshop FROM player.player WHERE id = %u", ch->GetPlayerID()));

	if (pMsg->Get()->uiNumRows == 0)
		return false;

	DWORD dwCurrentBar = 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwCurrentBar, row[0]);

	if (dwCurrentBar <= 0)
		return false;

	if (dwRequiredBar > dwCurrentBar)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Bankadaki tahsilatýndan fazla deðer giremezsin.");
		return false;
	}

	if (dwRequiredBar > ITEM_MAX_COUNT)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Maximum item sayý sýnýrýný geçemezsin.");
		return false;
	}

	LPITEM pItem = ITEM_MANAGER::Instance().CreateItem(OFFLINE_SHOP_PRICE_GOLD_BAR_1, dwRequiredBar);

	int iEmptyPos = ch->GetEmptyInventory(pItem->GetSize());

	if (iEmptyPos < 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Envanterinde boþ yer yok.");
		return false;
	}

	pItem->AddToCharacter(ch, TItemPos(INVENTORY,iEmptyPos));
	DBManager::instance().DirectQuery("UPDATE player.player SET bar1_offlineshop = bar1_offlineshop - %u WHERE id = %u", dwRequiredBar, ch->GetPlayerID());

	ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Tezgâh bankasýndan (%d) tane Bar(250M) alýndý.", dwRequiredBar);

	char pszConv[32];
	sprintf(pszConv, "%d", dwRequiredBar);

	LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pszConv, "FETCH_BAR1");

	return true;
}

bool COfflineShopManager::WithdrawBar2(LPCHARACTER ch, DWORD dwRequiredBar)
{
	if (!ch)
		return false;

	if (dwRequiredBar <= 0)
		return false;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT bar2_offlineshop FROM player.player WHERE id = %u", ch->GetPlayerID()));

	if (pMsg->Get()->uiNumRows == 0)
		return false;

	DWORD dwCurrentBar = 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwCurrentBar, row[0]);

	if (dwCurrentBar <= 0)
		return false;

	if (dwRequiredBar > dwCurrentBar)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Bankadaki tahsilatýndan fazla deðer giremezsin.");
		return false;
	}

	if (dwRequiredBar > ITEM_MAX_COUNT)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Maximum item sayý sýnýrýný geçemezsin.");
		return false;
	}

	LPITEM pItem = ITEM_MANAGER::Instance().CreateItem(OFFLINE_SHOP_PRICE_GOLD_BAR_2, dwRequiredBar);

	int iEmptyPos = ch->GetEmptyInventory(pItem->GetSize());

	if (iEmptyPos < 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Envanterinde boþ yer yok.");
		return false;
	}

	pItem->AddToCharacter(ch, TItemPos(INVENTORY,iEmptyPos));
	DBManager::instance().DirectQuery("UPDATE player.player SET bar2_offlineshop = bar2_offlineshop - %u WHERE id = %u", dwRequiredBar, ch->GetPlayerID());

	ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Tezgâh bankasýndan (%d) tane Bar(500M) alýndý.", dwRequiredBar);

	char pszConv[32];
	sprintf(pszConv, "%d", dwRequiredBar);

	LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pszConv, "FETCH_BAR2");

	return true;
}

bool COfflineShopManager::WithdrawBar3(LPCHARACTER ch, DWORD dwRequiredBar)
{
	if (!ch)
		return false;

	if (dwRequiredBar <= 0)
		return false;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT bar3_offlineshop FROM player.player WHERE id = %u", ch->GetPlayerID()));

	if (pMsg->Get()->uiNumRows == 0)
		return false;

	DWORD dwCurrentBar = 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwCurrentBar, row[0]);

	if (dwCurrentBar <= 0)
		return false;

	if (dwRequiredBar > dwCurrentBar)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Bankadaki tahsilatýndan fazla deðer giremezsin.");
		return false;
	}

	if (dwRequiredBar > ITEM_MAX_COUNT)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Maximum item sayý sýnýrýný geçemezsin.");
		return false;
	}

	LPITEM pItem = ITEM_MANAGER::Instance().CreateItem(OFFLINE_SHOP_PRICE_GOLD_BAR_3, dwRequiredBar);

	int iEmptyPos = ch->GetEmptyInventory(pItem->GetSize());

	if (iEmptyPos < 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Envanterinde boþ yer yok.");
		return false;
	}

	pItem->AddToCharacter(ch, TItemPos(INVENTORY,iEmptyPos));
	DBManager::instance().DirectQuery("UPDATE player.player SET bar3_offlineshop = bar3_offlineshop - %u WHERE id = %u", dwRequiredBar, ch->GetPlayerID());

	ch->ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Tezgâh bankasýndan (%d) tane Bar(1 T) alýndý.", dwRequiredBar);

	char pszConv[32];
	sprintf(pszConv, "%d", dwRequiredBar);

	LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pszConv, "FETCH_BAR3");

	return true;
}