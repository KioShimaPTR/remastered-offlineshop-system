// arat: HEADER_GC_DRAGON_SOUL_REFINE
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			Set(HEADER_GC_OFFLINE_SHOP,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShop), DYNAMIC_SIZE_PACKET));
			Set(HEADER_GC_OFFLINE_SHOP_SIGN,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShopSign), STATIC_SIZE_PACKET));
			// END_OF_OFFLINE_SHOP