//Add it somewhere convenient

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
ACMD(do_open_offline_shop);
ACMD(do_destroy_offlineshop);
#endif

//Add it somewhere convenient

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	{ "open_offlineshop", do_open_offline_shop, 0, POS_DEAD, GM_PLAYER		},
	{ "destroy_offline_shop", do_destroy_offlineshop, 	0, POS_DEAD,		GM_LOW_WIZARD},
#endif