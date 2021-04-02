// arat: PyObject * chrIsNPC(PyObject* poSelf, PyObject* poArgs)
// altına ekle:
// BEGIN_OFFLINE_SHOP
PyObject * chrIsOfflineShop(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pkInst = CPythonCharacterManager::Instance().GetSelectedInstancePtr();

	if (!pkInst)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pkInst->IsOfflineShopNPC());
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: { "IsNPC",						chrIsNPC,							METH_VARARGS },
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		{ "IsOfflineShop",				chrIsOfflineShop,					METH_VARARGS },
		// END_OF_OFFLINE_SHOP