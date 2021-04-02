//Search:

PyObject * chrIsNPC(PyObject* poSelf, PyObject* poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BuildException();

	CInstanceBase * pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (!pInstance)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pInstance->IsNPC());
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
PyObject * chrIsOfflineShop(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pkInst->IsOfflineShopNPC());
}

PyObject * chrIsOfflineShopPC(PyObject* poSelf, PyObject* poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BuildException();

	CInstanceBase * pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);
	if (!pInstance)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pInstance->IsOfflineShop());
}
#endif

//Search:

		{ "IsGameMaster",				chrIsGameMaster,					METH_VARARGS },

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		{ "IsOfflineShop",				chrIsOfflineShop,					METH_VARARGS },
		{ "IsOfflineShopPC",			chrIsOfflineShopPC,					METH_VARARGS },
#endif