// arat: #include "ClientPackageCryptInfo.h"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "questmanager.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: void DESC_MANAGER::SendClientPackageSDBToLoadMap( LPDESC desc, const char* pMapName )
// kod bloğunun altına ekle:
// BEGIN_OFFLINE_SHOP
struct online_count_packet_func
{
	const DWORD dwOnlinePlayers;
	const DWORD dwOnlineShops;

	online_count_packet_func(const DWORD dwOnlinePlayers, const DWORD dwOnlineShops) : dwOnlinePlayers(dwOnlinePlayers), dwOnlineShops(dwOnlineShops) {}

	void operator () (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "CurrentPlayerOnline %d",
			dwOnlinePlayers + quest::CQuestManager::instance().GetEventFlag("online_count"));

		d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "CurrentShopOnline %d",
			dwOnlineShops + quest::CQuestManager::instance().GetEventFlag("shop_count"));
	}
};

void DESC_MANAGER::BroadcastOnlineCount(DWORD dwOnlinePlayers, DWORD dwOnlineShops)
{
	const DESC_SET & c_ref_set = GetClientSet();
	std::for_each(c_ref_set.begin(), c_ref_set.end(), online_count_packet_func(dwOnlinePlayers, dwOnlineShops));
}
// END_OF_OFFLINE_SHOP