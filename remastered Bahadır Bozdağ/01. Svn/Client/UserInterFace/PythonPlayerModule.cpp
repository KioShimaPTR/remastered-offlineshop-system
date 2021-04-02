//Search:

PyObject * playerSetPlayTime(PyObject* poSelf, PyObject* poArgs)
{
	int iTime;
	if (!PyTuple_GetInteger(poArgs, 0, &iTime))
		return Py_BuildException();

	CPythonPlayer::Instance().SetPlayTime(iTime);
	return Py_BuildNone();
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
PyObject * playerGetCurrentOfflineShopMoney(PyObject * poSelf, PyObject * poArgs)
{
	return PyLong_FromLongLong(CPythonPlayer::instance().GetCurrentOfflineShopMoney());
}

PyObject * playerGetCurrentOfflineShopBar1(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::instance().GetCurrentOfflineShopBar1());
}

PyObject * playerGetCurrentOfflineShopBar2(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::instance().GetCurrentOfflineShopBar2());
}

PyObject * playerGetCurrentOfflineShopBar3(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::instance().GetCurrentOfflineShopBar3());
}
#endif

//Search:

		{ "SetPlayTime",				playerSetPlayTime,					METH_VARARGS },

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		{ "GetCurrentOfflineShopMoney",	playerGetCurrentOfflineShopMoney,	METH_VARARGS },
		{ "GetCurrentOfflineShopBar1",	playerGetCurrentOfflineShopBar1,	METH_VARARGS },
		{ "GetCurrentOfflineShopBar2",	playerGetCurrentOfflineShopBar2,	METH_VARARGS },
		{ "GetCurrentOfflineShopBar3",	playerGetCurrentOfflineShopBar3,	METH_VARARGS },
#endif

//Search:

	PyModule_AddIntConstant(poModule, "SLOT_TYPE_DRAGON_SOUL_INVENTORY",	SLOT_TYPE_DRAGON_SOUL_INVENTORY);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_OFFLINE_SHOP",				SLOT_TYPE_OFFLINE_SHOP);
#endif