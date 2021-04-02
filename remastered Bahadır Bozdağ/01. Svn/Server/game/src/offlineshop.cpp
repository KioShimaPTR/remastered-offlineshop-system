/*
 * file        : offlineshop.cpp
 * author      : Bahadýr Bozdað || Cosby || Kioshima
 */

#include "stdafx.h"
#include "../../libgame/include/grid.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
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
#include "offlineshop.h"
#include "p2p.h"

COfflineShop::COfflineShop() : m_pkOfflineShopNPC(NULL)
{
	m_pGrid = M2_NEW CGrid(5, 9);
}

COfflineShop::~COfflineShop()
{
	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	Broadcast(&pack, sizeof(pack));

	for (auto it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		ch->SetOfflineShop(NULL);
	}

	M2_DELETE(m_pGrid);
	looked.clear();
}

void COfflineShop::SetOfflineShopNPC(LPCHARACTER npc)
{
	m_pkOfflineShopNPC = npc;
}

void COfflineShop::CreateTable(DWORD dwOwnerID)
{
	MYSQL_ROW row;
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT * FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), dwOwnerID));

	m_offlineShopItemVector.resize(OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_offlineShopItemVector[0], 0, sizeof(OFFLINE_SHOP_ITEM) * m_offlineShopItemVector.size());

	if (pMsg->Get()->uiNumRows > 0)
	{
		while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			DWORD vnum = 0;
			str_to_number(vnum, row[4]);
			BYTE bPos = 0;
			str_to_number(bPos, row[2]);

			const TItemTable * item_table = ITEM_MANAGER::instance().GetTable(vnum);

			if (!item_table)
			{
				sys_err("OfflineShop: no item table by item vnum #%d", vnum);
				continue;
			}

			/* memory part begin */
			OFFLINE_SHOP_ITEM & offShopItem = m_offlineShopItemVector[bPos];

			str_to_number(offShopItem.owner_id, row[1]);
			str_to_number(offShopItem.pos, row[2]);
			str_to_number(offShopItem.count, row[3]);
			str_to_number(offShopItem.vnum, row[4]);
			str_to_number(offShopItem.price, row[5]);
			str_to_number(offShopItem.price2, row[6]);
			str_to_number(offShopItem.price_type, row[7]);
			str_to_number(offShopItem.evolution, row[8]);
			str_to_number(offShopItem.bIsSold, row[9]);
			strlcpy(offShopItem.buyerName, row[10], CHARACTER_NAME_MAX_LEN);
			str_to_number(offShopItem.alSockets[0], row[11]);
			str_to_number(offShopItem.alSockets[1], row[12]);
			str_to_number(offShopItem.alSockets[2], row[13]);
			str_to_number(offShopItem.alSockets[3], row[14]);
			str_to_number(offShopItem.aAttr[0].bType, row[15]);
			str_to_number(offShopItem.aAttr[0].sValue, row[16]);
			str_to_number(offShopItem.aAttr[1].bType, row[17]);
			str_to_number(offShopItem.aAttr[1].sValue, row[18]);
			str_to_number(offShopItem.aAttr[2].bType, row[19]);
			str_to_number(offShopItem.aAttr[2].sValue, row[20]);
			str_to_number(offShopItem.aAttr[3].bType, row[21]);
			str_to_number(offShopItem.aAttr[3].sValue, row[22]);
			str_to_number(offShopItem.aAttr[4].bType, row[23]);
			str_to_number(offShopItem.aAttr[4].sValue, row[24]);
			str_to_number(offShopItem.aAttr[5].bType, row[25]);
			str_to_number(offShopItem.aAttr[5].sValue, row[26]);
			str_to_number(offShopItem.aAttr[6].bType, row[27]);
			str_to_number(offShopItem.aAttr[6].sValue, row[28]);
			strlcpy(offShopItem.szName, row[29], sizeof(offShopItem.szName));
			DWORD vid = GetOfflineShopNPC()->GetVID();
			offShopItem.shop_id = vid;
		}
	}
}

void COfflineShop::SetShopItems(TShopItemTable2 * pTable, BYTE bItemCount, LPCHARACTER ch)
{
	if (!ch)
		return;

	m_pGrid->Clear();

	m_offlineShopItemVector.resize(OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_offlineShopItemVector[0], 0, sizeof(OFFLINE_SHOP_ITEM) * m_offlineShopItemVector.size());

	for (int i = 0; i < bItemCount; ++i)
	{
		int iPos;
		LPITEM pkItem = ch->GetItem(pTable->pos);

		if (!pkItem)
		{
			sys_err("COfflineShop::SetShopItems: cannot find item on pos (%d, %d) (name: %s)", pTable->pos.window_type, pTable->pos.cell, ch->GetName());
			continue;
		}

#ifdef __SOULBINDING_SYSTEM__
		if (pkItem->IsBind() || pkItem->IsUntilBind())
			continue;
#endif

		if(pkItem->IsEquipped() == true || pkItem->isLocked() == true || pkItem->IsExchanging())
			continue;

		const TItemTable * item_table = pkItem->GetProto();

		if (!item_table)
		{
			sys_err("COfflineShop::SetShopItems: no item table by item vnum #%d", pTable->vnum);
			continue;
		}

		if (IS_SET(item_table->dwAntiFlags, ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_MYSHOP))
			continue;

		iPos = pTable->display_pos;

		if (iPos < 0)
		{
			sys_err("COfflineShop::SetShopItems: not enough shop window!");
			continue;
		}

		//sys_log(0, "COfflineShop::SetShopItems: use position %d", iPos);

		if (!m_pGrid->IsEmpty(iPos, 1, pkItem->GetSize()))
		{
			sys_err("COfflineShop::SetShopItems: not empty position offline shop %s[%d]", ch->GetName(), ch->GetPlayerID());
			continue;
		}

		m_pGrid->Put(iPos, 1, pkItem->GetSize());

		/* memory part begin */
		OFFLINE_SHOP_ITEM & offShopItem = m_offlineShopItemVector[iPos];

		//////////
		// bIsSold already declared as = false in offlineshop.h
		offShopItem.owner_id = ch->GetPlayerID();
		offShopItem.pos = iPos;
		offShopItem.count = pkItem->GetCount();
		offShopItem.price = pTable->price;
		offShopItem.price2 = pTable->price2;
		offShopItem.price_type = pTable->price_type;
		offShopItem.vnum = pkItem->GetVnum();
		offShopItem.evolution = pkItem->GetEvolution();

		for (int x = 0; x < ITEM_SOCKET_MAX_NUM; x++)
			offShopItem.alSockets[x] = pkItem->GetSocket(x);

		for (int x = 0; x < ITEM_ATTRIBUTE_MAX_NUM; x++)
		{
			offShopItem.aAttr[x].bType = pkItem->GetAttributeType(x);
			offShopItem.aAttr[x].sValue = pkItem->GetAttributeValue(x);
		}

		strlcpy(offShopItem.szName, pkItem->GetName(), sizeof(offShopItem.szName));
		offShopItem.shop_id = GetOfflineShopNPC()->GetVID();

		/* memory part end */

		/* query part begin */
		char szColumns[QUERY_MAX_LEN], szValues[QUERY_MAX_LEN];

		snprintf(szColumns, sizeof(szColumns),
		"owner_id, pos, count, price, price2, price_type, vnum, evolution,"
		"socket0, socket1, socket2, socket3,"
		"attrtype0, attrvalue0, attrtype1, attrvalue1,"
		"attrtype2, attrvalue2, attrtype3, attrvalue3,"
		"attrtype4, attrvalue4, attrtype5, attrvalue5,"
		"attrtype6, attrvalue6, item_name, shop_id");
		
		snprintf(szValues, sizeof(szValues), "%u, %d, %u, %u, %d, %d, %u, %d, %ld, %ld, %ld, %ld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d", 
			ch->GetPlayerID(), iPos, pkItem->GetCount(), pTable->price, pTable->price2, pTable->price_type, pkItem->GetVnum(), pkItem->GetEvolution(),
			pkItem->GetSocket(0), pkItem->GetSocket(1),
			pkItem->GetSocket(2), pkItem->GetSocket(3),
			pkItem->GetAttributeType(0), pkItem->GetAttributeValue(0), pkItem->GetAttributeType(1), pkItem->GetAttributeValue(1),
			pkItem->GetAttributeType(2), pkItem->GetAttributeValue(2), pkItem->GetAttributeType(3), pkItem->GetAttributeValue(3),
			pkItem->GetAttributeType(4), pkItem->GetAttributeValue(4), pkItem->GetAttributeType(5), pkItem->GetAttributeValue(5),
			pkItem->GetAttributeType(6), pkItem->GetAttributeValue(6), pkItem->GetName2(), (DWORD)GetOfflineShopNPC()->GetVID());

		char szInsertQuery[QUERY_MAX_LEN];
		snprintf(szInsertQuery, sizeof(szInsertQuery), "INSERT INTO %soffline_shop_item (%s) VALUES (%s)", get_table_postfix(), szColumns, szValues);
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szInsertQuery));
		/* query part end */

		/* log for web */
		LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pkItem->GetName(), "PUT");
		/* end log for web */

		ITEM_MANAGER::instance().RemoveItem(pkItem);

		++pTable;
	}
}

bool COfflineShop::AddGuest(LPCHARACTER ch, LPCHARACTER npc)
{
	if (!ch || ch->GetExchange() || ch->GetShop() || ch->GetMyShop() || ch->GetOfflineShop())
		return false;

	ch->SetOfflineShop(this);
	m_map_guest.insert(GuestMapType::value_type(ch, false));

	TPacketGCShop pack;
	pack.header		= HEADER_GC_OFFLINE_SHOP;
	pack.subheader	= SHOP_SUBHEADER_GC_START;

	TPacketGCOfflineShopStart pack2;
	memset(&pack2, 0, sizeof(pack2));
	pack2.owner_vid = npc->GetVID();

	if (GetLeftItemCount() == 0)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.offline_shop_npc WHERE owner_id = %u", npc->GetOfflineShopRealOwner());
		ch->SetOfflineShop(NULL);
		ch->SetOfflineShopOwner(NULL);
		M2_DESTROY_CHARACTER(npc);
		return false;
	}

	LPCHARACTER owner = CHARACTER_MANAGER::instance().Find(npc->GetVID());
	if (owner)
	{
		if (ch != owner && owner->IsOfflineShopNPC())
		{
			looked.push_back(looked.size()+1);
			pack2.looked = looked.size();
		}
		else
			pack2.looked = looked.size();
	}

	for (DWORD i = 0; i < m_offlineShopItemVector.size() && i < OFFLINE_SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const OFFLINE_SHOP_ITEM & offShopItem = m_offlineShopItemVector[i];

		pack2.items[i].count = offShopItem.count;
		pack2.items[i].vnum = offShopItem.vnum;
		pack2.items[i].price = offShopItem.price;
		pack2.items[i].price2 = offShopItem.price2;
		pack2.items[i].price_type = offShopItem.price_type;
		pack2.items[i].evolution = offShopItem.evolution;
		pack2.items[i].bIsSold = offShopItem.bIsSold;
		strlcpy(pack2.items[i].buyerName, offShopItem.buyerName, CHARACTER_NAME_MAX_LEN);

		for (int x = 0; x < ITEM_SOCKET_MAX_NUM; x++)
			pack2.items[i].alSockets[x] = offShopItem.alSockets[x];
	
		for (int x = 0; x < ITEM_ATTRIBUTE_MAX_NUM; x++)
		{
			const TPlayerItemAttribute & c_rItemAttr = offShopItem.aAttr[x];
			
			pack2.items[i].aAttr[x].bType = c_rItemAttr.bType;
			pack2.items[i].aAttr[x].sValue = c_rItemAttr.sValue;
		}
	}

	pack.size = sizeof(pack) + sizeof(pack2);

	if (ch->GetDesc())
	{
		ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCOfflineShopStart));
	}

	return true;
}

void COfflineShop::RemoveGuest(LPCHARACTER ch)
{
	if (ch->GetOfflineShop() != this)
		return;

	m_map_guest.erase(ch);
	ch->SetOfflineShop(NULL);

	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	ch->GetDesc()->Packet(&pack, sizeof(pack));
}

void COfflineShop::RemoveAllGuest()
{
	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	Broadcast(&pack, sizeof(pack));

	for (auto it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		ch->SetOfflineShop(NULL);
	}
}

void COfflineShop::Destroy(LPCHARACTER npc)
{
	DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u and sold = 1", get_table_postfix(), npc->GetOfflineShopRealOwner());
	DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), npc->GetOfflineShopRealOwner());
	RemoveAllGuest();
	M2_DESTROY_CHARACTER(npc);
}

int COfflineShop::Buy(LPCHARACTER ch, BYTE bPos)
{
	//if (!ch)
	//	return SHOP_SUBHEADER_GC_END;
	
	if (ch->GetOfflineShopOwner()->GetOfflineShopRealOwner() == ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't buy anything from your offline shop."));
		return SHOP_SUBHEADER_GC_OK;
	}

	if (bPos >= m_offlineShopItemVector.size())
	{
		sys_log(0, "COfflineShop::Buy: invalid position %d : %s", bPos, ch->GetName());
		return SHOP_SUBHEADER_GC_INVALID_POS;
	}

	//sys_log(0, "COfflineShop::Buy: name: %s, pos: %d", ch->GetName(), bPos);

	auto it = m_map_guest.find(ch);

	if (it == m_map_guest.end())
		return SHOP_SUBHEADER_GC_END;

	OFFLINE_SHOP_ITEM & r_item = m_offlineShopItemVector[bPos];

	if (r_item.bIsSold)
		return SHOP_SUBHEADER_GC_SOLD_OUT;

	DWORD dwPrice = r_item.price;
	WORD wPrice2 = r_item.price2;
	BYTE bPrice_type = r_item.price_type;

	if (dwPrice < 0)
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;

	if (bPrice_type == 1 && wPrice2 > 0 && wPrice2 > ch->CountSpecifyItem(OFFLINE_SHOP_PRICE_GOLD_BAR_1))
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_BAR_1;

	if (bPrice_type == 2 && wPrice2 > 0 && wPrice2 > ch->CountSpecifyItem(OFFLINE_SHOP_PRICE_GOLD_BAR_2))
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_BAR_2;

	if (bPrice_type == 3 && wPrice2 > 0 && wPrice2 > ch->CountSpecifyItem(OFFLINE_SHOP_PRICE_GOLD_BAR_3))
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_BAR_3;

	if (ch->GetGold() < static_cast<int>(dwPrice))
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;

	LPITEM item = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);

	if (!item)
		return SHOP_SUBHEADER_GC_END;

	int iEmptyPos = 0;

#ifdef ENABLE_SPECIAL_STORAGE
	if (item->IsUpgradeItem())
		iEmptyPos = ch->GetEmptyUpgradeInventory(item);
	else if (item->IsBook())
		iEmptyPos = ch->GetEmptyBookInventory(item);
	else if (item->IsStone())
		iEmptyPos = ch->GetEmptyStoneInventory(item);
#endif
	else
		iEmptyPos = ch->GetEmptyInventory(item->GetSize());

	if (iEmptyPos < 0)
	{
		M2_DESTROY_ITEM(item);
		return SHOP_SUBHEADER_GC_INVENTORY_FULL;
	}

	if (dwPrice > 0)
		ch->PointChange(POINT_GOLD, -dwPrice, false);

	if (wPrice2 > 0)
	{
		if (bPrice_type == 1)
			ch->RemoveSpecifyItem(OFFLINE_SHOP_PRICE_GOLD_BAR_1, wPrice2);
		else if (bPrice_type == 2)
			ch->RemoveSpecifyItem(OFFLINE_SHOP_PRICE_GOLD_BAR_2, wPrice2);
		else if (bPrice_type == 3)
			ch->RemoveSpecifyItem(OFFLINE_SHOP_PRICE_GOLD_BAR_3, wPrice2);
	}

	DBManager::instance().Query("INSERT INTO pazar_gecmis%s (name, alanname, vnum, yang, time) values('%s', '%s', %u, %u, now());", get_table_postfix(), ch->GetOfflineShopOwner()->GetName(), ch->GetName(), item->GetVnum(), dwPrice);

	r_item.bIsSold = 1;
	strlcpy(r_item.buyerName, ch->GetName(), CHARACTER_NAME_MAX_LEN);

	item->SetSockets(r_item.alSockets);
	item->SetAttributes(r_item.aAttr);
	item->SetEvolution(r_item.evolution);

#ifdef ENABLE_SPECIAL_STORAGE
	if (item->IsUpgradeItem())
		item->AddToCharacter(ch, TItemPos(UPGRADE_INVENTORY, iEmptyPos));
	else if (item->IsBook())
		item->AddToCharacter(ch, TItemPos(BOOK_INVENTORY, iEmptyPos));
	else if (item->IsStone())
		item->AddToCharacter(ch, TItemPos(STONE_INVENTORY, iEmptyPos));
#endif
	else
		item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

	if (dwPrice > 0)
		DBManager::instance().DirectQuery("UPDATE player.player SET gold_offlineshop = gold_offlineshop + %u WHERE id = %u", dwPrice, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());

	if (wPrice2 > 0)
	{
		if (bPrice_type == 1)
			DBManager::instance().DirectQuery("UPDATE player.player SET bar1_offlineshop = bar1_offlineshop + %u WHERE id = %u", wPrice2, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
		else if (bPrice_type == 2)
			DBManager::instance().DirectQuery("UPDATE player.player SET bar2_offlineshop = bar2_offlineshop + %u WHERE id = %u", wPrice2, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
		else if (bPrice_type == 3)
			DBManager::instance().DirectQuery("UPDATE player.player SET bar3_offlineshop = bar3_offlineshop + %u WHERE id = %u", wPrice2, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
	}

	DBManager::instance().DirectQuery("UPDATE %soffline_shop_item SET sold = 1 WHERE pos = %d and owner_id = %u", get_table_postfix(), bPos, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
	DBManager::instance().DirectQuery("UPDATE %soffline_shop_item SET buyer_name='%s' WHERE pos = %d and owner_id = %u", get_table_postfix(), ch->GetName(), bPos, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
	LogManager::instance().OfflineShopLog(ch->GetOfflineShopOwner()->GetOfflineShopRealOwnerAccountID(), item->GetName(), "SELL");

	BroadcastUpdateItem(bPos);

	ITEM_MANAGER::instance().FlushDelayedSave(item);

	ch->SetMyOfflineShopTime();
	ch->Save();

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindByPID(ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());

	if (tch)
	{
		char msg[CHAT_MAX_LEN + 1];
		snprintf(msg, sizeof(msg), LC_TEXT("Your item %s sold, buyer is: %s."), item->GetName(), ch->GetName());

		LPDESC pkVictimDesc = tch->GetDesc();

		if (pkVictimDesc)
		{
			TPacketGCWhisper pack;

			int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

			pack.bHeader = HEADER_GC_WHISPER;
			pack.wSize = sizeof(TPacketGCWhisper) + len;
			pack.bType = WHISPER_TYPE_SYSTEM;
			strlcpy(pack.szNameFrom, "[Market Tezgahý]", sizeof(pack.szNameFrom));

			TEMP_BUFFER buf;

			buf.write(&pack, sizeof(TPacketGCWhisper));
			buf.write(msg, len);

			pkVictimDesc->Packet(buf.read_peek(), buf.size());
		}

		//tch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Market Tezgâhýndan nesne satýldý: %s."), item->GetName());
	}
	else
	{
		// P2P_FIND

		TPacketGGOfflineShopMessage p;
		p.bHeader = HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE;
		p.dwTargetPID = ch->GetOfflineShopOwner()->GetOfflineShopRealOwner();
		strlcpy(p.szItemName, item->GetName(), sizeof(p.szItemName));
		strlcpy(p.szName, ch->GetName(), sizeof(p.szName));
		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGOfflineShopMessage));
	}

	if (m_pkOfflineShopNPC && m_pkOfflineShopNPC->IsOfflineShopNPC())
	{
		if (GetLeftItemCount() <= 0)
			m_pkOfflineShopNPC->DestroyOfflineShop();
	}

	return (SHOP_SUBHEADER_GC_OK);
}

void COfflineShop::BroadcastUpdateItem(BYTE bPos)
{
	TPacketGCShop pack;
	TPacketGCShopUpdateOfflineItem pack2;

	TEMP_BUFFER buf;

	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_UPDATE_ITEM;
	pack.size = sizeof(pack) + sizeof(pack2);

	OFFLINE_SHOP_ITEM & r_item = m_offlineShopItemVector[bPos];

	pack2.item.count = r_item.count;
	pack2.item.price = r_item.price;
	pack2.item.price2 = r_item.price2;
	pack2.item.price_type = r_item.price_type;
	pack2.item.vnum = r_item.vnum;
	pack2.item.evolution = r_item.evolution;
	pack2.item.bIsSold = r_item.bIsSold;
	pack2.pos = r_item.pos;
	strlcpy(pack2.item.buyerName, r_item.buyerName, CHARACTER_NAME_MAX_LEN);

	for (int x = 0; x < ITEM_SOCKET_MAX_NUM; x++)
		pack2.item.alSockets[x] = r_item.alSockets[x];

	for (int x = 0; x < ITEM_ATTRIBUTE_MAX_NUM; x++)
	{
		const TPlayerItemAttribute & c_rItemAttr = r_item.aAttr[x];

		pack2.item.aAttr[x].bType = c_rItemAttr.bType;
		pack2.item.aAttr[x].sValue = c_rItemAttr.sValue;
	}

	buf.write(&pack, sizeof(pack));
	buf.write(&pack2, sizeof(pack2));
	Broadcast(buf.read_peek(), buf.size());
}

void COfflineShop::Broadcast(const void * data, int bytes)
{
	for (auto it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		if (ch->GetDesc())
			ch->GetDesc()->Packet(data, bytes);
	}
}

BYTE COfflineShop::GetLeftItemCount()
{
	BYTE bCount = 0;
	for (DWORD i = 0; i < m_offlineShopItemVector.size() && i < OFFLINE_SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const OFFLINE_SHOP_ITEM & item = m_offlineShopItemVector[i];
		if (item.vnum == 0)
			continue;
		if (item.bIsSold == 0)
			bCount++;
	}

	return bCount;
}