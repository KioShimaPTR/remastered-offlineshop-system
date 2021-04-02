// arat: #define INSTANT_FLAG_NO_REWARD			(1 << 4)
// altına ekle:
// BEGIN_OFFLINE_SHOP
#define INSTANT_FLAG_OFFLINE_SHOP	(1 << 5)
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: LPENTITY		m_pDragonSoulRefineWindowOpener;
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	DWORD			real_owner;
	DWORD			real_owner_aid;
	bool			isOfflineShop;
	int				leftTime;
	BYTE			bSaveTime;
	BYTE			bChannel;
	DWORD			dwVID;
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: typedef class CShop * LPSHOP;
// altına ekle:
// BEGIN_OFFLINE_SHOP
class COfflineShop;
typedef class COfflineShop * LPOFFLINESHOP;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: // End of shop
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	public:
		void			SetOfflineShop(LPOFFLINESHOP pkOfflineShop);
		LPOFFLINESHOP	GetOfflineShop() const { return m_pkOfflineShop; }
		void			SetOfflineShopOwner(LPCHARACTER ch) { m_pkChrOfflineShopOwner = ch; }
		LPCHARACTER		GetOfflineShopOwner() const { return m_pkChrOfflineShopOwner; }
		void			SetOfflineShopRealOwner(DWORD dwVID) { m_pointsInstant.real_owner = dwVID; }
		DWORD			GetOfflineShopRealOwner() const { return m_pointsInstant.real_owner; }
		void			SetOfflineShopRealOwnerAccountID(DWORD dwVID) { m_pointsInstant.real_owner_aid = dwVID; }
		DWORD			GetOfflineShopRealOwnerAccountID() const { return m_pointsInstant.real_owner_aid; }
		void			SetOfflineShopNPC(bool flag) { m_pointsInstant.isOfflineShop = flag; }
		bool			IsOfflineShopNPC() const { return m_pointsInstant.isOfflineShop; }
		void			OpenMyOfflineShop(const char * c_pszSign, TShopItemTable * pTable, BYTE bItemCount, BYTE bTime);
		void			SetOfflineShopTimer(int iTime) { m_pointsInstant.leftTime = iTime; }
		int				GetOfflineShopTimer() { return m_pointsInstant.leftTime; }
		void			SetOfflineShopSaveTime(BYTE bSaveTime) { m_pointsInstant.bSaveTime = bSaveTime; }
		BYTE			GetOfflineShopSaveTime() { return m_pointsInstant.bSaveTime; }
		void			StartOfflineShopUpdateEvent();
		void			StopOfflineShopUpdateEvent();
		void			SetOfflineShopVID(DWORD dwVID) { m_pointsInstant.dwVID = dwVID; }
		DWORD			GetOfflineShopVID() { return m_pointsInstant.dwVID; }

	protected:
		LPOFFLINESHOP	m_pkOfflineShop;
		LPCHARACTER     m_pkChrOfflineShopOwner;
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: LPEVENT				m_pkPetSystemUpdateEvent;
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		LPEVENT				m_pkOfflineShopUpdateEvent;
		// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: void	SetMyShopTime() { m_iMyShopTime = thecore_pulse(); }
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		int 	m_iMyOfflineShopTime;
		int		GetMyOfflineShopTime() const	{ return m_iMyOfflineShopTime; }
		void	SetMyOfflineShopTime() { m_iMyOfflineShopTime = thecore_pulse(); }
		// END_OF_OFFLINE_SHOP