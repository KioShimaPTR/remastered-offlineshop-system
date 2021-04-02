//Search:

	Set(HEADER_CG_MYSHOP, sizeof(TPacketCGMyShop), "MyShop", false);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	Set(HEADER_CG_OFFLINE_SHOP, sizeof(TPacketCGShop), "OfflineShop", false);
	Set(HEADER_CG_MY_OFFLINE_SHOP, sizeof(TPacketCGMyOfflineShop), "MyOfflineShop", false);
#endif

//Search:

	Set(HEADER_GG_CHECK_AWAKENESS,		sizeof(TPacketGGCheckAwakeness),	"CheckAwakeness",		false);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	Set(HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE,		sizeof(TPacketGGOfflineShopMessage),		"OfflineShopUpdateMessage", false);
#endif