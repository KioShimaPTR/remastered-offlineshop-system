// arat: if ( ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen())
// değiştir:
	// BEGIN_OFFLINE_SHOP
	if ( ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	// END_OF_OFFLINE_SHOP