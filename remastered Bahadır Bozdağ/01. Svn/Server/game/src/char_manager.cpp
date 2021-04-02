//Search:

LPCHARACTER CHARACTER_MANAGER::SpawnMob(DWORD dwVnum, long lMapIndex, long x, long y, long z, bool bSpawnMotion, int iRot, bool bShow)

//Change:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
LPCHARACTER CHARACTER_MANAGER::SpawnMob(DWORD dwVnum, long lMapIndex, long x, long y, long z, bool bSpawnMotion, int iRot, bool bShow, bool isOfflineShopNPC, DWORD real_owner, DWORD real_owner_aid)
#else
LPCHARACTER CHARACTER_MANAGER::SpawnMob(DWORD dwVnum, long lMapIndex, long x, long y, long z, bool bSpawnMotion, int iRot, bool bShow)
#endif

//Search:

	if (pkMob->m_table.bType == CHAR_TYPE_NPC)
		if (ch->GetEmpire() == 0)
			ch->SetEmpire(SECTREE_MANAGER::instance().GetEmpireFromMapIndex(lMapIndex));

	ch->SetRotation(iRot);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (isOfflineShopNPC)
	{
		ch->SetOfflineShopNPC(isOfflineShopNPC);
		ch->SetOfflineShopRealOwner(real_owner);
		ch->SetOfflineShopRealOwnerAccountID(real_owner_aid);
	}
#endif