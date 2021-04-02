#include "stdafx.h"
#include "Main.h"
#include "ClientManager.h"

extern DWORD dwOfflineShopCount;

void CClientManager::QUERY_UPDATE_OFFLINE_SHOP_COUNT(TPacketUpdateOfflineShopsCount* p)
{
	// CODE 0 = ADD
	// CODE 1 = SUBTRACT

	if (p->bIncrease)
	{
		dwOfflineShopCount = dwOfflineShopCount + 1;
	}
	else
	{
		if (dwOfflineShopCount > 0) // just in case
			dwOfflineShopCount = dwOfflineShopCount - 1;
	}
}

void CClientManager::QUERY_ADD_OFFLINESHOP_ITEM(TPlayerOfflineShopAddItem* d, CPeer* peer)
{
	char szQuery[QUERY_MAX_LEN];

	snprintf(szQuery, sizeof(szQuery),
			"INSERT INTO %soffline_shop_item (owner_id, pos, count, price, vnum, socket0, socket1, socket2, "
			"attrtype0, attrvalue0,"
			"attrtype1, attrvalue1,"
			"attrtype2, attrvalue2,"
			"attrtype3, attrvalue3,"
			"attrtype4, attrvalue4,"
			"attrtype5, attrvalue5,"
			"attrtype6, attrvalue6)"
			"VALUES (%u, %d, %u, %d, %u, %ld, %ld, %ld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
			GetTablePostfix(),
			d->owner,
			d->pos,
			d->count,
			d->price,
			d->vnum,
			d->alSockets[0],
			d->alSockets[1],
			d->alSockets[2],
			d->aAttr[0].bType, d->aAttr[0].sValue,
			d->aAttr[1].bType, d->aAttr[1].sValue,
			d->aAttr[2].bType, d->aAttr[2].sValue,
			d->aAttr[3].bType, d->aAttr[3].sValue,
			d->aAttr[4].bType, d->aAttr[4].sValue,
			d->aAttr[5].bType, d->aAttr[5].sValue,
			d->aAttr[6].bType, d->aAttr[6].sValue);

	std::auto_ptr<SQLMsg> pmsg_insert(CDBManager::instance().DirectQuery(szQuery));

	sys_log(0, "COfflineShopManager::QUERY_ADD_OFFLINESHOP_ITEM OwnerID: %u, Pos: %d, Count: %u, Price: %d", d->owner, d->pos, d->count, d->price);
}

void CClientManager::QUERY_DEL_OFFLINESHOP_ITEM(TPlayerOfflineShopAddItem* d, CPeer* peer)
{
	char szQuery[QUERY_MAX_LEN];
}