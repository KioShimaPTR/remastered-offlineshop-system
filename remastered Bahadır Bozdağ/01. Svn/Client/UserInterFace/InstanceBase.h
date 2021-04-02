//Search:

			NAMECOLOR_WAYPOINT,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			NAMECOLOR_OFFLINESHOP,
#endif

//Search:

			EFFECT_SELECT,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			EFFECT_SELECT_OFFLINESHOP,
#endif

//Search:

			EFFECT_TARGET,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			EFFECT_TARGET_OFFLINESHOP,
#endif

//Search:

		BOOL					IsInvisibility();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		BOOL					IsOfflineShop();
#endif

//Search:

		BOOL					IsPC();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		BOOL					IsOfflineShopNPC();
#endif

//Search:

		void __AttachSelectEffect();
		void __DetachSelectEffect();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void __AttachOfflineShopSelectEffect();
		void __DetachOfflineShopSelectEffect();
#endif

//Search:

		void __AttachTargetEffect();
		void __DetachTargetEffect();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		void __AttachOfflineShopTargetEffect();
		void __DetachOfflineShopTargetEffect();
#endif