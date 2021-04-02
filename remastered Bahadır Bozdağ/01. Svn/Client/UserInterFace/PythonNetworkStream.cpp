//Search:

			Set(HEADER_GC_SHOP_SIGN,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShopSign), STATIC_SIZE_PACKET));

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			Set(HEADER_GC_OFFLINE_SHOP,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShop), DYNAMIC_SIZE_PACKET));
			Set(HEADER_GC_OFFLINE_SHOP_SIGN,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShopSign), STATIC_SIZE_PACKET));
#endif