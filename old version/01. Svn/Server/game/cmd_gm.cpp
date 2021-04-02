// arat: sys_log(0, "PURGE: %s %d", pkChr->GetName(), iDist);
// altındaki if i değiştir:
		// BEGIN_OFFLINE_SHOP
		if (!pkChr->IsOfflineShopNPC() && pkChr->GetRaceNum() != 30000 && pkChr->IsNPC() && !pkChr->IsPet() && pkChr->GetRider() == NULL)
		// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: strlcat(buf, ", Shop", sizeof(buf));
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (ch->GetOfflineShop())
		strlcat(buf, ", Offline Shop", sizeof(buf));
	// END_OF_OFFLINE_SHOP