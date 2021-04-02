//Search:

		BOOL GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData);

//Add Below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void SetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData & c_rShopItemData);
		BOOL GetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData ** c_ppItemData);

		void SetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData & c_rShopItemData);
		BOOL GetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData ** c_ppItemData);
#endif

//Search:

		BOOL IsMainPlayerPrivateShop();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		BOOL IsOfflineShop();
#endif

//Search:

		void BuildPrivateShop(const char * c_szName);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void ClearOfflineShopStock();
		void AddOfflineShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice, WORD wPrice2, BYTE bPrice_type);
		void DelOfflineShopItemStock(TItemPos ItemPos);
		int	 GetOfflineShopItemPrice(TItemPos ItemPos);
		WORD GetOfflineShopItemPrice2(TItemPos ItemPos);
		BYTE GetOfflineShopItemPriceType(TItemPos ItemPos);
		void BuildOfflineShop(const char * c_szName, BYTE bTime);
#endif

//Search:

		BOOL				m_isMainPlayerPrivateShop;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		BOOL				m_isOfflineShop;
#endif

//Search:

		struct ShopTab
		{
			ShopTab()
			{
				coinType = SHOP_COIN_TYPE_GOLD;
			}
			BYTE				coinType;
			std::string			name;
			TShopItemData		items[SHOP_HOST_ITEM_MAX_NUM];
		};

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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
#endif

//Search:

		ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		OfflineShopTab m_aOfflineShoptabs[SHOP_TAB_COUNT_MAX];
#endif

//Search:

		typedef std::map<TItemPos, TShopItemTable> TPrivateShopItemStock;
		TPrivateShopItemStock	m_PrivateShopItemStock;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		typedef std::map<TItemPos, TShopItemTable2> TOfflineShopItemStock;
		TOfflineShopItemStock	m_OfflineShopItemStock;
#endif