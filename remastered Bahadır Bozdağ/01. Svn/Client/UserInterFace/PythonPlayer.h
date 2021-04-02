//Search:

		void	SetPlayTime(DWORD dwPlayTime);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void		SetCurrentOfflineShopMoney(long long llMoney);
		long long	GetCurrentOfflineShopMoney();

		void		SetCurrentOfflineShopBar1(DWORD dwBar1);
		DWORD		GetCurrentOfflineShopBar1();

		void		SetCurrentOfflineShopBar2(DWORD dwBar2);
		DWORD		GetCurrentOfflineShopBar2();

		void		SetCurrentOfflineShopBar3(DWORD dwBar3);
		DWORD		GetCurrentOfflineShopBar3();
#endif

//Search:

		DWORD					m_dwPlayTime;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		long long				m_llCurrentOfflineShopMoney;
		DWORD					m_dwCurrentOfflineShopBar1;
		DWORD					m_dwCurrentOfflineShopBar2;
		DWORD					m_dwCurrentOfflineShopBar3;
#endif