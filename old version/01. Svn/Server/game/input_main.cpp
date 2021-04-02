// arat: #include "DragonSoul.h"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "offlineshop.h"
#include "offlineshop_manager.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: int CInputMain::Shop(LPCHARACTER ch, const char * data, size_t uiBytes)
// kod bloğundan sonra ekle:
// BEGIN_OFFLINE_SHOP
int CInputMain::OfflineShop(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	TPacketCGShop * p = (TPacketCGShop *)data;

	if (uiBytes < sizeof(TPacketCGShop))
		return -1;

	if (test_server)
		sys_log(0, "CInputMain::OfflineShop ==> SubHeader %d", p->subheader);

	const char * c_pData = data + sizeof(TPacketCGShop);
	uiBytes -= sizeof(TPacketCGShop);

	switch (p->subheader)
	{
		case SHOP_SUBHEADER_CG_END:
		{
			sys_log(1, "INPUT: %s OFFLINE_SHOP: END", ch->GetName());
			COfflineShopManager::instance().StopShopping(ch);
			return 0;
		}
		case SHOP_SUBHEADER_CG_BUY:
		{
			if (uiBytes < sizeof(BYTE) + sizeof(BYTE))
				return -1;

			BYTE bPos = *(c_pData + 1);
			sys_log(1, "INPUT: %s OFFLINE_SHOP: BUY %d", ch->GetName(), bPos);
			COfflineShopManager::instance().Buy(ch, bPos);
			return (sizeof(BYTE) + sizeof(BYTE));
		}
		case SHOP_SUBHEADER_CG_PUT_ITEM:
		{
			if (uiBytes < sizeof(TOfflineShopItemTable))
				return -1;

			TOfflineShopItemTable * pTable = (TOfflineShopItemTable *)(c_pData);
			COfflineShopManager::instance().PutItem(ch, pTable->item_pos, pTable->iPrice);
			return (sizeof(TOfflineShopItemTable));
		}
		case SHOP_SUBHEADER_CG_DESTROY_OFFLINE_SHOP:
		{
			sys_log(1, "INPUT: %s OFFLINE_SHOP_DESTROY", ch->GetName());
			COfflineShopManager::instance().DestroyOfflineShop(ch, 0, true);
			return 0;
		}
		default:
		{
			sys_err("CInputMain::OfflineShop : Unknown subheader %d : %s", p->subheader, ch->GetName());
			break;
		}
	}

	return 0;
}
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (ch->GetMyShop() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen())
// değiştir:
					// BEGIN_OFFLINE_SHOP
					if (ch->GetMyShop() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
					// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen())
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: int CInputMain::MyShop(LPCHARACTER ch, const char * c_pData, size_t uiBytes)
// kod bloğundan sonra ekle:
// BEGIN_OFFLINE_SHOP
int CInputMain::MyOfflineShop(LPCHARACTER ch, const char * c_pData, size_t uiBytes)
{
	TPacketCGMyOfflineShop * p = (TPacketCGMyOfflineShop *)c_pData;
	int iExtraLen = p->bCount * sizeof(TShopItemTable);

	if (uiBytes < sizeof(TPacketCGMyOfflineShop) + iExtraLen)
		return -1;

	if (ch->IsStun() || ch->IsDead())
		return (iExtraLen);

	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중일경우 개인상점을 열수가 없습니다."));
		return (iExtraLen);
	}

	sys_log(0, "MyOfflineShop count %d", p->bCount);
	ch->OpenMyOfflineShop(p->szSign, (TShopItemTable *)(c_pData + sizeof(TPacketCGMyOfflineShop)), p->bCount, p->bTime);
	return (iExtraLen);
}
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->GetMyShop() || ch->IsCubeOpen())
// değiştir:
	// BEGIN_OFFLINE_SHOP
	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->GetMyShop() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: case HEADER_CG_SHOP:
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		case HEADER_CG_OFFLINE_SHOP:
			if ((iExtraLen = OfflineShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
		// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: case HEADER_CG_MYSHOP:
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		case HEADER_CG_MY_OFFLINE_SHOP:
			if ((iExtraLen = MyOfflineShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
		// END_OF_OFFLINE_SHOP