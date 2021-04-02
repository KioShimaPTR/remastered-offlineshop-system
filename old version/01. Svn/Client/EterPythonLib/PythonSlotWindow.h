// arat: BOOL	bActive;
// altına ekle:
				// BEGIN_OFFLINE_SHOP
				BOOL	bActiveOfflineShop;
				// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void RefreshSlot();
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			void ActivateOfflineShopSlot(DWORD dwIndex);
			void DeactivateOfflineShopSlot(DWORD dwIndex);
			// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: CAniImageBox * m_pSlotActiveEffect;
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			CAniImageBox* m_apOfflineShopSlotActiveEffect[3];
			// END_OF_OFFLINE_SHOP