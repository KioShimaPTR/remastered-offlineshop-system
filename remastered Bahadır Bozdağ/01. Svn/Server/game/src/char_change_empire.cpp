//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "desc.h"
#include "offlineshop_manager.h"
#endif

//Search:

int CHARACTER::ChangeEmpire(BYTE empire)
{

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (COfflineShopManager::instance().HaveOfflineShopOnAccount(GetDesc()->GetAccountTable().id))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't change your empire while you have open offlineshop."));
		return 1;
	}
#endif