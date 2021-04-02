//Search:

			BOOL	bActive;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			BOOL	bActiveOfflineShop;
#endif

//Search:

		void RefreshSlot();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void ActivateOfflineShopSlot(DWORD dwIndex);
		void DeactivateOfflineShopSlot(DWORD dwIndex);
#endif

//Search:

		std::deque<DWORD> m_ReserveDestroyEffectDeque;

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		CAniImageBox* m_apOfflineShopSlotActiveEffect[3];
#endif