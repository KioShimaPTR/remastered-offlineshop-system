//Search:

		void		Refine(LPCHARACTER ch, const char* c_pData);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		int			OfflineShop(LPCHARACTER ch, const char * data, size_t uiBytes);
		int			MyOfflineShop(LPCHARACTER ch, const char * c_pData, size_t uiBytes);
#endif

//Search:

		void		IamAwake(LPDESC d, const char * c_pData);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void		SendOfflineShopMessage(LPDESC d, const char * c_pData);
#endif