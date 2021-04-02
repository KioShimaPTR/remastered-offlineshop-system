// arat: #include "log.h"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "offlineshop_manager.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// en alta "endif" ten sonra ekle:
// BEGIN_OFFLINE_SHOP
ACMD(do_open_offline_shop)
{
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

	COfflineShopManager::instance().ResetOfflineShopStatus(ch);

	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenOfflineShop");
}

ACMD(do_withdraw_offline_shop_money)
{
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetOfflineShop() || ch->IsCubeOpen() || ch->IsOpenSafebox() || ch->GetShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
		return;
	}

	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
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

	ch->SetMyOfflineShopTime();

	COfflineShopManager::instance().WithdrawAllMoney(ch);
}

ACMD(do_retrieve_offline_shop_item)
{
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetOfflineShop() || ch->IsCubeOpen() || ch->IsOpenSafebox() || ch->GetShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
		return;
	}

	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
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

	if (ch->IsAffectFlag(AFF_SHOPOWNER))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have to close your shop before use this!"));
		return;
	}

	ch->SetMyOfflineShopTime();

	COfflineShopManager::instance().FetchMyItems(ch);
}

ACMD(do_destroy_offlineshop)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD vid = 0;
	str_to_number(vid, arg1);

	LPCHARACTER npc = CHARACTER_MANAGER::instance().Find(vid);

	if (!npc)
	{
		sys_err("do_destroy_offlineshop can't find the npc vid: %d", vid);
		return;
	}

	if (!npc->IsOfflineShopNPC())
	{
		sys_err("do_destroy_offlineshop: the VID isn't OfflineShopNPC: %d", vid);
		return;
	}

	COfflineShopManager::instance().DestroyOfflineShop(NULL, vid, false);
	M2_DESTROY_CHARACTER(npc);
}
// END_OF_OFFLINE_SHOP