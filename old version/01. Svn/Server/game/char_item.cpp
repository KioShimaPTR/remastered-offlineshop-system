// arat: if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
// değiştir:
		// BEGIN_OFFLINE_SHOP
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
		// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		if (iPulse - GetMyOfflineShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개인상점 사용후 %d초 이내에는 귀환부,귀환기억부를 사용할 수 없습니다."), g_nPortalLimitTime);
			return false;
		}
		// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (item->GetVnum() == 50200 | item->GetVnum() == 71049)
// içerisinde değiştir: if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
		// BEGIN_OFFLINE_SHOP
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
		// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (IsWarping())	return false;
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (GetOfflineShop()) return false;
	// END_OF_OFFLINE_SHOP