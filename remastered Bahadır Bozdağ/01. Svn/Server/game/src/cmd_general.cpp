//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offlineshop_manager.h"
#endif

//Add to end:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
ACMD(do_open_offline_shop)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	BYTE status = 0;
	str_to_number(status, arg1);

	if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetOfflineShop() || ch->GetMyShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
		return;
	}

	if (!COfflineShopManager::instance().MapCheck(ch->GetMapIndex()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that map!"));
		return;
	}

	if (!COfflineShopManager::instance().ChannelCheck(g_bChannel))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that channel!"));
		return;
	}

	COfflineShopManager::instance().ResetOfflineShopStatus(ch);

	if (status)
		ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenOfflineShop");
}