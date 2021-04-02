//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offlineshop.h"
#endif

//Search:

	CShopManager::instance().StopShopping(this);
	CloseMyShop();
	CloseSafebox();

//Add Below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (GetOfflineShop())
	{
		GetOfflineShop()->RemoveGuest(this);
		SetOfflineShop(NULL);
	}
#endif