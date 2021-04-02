// arat: if ( IsOpenSafebox() || GetShopOwner() || GetMyShop() || IsCubeOpen())
// değiştir:
	// BEGIN_OFFLINE_SHOP
	if ( IsOpenSafebox() || GetShopOwner() || GetMyShop() || IsCubeOpen() || GetOfflineShopOwner())
	// END_OF_OFFLINE_SHOP

// arat: if ( victim->IsOpenSafebox() || victim->GetShopOwner() || victim->GetMyShop() || victim->IsCubeOpen())
// değiştir:
	// BEGIN_OFFLINE_SHOP
	if ( victim->IsOpenSafebox() || victim->GetShopOwner() || victim->GetMyShop() || victim->IsCubeOpen() || victim->GetOfflineShopOwner())
	// END_OF_OFFLINE_SHOP