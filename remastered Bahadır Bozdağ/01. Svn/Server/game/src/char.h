//Search:

#define INSTANT_FLAG_NO_REWARD			(1 << 4)

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#define INSTANT_FLAG_OFFLINE_SHOP	(1 << 5)
#endif

//Search:

	BYTE			bBlockMode;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	DWORD			real_owner;
	DWORD			real_owner_aid;
	bool			isOfflineShop;
	int				leftTime;
	BYTE			bSaveTime;
	BYTE			bChannel;
	DWORD			dwVID;
#endif

//Search:

class CShop;
typedef class CShop * LPSHOP;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
class COfflineShop;
typedef class COfflineShop * LPOFFLINESHOP;
#endif

//Search:

		LPSHOP			m_pkShop;
		LPSHOP			m_pkMyShop;
		std::string		m_stShopSign;
		LPCHARACTER		m_pkChrShopOwner;
		// End of shop

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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

		void			OpenMyOfflineShop(const char * c_pszSign, TShopItemTable2 * pTable, BYTE bItemCount, BYTE bTime);

		void			SetOfflineShopTimer(int iTime) { m_pointsInstant.leftTime = iTime; }
		int				GetOfflineShopTimer() { return m_pointsInstant.leftTime; }
		void			SetOfflineShopSaveTime(BYTE bSaveTime) { m_pointsInstant.bSaveTime = bSaveTime; }
		BYTE			GetOfflineShopSaveTime() { return m_pointsInstant.bSaveTime; }

		void			DestroyOfflineShop();

		void			StartOfflineShopUpdateEvent();
		void			StopOfflineShopUpdateEvent();

		void			SetOfflineShopVID(DWORD dwVID) { m_pointsInstant.dwVID = dwVID; }
		DWORD			GetOfflineShopVID() { return m_pointsInstant.dwVID; }

	protected:
		LPOFFLINESHOP	m_pkOfflineShop;
		LPCHARACTER     m_pkChrOfflineShopOwner;
#endif

//Search:

		LPEVENT				m_pkPetSystemUpdateEvent;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		LPEVENT				m_pkOfflineShopUpdateEvent;
#endif

//Search:

		int 	m_iMyShopTime;
		int		GetMyShopTime() const	{ return m_iMyShopTime; }
		void	SetMyShopTime() { m_iMyShopTime = thecore_pulse(); }

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		int 	m_iMyOfflineShopTime;
		int		GetMyOfflineShopTime() const	{ return m_iMyOfflineShopTime; }
		void	SetMyOfflineShopTime() { m_iMyOfflineShopTime = thecore_pulse(); }
#endif