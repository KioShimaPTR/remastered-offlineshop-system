// arat: LPCHARACTER CHARACTER_MANAGER::SpawnMob
// değiştir:
// BEGIN_OFFLINE_SHOP
LPCHARACTER CHARACTER_MANAGER::SpawnMob(DWORD dwVnum, long lMapIndex, long x, long y, long z, bool bSpawnMotion, int iRot, bool bShow, bool isOfflineShopNPC,DWORD real_owner, DWORD real_owner_aid)
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: ch->SetRotation(iRot);
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (isOfflineShopNPC)
	{
		ch->SetOfflineShopNPC(isOfflineShopNPC);
		ch->SetOfflineShopRealOwner(real_owner);
		ch->SetOfflineShopRealOwnerAccountID(real_owner_aid);
	}
	// END_OF_OFFLINE_SHOP