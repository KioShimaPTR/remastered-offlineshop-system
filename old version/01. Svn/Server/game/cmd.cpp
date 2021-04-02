// arat: ACMD (do_clear_affect);
// altına ekle:
// BEGIN_OFFLINE_SHOP
ACMD (do_open_offline_shop);
ACMD (do_withdraw_offline_shop_money);
ACMD (do_retrieve_offline_shop_item);
ACMD (do_destroy_offlineshop);
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: do_clear_affect
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	{ "open_offlineshop", do_open_offline_shop, 0, POS_DEAD, GM_PLAYER		},
	{ "withdraw_offline_shop_money", do_withdraw_offline_shop_money, 0, POS_DEAD, GM_PLAYER	},
	{ "retrieve_offline_shop_item", do_retrieve_offline_shop_item, 0, POS_DEAD, GM_PLAYER },
	{ "destroy_offline_shop", do_destroy_offlineshop, 	0, POS_DEAD,		GM_LOW_WIZARD},
	// END_OF_OFFLINE_SHOP