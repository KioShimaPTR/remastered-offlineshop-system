// arat: Refine(LPCHARACTER ch, const char* c_pData);
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		int			OfflineShop(LPCHARACTER ch, const char * data, size_t uiBytes);
		int			MyOfflineShop(LPCHARACTER ch, const char * c_pData, size_t uiBytes);
		// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: MarriageRemove(TPacketMarriageRemove * p);
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	void		HandleCurrentOnline(TPacketOnlineSize * p);
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: IamAwake(LPDESC d, const char * c_pData);
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		void		SendOfflineShopMessage(LPDESC d, const char * c_pData);
		// END_OF_OFFLINE_SHOP