// arat: bool SendShopSellPacketNew(BYTE bySlot, BYTE byCount);
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		bool SendOfflineShopEndPacket();
		bool SendDestroyOfflineShop();
		bool SendOfflineShopBuyPacket(BYTE byCount);
		bool SendAddOfflineShopItemPacket(TItemPos item_pos, int iPrice);
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: bool SendBuildPrivateShopPacket(const char * c_szName, const std::vector<TShopItemTable> & c_rSellingItemStock);
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		bool SendBuildOfflineShopPacket(const char * c_szName, const std::vector<TShopItemTable> & c_rSellingItemStock, BYTE bTime);
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: bool RecvExchangePacket();
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		bool RecvOfflineShopPacket();
		bool RecvOfflineShopSignPacket();
		// END_OF_OFFLINE_SHOP