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
	m_pGrid = M2_NEW CGrid(8, 8);
}

COfflineShop::~COfflineShop()
{
	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	Broadcast(&pack, sizeof(pack));

	for (GuestMapType::iterator it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		ch->SetOfflineShop(NULL);
	}

	M2_DELETE(m_pGrid);
}

void COfflineShop::SetOfflineShopNPC(LPCHARACTER npc)
{
	m_pkOfflineShopNPC = npc;
}

void COfflineShop::SetShopItems(TShopItemTable * pTable, BYTE bItemCount, LPCHARACTER ch)
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
		offShopItem.vnum = pkItem->GetVnum();

		for (int x = 0; x < ITEM_SOCKET_MAX_NUM; x++)
			offShopItem.alSockets[x] = pkItem->GetSocket(x);

		for (int x = 0; x < ITEM_ATTRIBUTE_MAX_NUM; x++)
		{
			offShopItem.aAttr[x].bType = pkItem->GetAttributeType(x);
			offShopItem.aAttr[x].sValue = pkItem->GetAttributeValue(x);
		}
		/* memory part end */

		/* query part begin */
		char szColumns[QUERY_MAX_LEN], szValues[QUERY_MAX_LEN];

		snprintf(szColumns, sizeof(szColumns),
		"owner_id, pos, count, price, vnum,"
		"socket0, socket1, socket2,"
		"attrtype0, attrvalue0, attrtype1, attrvalue1,"
		"attrtype2, attrvalue2, attrtype3, attrvalue3,"
		"attrtype4, attrvalue4, attrtype5, attrvalue5,"
		"attrtype6, attrvalue6");
		
		snprintf(szValues, sizeof(szValues), "%u, %d, %u, %u, %u, %ld, %ld, %ld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
			ch->GetPlayerID(), iPos, pkItem->GetCount(), pTable->price, pkItem->GetVnum(),
			pkItem->GetSocket(0), pkItem->GetSocket(1),
			pkItem->GetSocket(2),
			pkItem->GetAttributeType(0), pkItem->GetAttributeValue(0), pkItem->GetAttributeType(1), pkItem->GetAttributeValue(1),
			pkItem->GetAttributeType(2), pkItem->GetAttributeValue(2), pkItem->GetAttributeType(3), pkItem->GetAttributeValue(3),
			pkItem->GetAttributeType(4), pkItem->GetAttributeValue(4), pkItem->GetAttributeType(5), pkItem->GetAttributeValue(5),
			pkItem->GetAttributeType(6), pkItem->GetAttributeValue(6));

		char szInsertQuery[QUERY_MAX_LEN];
		snprintf(szInsertQuery, sizeof(szInsertQuery), "INSERT INTO %soffline_shop_item (%s) VALUES (%s)", get_table_postfix(), szColumns, szValues);
		std::auto_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szInsertQuery));
		/* query part end */

		/* log for web */
		LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pkItem->GetName(), "PUT");
		/* end log for web */

		ITEM_MANAGER::instance().RemoveItem(pkItem);

		++pTable;
	}
}

void COfflineShop::PutItem(LPCHARACTER ch, TItemPos item_pos, int iPrice)
{
	if (!ch || !ch->CanHandleItem())
		return;

	LPITEM pkItem = ch->GetItem(item_pos);

	if (!pkItem)
		return;

	if (pkItem->IsEquipped() == true || pkItem->isLocked() == true || pkItem->IsExchanging())
		return;

	const TItemTable * item_table = pkItem->GetProto();

	if (!item_table)
		return;

	if (IS_SET(item_table->dwAntiFlags, ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_MYSHOP))
		return;

	int iPos = m_pGrid->FindBlank(1, item_table->bSize);

	if (iPos < 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough space to put your item!"));
		return;
	}

	if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough space to put your item!"));
		return;
	}

	m_pGrid->Put(iPos, 1, item_table->bSize);

	/* memory part begin */
	OFFLINE_SHOP_ITEM & offShopItem = m_offlineShopItemVector[iPos];

	//////////
	// bIsSold already declared as = false in offlineshop.h
	offShopItem.owner_id = ch->GetPlayerID();
	offShopItem.pos = iPos;
	offShopItem.count = pkItem->GetCount();
	offShopItem.price = iPrice;
	offShopItem.vnum = pkItem->GetVnum();

	for (int x = 0; x < ITEM_SOCKET_MAX_NUM; x++)
		offShopItem.alSockets[x] = pkItem->GetSocket(x);

	for (int x = 0; x < ITEM_ATTRIBUTE_MAX_NUM; x++)
	{
		offShopItem.aAttr[x].bType = pkItem->GetAttributeType(x);
		offShopItem.aAttr[x].sValue = pkItem->GetAttributeValue(x);
	}
	/* memory part end */
	
	/* query part begin */
	char szColumns[QUERY_MAX_LEN], szValues[QUERY_MAX_LEN];

	snprintf(szColumns, sizeof(szColumns),
		"owner_id, pos, count, price, vnum,"
		"socket0, socket1, socket2,"
		"attrtype0, attrvalue0, attrtype1, attrvalue1,"
		"attrtype2, attrvalue2, attrtype3, attrvalue3,"
		"attrtype4, attrvalue4, attrtype5, attrvalue5,"
		"attrtype6, attrvalue6");

	snprintf(szValues, sizeof(szValues), "%u, %d, %u, %u, %u, %ld, %ld, %ld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		ch->GetPlayerID(), iPos, pkItem->GetCount(), iPrice, pkItem->GetVnum(),
		pkItem->GetSocket(0), pkItem->GetSocket(1),
		pkItem->GetSocket(2),
		pkItem->GetAttributeType(0), pkItem->GetAttributeValue(0), pkItem->GetAttributeType(1), pkItem->GetAttributeValue(1),
		pkItem->GetAttributeType(2), pkItem->GetAttributeValue(2), pkItem->GetAttributeType(3), pkItem->GetAttributeValue(3),
		pkItem->GetAttributeType(4), pkItem->GetAttributeValue(4), pkItem->GetAttributeType(5), pkItem->GetAttributeValue(5),
		pkItem->GetAttributeType(6), pkItem->GetAttributeValue(6));

	char szInsertQuery[QUERY_MAX_LEN];
	snprintf(szInsertQuery, sizeof(szInsertQuery), "INSERT INTO %soffline_shop_item (%s) VALUES (%s)", get_table_postfix(), szColumns, szValues);
	std::auto_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szInsertQuery));
	/* query part end */

	/* log for web */
	LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pkItem->GetName(), "PUT_REMOTE");
	/* end log for web */

	BroadcastUpdateItem(iPos);

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Item %s has been successfully added to your offline shop, price: %d."), pkItem->GetName(), iPrice);
	
	ITEM_MANAGER::instance().RemoveItem(pkItem);
}

bool COfflineShop::AddGuest(LPCHARACTER ch, LPCHARACTER npc)
{
	if (!ch || ch->GetExchange() || ch->GetShop() || ch->GetMyShop() || ch->GetOfflineShop())
		return false;

	ch->SetOfflineShop(this);
	m_map_guest.insert(GuestMapType::value_type(ch, false));

	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_START;

	TPacketGCOfflineShopStart pack2;
	memset(&pack2, 0, sizeof(pack2));
	pack2.owner_vid = npc->GetVID();

	for (DWORD i = 0; i < m_offlineShopItemVector.size() && i < OFFLINE_SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const OFFLINE_SHOP_ITEM & offShopItem = m_offlineShopItemVector[i];

		pack2.items[i].count = offShopItem.count;
		pack2.items[i].price = offShopItem.price;
		pack2.items[i].vnum = offShopItem.vnum;
		pack2.items[i].bIsSold = offShopItem.bIsSold;
		strncpy(pack2.items[i].buyerName, offShopItem.buyerName, CHARACTER_NAME_MAX_LEN);

		for (int x = 0; x < ITEM_SOCKET_MAX_NUM; x++)
			pack2.items[i].alSockets[x] = offShopItem.alSockets[x];
	
		for (int x = 0; x < ITEM_ATTRIBUTE_MAX_NUM; x++)
		{
			const TPlayerItemAttribute & c_rItemAttr = offShopItem.aAttr[x];
			
			pack2.items[i].aAttr[x].bType = c_rItemAttr.bType;
			pack2.items[i].aAttr[x].sValue = c_rItemAttr.sValue;
		}
	}

	pack.size = sizeof(pack)+sizeof(pack2);
	ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
	ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCOfflineShopStart));
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

	for (GuestMapType::iterator it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		ch->SetOfflineShop(NULL);
	}
}

void COfflineShop::Destroy(LPCHARACTER npc)
{
	RemoveAllGuest();
	M2_DESTROY_CHARACTER(npc);
}

void COfflineShop::GiveBackMyItems(LPCHARACTER ch)
{
	if (!ch)
		return;

	RemoveAllGuest();

	/* cleanup begin */
	DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
	/* cleanup end */

	for (DWORD i = 0; i < m_offlineShopItemVector.size() && i < OFFLINE_SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const OFFLINE_SHOP_ITEM & r_item = m_offlineShopItemVector[i];

		if (r_item.bIsSold || !r_item.vnum)
			continue;

		LPITEM pItem = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);
		
		if (pItem)
		{
			pItem->SetSockets(r_item.alSockets);
			pItem->SetAttributes(r_item.aAttr);

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

			/* log for web */
			LogManager::instance().OfflineShopLog(ch->GetDesc()->GetAccountTable().id, pItem->GetName(), "GIVE_BACK");
			/* end log for web */
		}
	}
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

	GuestMapType::iterator it = m_map_guest.find(ch);
	if (it == m_map_guest.end())
		return SHOP_SUBHEADER_GC_END;

	OFFLINE_SHOP_ITEM & r_item = m_offlineShopItemVector[bPos];

	if (r_item.bIsSold)
		return SHOP_SUBHEADER_GC_SOLD_OUT;

	DWORD dwPrice = r_item.price;

	if (r_item.price < 0)
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;

	if (ch->GetGold() < static_cast<int>(dwPrice))
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;

	LPITEM item = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);

	if (!item)
		return SHOP_SUBHEADER_GC_END;

	int iEmptyPos = ch->GetEmptyInventory(item->GetSize());

	if (iEmptyPos < 0)
	{
		M2_DESTROY_ITEM(item);
		return SHOP_SUBHEADER_GC_INVENTORY_FULL;
	}

	ch->PointChange(POINT_GOLD, -dwPrice, false);
	r_item.bIsSold = true;
	strncpy(r_item.buyerName, ch->GetName(), CHARACTER_NAME_MAX_LEN);

	item->SetSockets(r_item.alSockets);
	item->SetAttributes(r_item.aAttr);

	item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

	DBManager::instance().DirectQuery("UPDATE player.player SET gold_offlineshop = gold_offlineshop + %u WHERE id = %u", dwPrice, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
	DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u AND pos = %d AND vnum = %d LIMIT 1", get_table_postfix(), r_item.owner_id, bPos, r_item.vnum);

	LogManager::instance().OfflineShopLog(ch->GetOfflineShopOwner()->GetOfflineShopRealOwnerAccountID(), item->GetName(), "SELL");

	BroadcastUpdateItem(bPos);

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
	pack2.item.vnum = r_item.vnum;
	pack2.item.bIsSold = r_item.bIsSold;
	pack2.pos = r_item.pos;
	strncpy(pack2.item.buyerName, r_item.buyerName, CHARACTER_NAME_MAX_LEN);

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
	for (GuestMapType::iterator it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		if (ch->GetDesc())
			ch->GetDesc()->Packet(data, bytes);
	}
}
