//Search:

PyObject* netSendExchangeStartPacket(PyObject* poSelf, PyObject* poArgs)

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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

PyObject * netSendDestroyOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendDestroyOfflineShop();
	return Py_BuildNone();
}

PyObject * netSendRefreshOfflineShopPrice(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendRefreshOfflineShopPrice();
	return Py_BuildNone();
}

PyObject * netSendUnsoldItems(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendUnsoldItems();
	return Py_BuildNone();
}

PyObject * netSendOfflineShopFetchItem(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bDisplayPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bDisplayPos))
		return Py_BuildException();

	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendOfflineShopFetchItem(bDisplayPos);
	return Py_BuildNone();
}

PyObject * netSendOfflineShopWithdrawMoney(PyObject * poSelf, PyObject * poArgs)
{
	int iMoney;
	if (!PyTuple_GetInteger(poArgs, 0, &iMoney))
		return Py_BuildException();

	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendOfflineShopWithdrawMoney(iMoney);
	return Py_BuildNone();
}

PyObject * netSendOfflineShopWithdrawBar1(PyObject * poSelf, PyObject * poArgs)
{
	int iBar1;
	if (!PyTuple_GetInteger(poArgs, 0, &iBar1))
		return Py_BuildException();

	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendOfflineShopWithdrawBar1(iBar1);
	return Py_BuildNone();
}

PyObject * netSendOfflineShopWithdrawBar2(PyObject * poSelf, PyObject * poArgs)
{
	int iBar2;
	if (!PyTuple_GetInteger(poArgs, 0, &iBar2))
		return Py_BuildException();

	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendOfflineShopWithdrawBar2(iBar2);
	return Py_BuildNone();
}

PyObject * netSendOfflineShopWithdrawBar3(PyObject * poSelf, PyObject * poArgs)
{
	int iBar3;
	if (!PyTuple_GetInteger(poArgs, 0, &iBar3))
		return Py_BuildException();

	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendOfflineShopWithdrawBar3(iBar3);
	return Py_BuildNone();
}

PyObject * netSendOfflineShopTeleport(PyObject * poSelf, PyObject * poArgs)
{
	CPythonNetworkStream & rkNetStream = CPythonNetworkStream::instance();
	rkNetStream.SendOfflineShopTeleport();
	return Py_BuildNone();
}
#endif

//Search:

		{ "SendShopSellPacketNew",				netSendShopSellPacketNew,				METH_VARARGS },

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		{ "SendOfflineShopEndPacket",			netSendOfflineShopEndPacket,			METH_VARARGS },
		{ "SendOfflineShopBuyPacket",			netSendOfflineShopBuyPacket,			METH_VARARGS },
		{ "SendDestroyOfflineShop",				netSendDestroyOfflineShop,				METH_VARARGS },
		{ "SendRefreshOfflineShopPrice",		netSendRefreshOfflineShopPrice,			METH_VARARGS },
		{ "SendUnsoldItems",					netSendUnsoldItems,						METH_VARARGS },
		{ "SendOfflineShopWithdrawMoney",		netSendOfflineShopWithdrawMoney,		METH_VARARGS },
		{ "SendOfflineShopWithdrawBar1",		netSendOfflineShopWithdrawBar1,			METH_VARARGS },
		{ "SendOfflineShopWithdrawBar2",		netSendOfflineShopWithdrawBar2,			METH_VARARGS },
		{ "SendOfflineShopWithdrawBar3",		netSendOfflineShopWithdrawBar3,			METH_VARARGS },
		{ "SendOfflineShopFetchItem",			netSendOfflineShopFetchItem,			METH_VARARGS },
		{ "SendOfflineShopTeleportPacket",		netSendOfflineShopTeleport,				METH_VARARGS },
#endif