//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offlineshop.h"
#include "offlineshop_manager.h"
#endif

//Search:

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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
		case SHOP_SUBHEADER_CG_DESTROY_OFFLINE_SHOP:
		{
			sys_log(1, "INPUT: %s OFFLINE_SHOP_DESTROY", ch->GetName());
			COfflineShopManager::instance().DestroyOfflineShop(ch, 0, true);
			return 0;
		}
		case SHOP_SUBHEADER_CG_REFRESH_PRICE:
		{
			sys_log(0, "INPUT: %s OFFLINE_SHOP_REFRESH_MONEY", ch->GetName());
			COfflineShopManager::instance().RefreshPrice(ch);
			return 0;
		}
		case SHOP_SUBHEADER_CG_WITHDRAW_MONEY:
		{
			if (uiBytes < sizeof(DWORD))
				return -1;

			const DWORD gold = *reinterpret_cast<const DWORD*>(c_pData);		
			sys_log(0, "INPUT: %s(%d) OFFLINE_SHOP_WITHDRAW_MONEY", ch->GetName(), gold);	
			COfflineShopManager::instance().WithdrawMoney(ch, gold);
			return (sizeof(DWORD));
		}
		case SHOP_SUBHEADER_CG_WITHDRAW_BAR_1:
		{
			if (uiBytes < sizeof(WORD))
				return -1;

			const WORD bar = *reinterpret_cast<const WORD*>(c_pData);		
			sys_log(0, "INPUT: %s(%d) SHOP_SUBHEADER_CG_WITHDRAW_BAR_1", ch->GetName(), bar);	
			COfflineShopManager::instance().WithdrawBar1(ch, bar);
			return (sizeof(WORD));	
		}
		case SHOP_SUBHEADER_CG_WITHDRAW_BAR_2:
		{
			if (uiBytes < sizeof(WORD))
				return -1;

			const WORD bar = *reinterpret_cast<const WORD*>(c_pData);		
			sys_log(0, "INPUT: %s(%d) SHOP_SUBHEADER_CG_WITHDRAW_BAR_2", ch->GetName(), bar);	
			COfflineShopManager::instance().WithdrawBar2(ch, bar);
			return (sizeof(WORD));	
		}
		case SHOP_SUBHEADER_CG_WITHDRAW_BAR_3:
		{
			if (uiBytes < sizeof(WORD))
				return -1;

			const WORD bar = *reinterpret_cast<const WORD*>(c_pData);		
			sys_log(0, "INPUT: %s(%d) SHOP_SUBHEADER_CG_WITHDRAW_BAR_3", ch->GetName(), bar);	
			COfflineShopManager::instance().WithdrawBar3(ch, bar);
			return (sizeof(WORD));
		}
		case SHOP_SUBHEADER_CG_UNSOLD_ITEMS:
		{
			if (ch->IsAffectFlag(AFF_SHOPOWNER))
				return 0;

			sys_log(0, "INPUT: %s OFFLINE_SHOP_UNSOLD_ITEMS", ch->GetName());
			COfflineShopManager::instance().UnsoldItems(ch);
			return 0;
		}
		case SHOP_SUBHEADER_CG_FETCH_ITEM:
		{
			if (ch->IsAffectFlag(AFF_SHOPOWNER))
				return 0;

			if (uiBytes < sizeof(BYTE))
				return -1;

			BYTE bPos = *c_pData;
			sys_log(0, "INPUT: %s OFFLINE_SHOP_TAKE_ITEM", ch->GetName());
			COfflineShopManager::instance().FetchMyItems(ch, bPos);
			return (sizeof(BYTE));
		}
		case SHOP_SUBHEADER_CG_TELEPORT:
		{
			if (!ch->IsAffectFlag(AFF_SHOPOWNER))
				return 0;

			if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetOfflineShop() || ch->GetMyShop())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
				return 0;
			}

			if (!COfflineShopManager::instance().MapCheck(ch->GetMapIndex()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that map!"));
				return 0;
			}

			if (!COfflineShopManager::instance().ChannelCheck(g_bChannel))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that channel!"));
				return 0;
			}

			auto npc = CHARACTER_MANAGER::instance().Find(COfflineShopManager::instance().FindMyOfflineShop(ch->GetPlayerID()));

			if (!npc)
				return 0;

			ch->WarpSet(npc->GetX(), npc->GetY());
			ch->Stop();

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
#endif

//Search:


void CInputMain::Refine(LPCHARACTER ch, const char* c_pData)

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
int CInputMain::MyOfflineShop(LPCHARACTER ch, const char * c_pData, size_t uiBytes)
{
	TPacketCGMyOfflineShop * p = (TPacketCGMyOfflineShop *)c_pData;
	int iExtraLen = p->bCount * sizeof(TShopItemTable2);

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
	ch->OpenMyOfflineShop(p->szSign, (TShopItemTable2 *)(c_pData + sizeof(TPacketCGMyOfflineShop)), p->bCount, p->bTime);
	return (iExtraLen);
}
#endif

//Search:

		case HEADER_CG_SHOP:
			if ((iExtraLen = Shop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		case HEADER_CG_OFFLINE_SHOP:
			if ((iExtraLen = OfflineShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif

//Search:

		case HEADER_CG_MYSHOP:
			if ((iExtraLen = MyShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		case HEADER_CG_MY_OFFLINE_SHOP:
			if ((iExtraLen = MyOfflineShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif