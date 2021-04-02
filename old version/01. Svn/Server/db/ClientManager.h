// arat: SendGuildSkillUsable(DWORD guild_id, DWORD dwSkillVnum, bool bUsable);
// üstüne ekle:
	// BEGIN_OFFLINE_SHOP
	void 			HandleCurrentOnline(TPacketOnlineSize * p);
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: void RequestChannelStatus(CPeer* peer, DWORD dwHandle);
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	public:
		void				EraseItemCache(DWORD id);
		const TItemTable*	GetItemTable(DWORD dwVnum) const;
		void QUERY_ADD_OFFLINESHOP_ITEM(TPlayerOfflineShopAddItem* d, CPeer* peer);
		void QUERY_DEL_OFFLINESHOP_ITEM(TPlayerOfflineShopAddItem* d, CPeer* peer);
		void QUERY_UPDATE_OFFLINE_SHOP_COUNT(TPacketUpdateOfflineShopsCount* p);
	// END_OF_OFFLINE_SHOP