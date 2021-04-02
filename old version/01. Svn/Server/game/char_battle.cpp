// arat: #include "DragonLair.h"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "offlineshop.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: CloseSafebox();
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (GetOfflineShop())
	{
		GetOfflineShop()->RemoveGuest(this);
		SetOfflineShop(NULL);
	}
	// END_OF_OFFLINE_SHOP