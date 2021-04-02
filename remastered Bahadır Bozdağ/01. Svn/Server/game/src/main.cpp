//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offlineshop_manager.h"
#endif

//Search:

	CShopManager	shop_manager;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	COfflineShopManager offlineshop_manager;
#endif