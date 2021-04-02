// arat: NAMECOLOR_WAYPOINT,
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			NAMECOLOR_OFFLINESHOP,
			// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: EFFECT_SELECT,
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			EFFECT_SELECT_OFFLINESHOP,
			// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: EFFECT_TARGET,
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			EFFECT_TARGET_OFFLINESHOP,
			// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: BOOL					IsPC();
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		BOOL					IsOfflineShopNPC();
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void __DetachSelectEffect();
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		void __AttachOfflineShopSelectEffect();
		void __DetachOfflineShopSelectEffect();
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void __DetachTargetEffect();
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		void __AttachOfflineShopTargetEffect();
		void __DetachOfflineShopTargetEffect();
		// END_OF_OFFLINE_SHOP