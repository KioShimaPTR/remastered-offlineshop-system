//Search:

	PyModule_AddIntConstant(poModule, "CAMERA_STOP",			CPythonApplication::CAMERA_STOP);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_OFFLINE_SHOP_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_OFFLINE_SHOP_SYSTEM", 0);
#endif