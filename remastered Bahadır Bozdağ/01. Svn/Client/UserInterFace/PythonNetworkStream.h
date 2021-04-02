//Search:

		bool SendShopSellPacketNew(BYTE bySlot, BYTE byCount);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		// Offline Shop
		bool SendOfflineShopEndPacket();
		bool SendDestroyOfflineShop();
		bool SendRefreshOfflineShopPrice();
		bool SendOfflineShopBuyPacket(BYTE byCount);
		bool SendUnsoldItems();
		bool SendOfflineShopWithdrawMoney(DWORD dwMoney);
		bool SendOfflineShopWithdrawBar1(WORD wBar1);
		bool SendOfflineShopWithdrawBar2(WORD wBar2);
		bool SendOfflineShopWithdrawBar3(WORD wBar3);
		bool SendOfflineShopFetchItem(BYTE bDisplayPos);
		bool SendOfflineShopTeleport();
#endif

//Search:

		bool SendBuildPrivateShopPacket(const char * c_szName, const std::vector<TShopItemTable> & c_rSellingItemStock);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		// Offline Shop
		void DisappearOfflineShopSign(DWORD dwVID);
		void AppearOfflineShopSign(DWORD dwVID, std::string stSign = "");
		bool SendBuildOfflineShopPacket(const char * c_szName, const std::vector<TShopItemTable2> & c_rSellingItemStock, BYTE bTime);
#endif

//Search:

		bool RecvExchangePacket();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		bool RecvOfflineShopPacket();
		bool RecvOfflineShopSignPacket();
#endif