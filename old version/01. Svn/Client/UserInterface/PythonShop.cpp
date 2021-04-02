// arat: void CPythonShop::SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData & c_rShopItemData)
// altına ekle:
// BEGIN_OFFLINE_SHOP
void CPythonShop::SetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData & c_rShopItemData)
{
	BYTE tabIdx = dwIndex / OFFLINE_SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % OFFLINE_SHOP_HOST_ITEM_MAX_NUM;

	SetOfflineShopItemData(tabIdx, dwSlotPos, c_rShopItemData);
}

void CPythonShop::SetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData & c_rShopItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
		return;
	}

	m_aOfflineShoptabs[tabIdx].items[dwSlotPos] = c_rShopItemData;
}

BOOL CPythonShop::GetOfflineShopItemData(DWORD dwIndex, const TShopOfflineItemData ** c_ppItemData)
{
	BYTE tabIdx = dwIndex / OFFLINE_SHOP_HOST_ITEM_MAX_NUM;
	DWORD dwSlotPos = dwIndex % OFFLINE_SHOP_HOST_ITEM_MAX_NUM;

	return GetOfflineShopItemData(tabIdx, dwSlotPos, c_ppItemData);
}

BOOL CPythonShop::GetOfflineShopItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopOfflineItemData ** c_ppItemData)
{
	if (tabIdx >= SHOP_TAB_COUNT_MAX || dwSlotPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		TraceError("Out of Index. tabIdx(%d) must be less than %d. dwSlotPos(%d) must be less than %d", tabIdx, SHOP_TAB_COUNT_MAX, dwSlotPos, OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
		return FALSE;
	}

	*c_ppItemData = &m_aOfflineShoptabs[tabIdx].items[dwSlotPos];

	return TRUE;
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void CPythonShop::BuildPrivateShop(const char * c_szName)
// altına ekle:
// BEGIN_OFFLINE_SHOP
void CPythonShop::ClearOfflineShopStock()
{
	m_OfflineShopItemStock.clear();
}

void CPythonShop::AddOfflineShopItemStock(TItemPos ItemPos, BYTE dwDisplayPos, DWORD dwPrice)
{
	DelOfflineShopItemStock(ItemPos);

	TShopItemTable SellingItem;
	SellingItem.vnum = 0;
	SellingItem.count = 0;
	SellingItem.pos = ItemPos;
	SellingItem.price = dwPrice;
	SellingItem.display_pos = dwDisplayPos;
	m_OfflineShopItemStock.insert(make_pair(ItemPos, SellingItem));
}

void CPythonShop::DelOfflineShopItemStock(TItemPos ItemPos)
{
	if (m_OfflineShopItemStock.end() == m_OfflineShopItemStock.find(ItemPos))
		return;

	m_OfflineShopItemStock.erase(ItemPos);
}

int CPythonShop::GetOfflineShopItemPrice(TItemPos ItemPos)
{
	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.find(ItemPos);

	if (m_OfflineShopItemStock.end() == itor)
		return 0;

	TShopItemTable & rShopItemTable = itor->second;
	return rShopItemTable.price;
}

void CPythonShop::BuildOfflineShop(const char * c_szName, BYTE bTime)
{
	std::vector<TShopItemTable> ItemStock;
	ItemStock.reserve(m_OfflineShopItemStock.size());

	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.begin();
	for (; itor != m_OfflineShopItemStock.end(); ++itor)
	{
		ItemStock.push_back(itor->second);
	}

	std::sort(ItemStock.begin(), ItemStock.end(), ItemStockSortFunc());

	CPythonNetworkStream::Instance().SendBuildOfflineShopPacket(c_szName, ItemStock, bTime);
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void CPythonShop::Open
// değiştir:
// BEGIN_OFFLINE_SHOP
void CPythonShop::Open(BOOL isPrivateShop, BOOL isMainPrivateShop, BOOL isOfflineShop)
{
	m_isShoping = TRUE;
	m_isPrivateShop = isPrivateShop;
	m_isMainPlayerPrivateShop = isMainPrivateShop;
	m_isOfflineShop = isOfflineShop;
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: BOOL CPythonShop::IsMainPlayerPrivateShop()
// altına ekle:
// BEGIN_OFFLINE_SHOP
BOOL CPythonShop::IsOfflineShop()
{
	return m_isOfflineShop;
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: m_isMainPlayerPrivateShop = FALSE;
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	m_isOfflineShop = FALSE;
	ClearOfflineShopStock();
	// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: PyObject * shopOpen(PyObject * poSelf, PyObject * poArgs)
// değiştir:
// BEGIN_OFFLINE_SHOP
PyObject * shopOpen(PyObject * poSelf, PyObject * poArgs)
{
	int isPrivateShop = false;
	PyTuple_GetInteger(poArgs, 0, &isPrivateShop);

	int isMainPrivateShop = false;
	PyTuple_GetInteger(poArgs, 1, &isMainPrivateShop);

	int isOfflineShop = false;
	PyTuple_GetInteger(poArgs, 2, &isOfflineShop);

	CPythonShop& rkShop=CPythonShop::Instance();
	rkShop.Open(isPrivateShop, isMainPrivateShop, isOfflineShop);
	return Py_BuildNone();
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: PyObject * shopBuildPrivateShop(PyObject * poSelf, PyObject * poArgs)
// altına ekle:
// BEGIN_OFFLINE_SHOP
PyObject * shopIsOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop & rkShop = CPythonShop::Instance();
	return Py_BuildValue("i", rkShop.IsOfflineShop());
}

PyObject * shopGetOfflineShopItemID(PyObject * poSelf, PyObject * poArgs)
{
	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(nPos, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->vnum);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemStatus(PyObject * poSelf, PyObject * poArgs)
{
	int nPos;
	if (!PyTuple_GetInteger(poArgs, 0, &nPos))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(nPos, &c_pItemData))
		return Py_BuildValue("is", c_pItemData->bIsSold, c_pItemData->buyerName);

	return Py_BuildValue("is", 0);
}

PyObject * shopGetOfflineShopItemCount(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->count);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->price);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemMetinSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iMetinSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->alSockets[iMetinSocketIndex]);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemAttribute(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	int iAttrSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Py_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		const TShopOfflineItemData * c_pItemData;
		if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
			return Py_BuildValue("ii", c_pItemData->aAttr[iAttrSlotIndex].bType, c_pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Py_BuildValue("ii", 0, 0);
}

PyObject * shopClearOfflineShopStock(PyObject * poSelf, PyObject * poArgs)
{
	CPythonShop::Instance().ClearOfflineShopStock();
	return Py_BuildNone();
}
PyObject * shopAddOfflineShopItemStock(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();
	int iDisplaySlotIndex;
	if (!PyTuple_GetInteger(poArgs, 2, &iDisplaySlotIndex))
		return Py_BuildException();
	int iPrice;
	if (!PyTuple_GetInteger(poArgs, 3, &iPrice))
		return Py_BuildException();

	CPythonShop::Instance().AddOfflineShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex), iDisplaySlotIndex, iPrice);
	return Py_BuildNone();
}
PyObject * shopDelOfflineShopItemStock(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	CPythonShop::Instance().DelOfflineShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex));
	return Py_BuildNone();
}
PyObject * shopGetOfflineShopItemPrice2(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	int iValue = CPythonShop::Instance().GetOfflineShopItemPrice(TItemPos(bItemWindowType, wItemSlotIndex));
	return Py_BuildValue("i", iValue);
}
PyObject * shopBuildOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	char * szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	BYTE bTime;
	if (!PyTuple_GetInteger(poArgs, 1, &bTime))
		return Py_BuildException();

	CPythonShop::Instance().BuildOfflineShop(szName, bTime);
	return Py_BuildNone();
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: { "BuildPrivateShop",			shopBuildPrivateShop,			METH_VARARGS },
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		{ "IsOfflineShop",					shopIsOfflineShop,					METH_VARARGS },
		{ "GetOfflineShopItemID",			shopGetOfflineShopItemID,			METH_VARARGS },
		{ "GetOfflineShopItemCount",		shopGetOfflineShopItemCount,		METH_VARARGS },
		{ "GetOfflineShopItemPrice",		shopGetOfflineShopItemPrice,		METH_VARARGS },
		{ "GetOfflineShopItemStatus",		shopGetOfflineShopItemStatus,		METH_VARARGS },
		{ "GetOfflineShopItemMetinSocket",	shopGetOfflineShopItemMetinSocket,	METH_VARARGS },
		{ "GetOfflineShopItemAttribute",	shopGetOfflineShopItemAttribute,	METH_VARARGS },

		{ "ClearOfflineShopStock",		shopClearOfflineShopStock,		METH_VARARGS },
		{ "AddOfflineShopItemStock",	shopAddOfflineShopItemStock,	METH_VARARGS },
		{ "DelOfflineShopItemStock",	shopDelOfflineShopItemStock,	METH_VARARGS },
		{ "GetOfflineShopItemPrice2",	shopGetOfflineShopItemPrice2,	METH_VARARGS },
		{ "BuildOfflineShop",			shopBuildOfflineShop,			METH_VARARGS },
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: PyModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_SECONDARY_COIN", SHOP_COIN_TYPE_SECONDARY_COIN);
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	PyModule_AddIntConstant(poModule, "OFFLINE_SHOP_SLOT_COUNT", OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
	// END_OF_OFFLINE_SHOP