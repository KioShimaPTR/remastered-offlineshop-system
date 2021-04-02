// arat: PyObject* netSendShopSellPacketNew(PyObject* poSelf, PyObject* poArgs)
// altına ekle:
// BEGIN_OFFLINE_SHOP
PyObject* netSendOfflineShopEndPacket(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendOfflineShopEndPacket();
	return Py_BuildNone();
}

PyObject* netSendOfflineShopBuyPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iCount;
	if (!PyTuple_GetInteger(poArgs, 0, &iCount))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendOfflineShopBuyPacket(iCount);
	return Py_BuildNone();
}

PyObject* netSendAddOfflineShopItemPacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos item_pos;
	int iPrice;

	if (!PyTuple_GetInteger(poArgs, 0, &item_pos.cell))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &iPrice))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendAddOfflineShopItemPacket(item_pos, iPrice);
	return Py_BuildNone();
}

PyObject * netSendDestroyOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendDestroyOfflineShop();
	return Py_BuildNone();
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: { "SendShopSellPacketNew",				netSendShopSellPacketNew,				METH_VARARGS },
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		{ "SendOfflineShopEndPacket",			netSendOfflineShopEndPacket,			METH_VARARGS },
		{ "SendOfflineShopBuyPacket",			netSendOfflineShopBuyPacket,			METH_VARARGS },
		{ "SendAddOfflineShopItemPacket",		netSendAddOfflineShopItemPacket,		METH_VARARGS },
		{ "SendDestroyOfflineShop",				netSendDestroyOfflineShop,				METH_VARARGS },
		// END_OF_OFFLINE_SHOP