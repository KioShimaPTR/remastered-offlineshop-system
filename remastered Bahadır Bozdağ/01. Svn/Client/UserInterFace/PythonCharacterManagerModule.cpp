//Search:

	PyModule_AddIntConstant(poModule, "NAMECOLOR_WAYPOINT", CInstanceBase::NAMECOLOR_WAYPOINT);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	PyModule_AddIntConstant(poModule, "NAMECOLOR_OFFLINESHOP", CInstanceBase::NAMECOLOR_OFFLINESHOP);
#endif

//Search:

	PyModule_AddIntConstant(poModule, "EFFECT_SELECT",				CInstanceBase::EFFECT_SELECT);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	PyModule_AddIntConstant(poModule, "EFFECT_SELECT_OFFLINESHOP",	CInstanceBase::EFFECT_SELECT_OFFLINESHOP);
#endif

//Search:

	PyModule_AddIntConstant(poModule, "EFFECT_TARGET",				CInstanceBase::EFFECT_TARGET);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	PyModule_AddIntConstant(poModule, "EFFECT_TARGET_OFFLINESHOP",	CInstanceBase::EFFECT_TARGET_OFFLINESHOP);
#endif