// arat: BOOL GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData);
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		void SetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData & c_rShopItemData);
		BOOL GetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData ** c_ppItemData);

		void SetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData & c_rShopItemData);
		BOOL GetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData ** c_ppItemData);
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void Open
// değiştir:
		// BEGIN_OFFLINE_SHOP
		void Open(BOOL isPrivateShop, BOOL isMainPrivateShop, BOOL isOfflineShop);
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: BOOL IsMainPlayerPrivateShop();
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		BOOL IsOfflineShop();
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void BuildPrivateShop(const char * c_szName);
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		void ClearOfflineShopStock();
		void AddOfflineShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice);
		void DelOfflineShopItemStock(TItemPos ItemPos);
		int	 GetOfflineShopItemPrice(TItemPos ItemPos);
		void BuildOfflineShop(const char * c_szName, BYTE bTime);
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: BOOL				m_isMainPlayerPrivateShop;
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		BOOL				m_isOfflineShop;
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: struct ShopTab
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		struct OfflineShopTab
		{
			OfflineShopTab()
			{
				coinType = SHOP_COIN_TYPE_GOLD;
			}
			BYTE				coinType;
			std::string			name;
			TShopOfflineItemData		items[OFFLINE_SHOP_HOST_ITEM_MAX_NUM];
		};
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		OfflineShopTab m_aOfflineShoptabs[SHOP_TAB_COUNT_MAX];
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: typedef std::map<TItemPos, TShopItemTable> TPrivateShopItemStock;
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		typedef std::map<TItemPos, TShopItemTable> TOfflineShopItemStock;
		TOfflineShopItemStock	m_OfflineShopItemStock;
		// END_OF_OFFLINE_SHOP