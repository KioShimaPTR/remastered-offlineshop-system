//Search:

BOOL CPythonShop::GetItemData(DWORD dwIndex, const TShopItemData ** c_ppItemData)

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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
#endif

//Search:

void CPythonShop::Open(BOOL isPrivateShop, BOOL isMainPrivateShop, BOOL isOfflineShop)

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CPythonShop::ClearOfflineShopStock()
{
	m_OfflineShopItemStock.clear();
}
void CPythonShop::AddOfflineShopItemStock(TItemPos ItemPos, BYTE dwDisplayPos, DWORD dwPrice, WORD wPrice2, BYTE bPrice_type)
{
	DelOfflineShopItemStock(ItemPos);

	TShopItemTable2 SellingItem;
	SellingItem.vnum = 0;
	SellingItem.count = 0;
	SellingItem.pos = ItemPos;
	SellingItem.price = dwPrice;
	SellingItem.price2 = wPrice2;
	SellingItem.price_type = bPrice_type;
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

	TShopItemTable2 & rShopItemTable = itor->second;
	return rShopItemTable.price;
}
WORD CPythonShop::GetOfflineShopItemPrice2(TItemPos ItemPos)
{
	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.find(ItemPos);

	if (m_OfflineShopItemStock.end() == itor)
		return 0;

	TShopItemTable2 & rShopItemTable = itor->second;
	return rShopItemTable.price2;
}
BYTE CPythonShop::GetOfflineShopItemPriceType(TItemPos ItemPos)
{
	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.find(ItemPos);

	if (m_OfflineShopItemStock.end() == itor)
		return 0;

	TShopItemTable2 & rShopItemTable = itor->second;
	return rShopItemTable.price_type;
}
void CPythonShop::BuildOfflineShop(const char * c_szName, BYTE bTime)
{
	std::vector<TShopItemTable2> ItemStock;
	ItemStock.reserve(m_OfflineShopItemStock.size());

	TOfflineShopItemStock::iterator itor = m_OfflineShopItemStock.begin();
	for (; itor != m_OfflineShopItemStock.end(); ++itor)
	{
		ItemStock.push_back(itor->second);
	}

	std::sort(ItemStock.begin(), ItemStock.end(), ItemStockSortFunc2());

	CPythonNetworkStream::Instance().SendBuildOfflineShopPacket(c_szName, ItemStock, bTime);
}
#endif

//Search:

BOOL CPythonShop::IsMainPlayerPrivateShop()
{
	return m_isMainPlayerPrivateShop;
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
BOOL CPythonShop::IsOfflineShop()
{
	return m_isOfflineShop;
}
#endif

//Search:

	m_isMainPlayerPrivateShop = FALSE;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_isOfflineShop = FALSE;
#endif

//Search:

	ClearPrivateShopStock();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	ClearOfflineShopStock();
#endif

//Search:

	int isMainPrivateShop = false;
	PyTuple_GetInteger(poArgs, 1, &isMainPrivateShop);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	int isOfflineShop = false;
	PyTuple_GetInteger(poArgs, 2, &isOfflineShop);
#endif

//Search:

PyObject * shopGetTabCount(PyObject * poSelf, PyObject * poArgs)

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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

PyObject * shopGetOfflineShopItemVnum(PyObject * poSelf, PyObject * poArgs)//eklenti
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->vnum);

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

PyObject * shopGetOfflineShopItemPrice2(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->price2);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemPriceType(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->price_type);

	return Py_BuildValue("i", 0);
}

PyObject * shopGetOfflineShopItemEvolution(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const TShopOfflineItemData * c_pItemData;
	if (CPythonShop::Instance().GetOfflineShopItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->evolution);

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
	WORD wPrice2;
	if (!PyTuple_GetInteger(poArgs, 4, &wPrice2))
		return Py_BuildException();
	BYTE bPrice_type;
	if (!PyTuple_GetInteger(poArgs, 5, &bPrice_type))
		return Py_BuildException();

	CPythonShop::Instance().AddOfflineShopItemStock(TItemPos(bItemWindowType, wItemSlotIndex), iDisplaySlotIndex, iPrice, wPrice2, bPrice_type);
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
PyObject * shopGetOfflineShopItemPriceReal(PyObject * poSelf, PyObject * poArgs)
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

PyObject * shopGetOfflineShopItemPriceReal2(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	int iValue = CPythonShop::Instance().GetOfflineShopItemPrice2(TItemPos(bItemWindowType, wItemSlotIndex));
	return Py_BuildValue("i", iValue);
}
PyObject * shopGetOfflineShopItemPriceTypeReal(PyObject * poSelf, PyObject * poArgs)
{
	BYTE bItemWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bItemWindowType))
		return Py_BuildException();
	WORD wItemSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wItemSlotIndex))
		return Py_BuildException();

	int iValue = CPythonShop::Instance().GetOfflineShopItemPriceType(TItemPos(bItemWindowType, wItemSlotIndex));
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
#endif

//Search:

		{ "BuildPrivateShop",			shopBuildPrivateShop,			METH_VARARGS },

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		// Offline Shop
		{ "IsOfflineShop",					shopIsOfflineShop,					METH_VARARGS },
		{ "GetOfflineShopItemID",			shopGetOfflineShopItemID,			METH_VARARGS },
		{ "GetOfflineShopItemCount",		shopGetOfflineShopItemCount,		METH_VARARGS },
		{ "GetOfflineShopItemVnum",			shopGetOfflineShopItemVnum,			METH_VARARGS },
		{ "GetOfflineShopItemPrice",		shopGetOfflineShopItemPrice,		METH_VARARGS },
		{ "GetOfflineShopItemPrice2",		shopGetOfflineShopItemPrice2,		METH_VARARGS },
		{ "GetOfflineShopItemPriceType",	shopGetOfflineShopItemPriceType,	METH_VARARGS },
		{ "GetOfflineShopItemEvolution",	shopGetOfflineShopItemEvolution,	METH_VARARGS },
		{ "GetOfflineShopItemStatus",		shopGetOfflineShopItemStatus,		METH_VARARGS },
		{ "GetOfflineShopItemMetinSocket",	shopGetOfflineShopItemMetinSocket,	METH_VARARGS },
		{ "GetOfflineShopItemAttribute",	shopGetOfflineShopItemAttribute,	METH_VARARGS },

		{ "ClearOfflineShopStock",			shopClearOfflineShopStock,			METH_VARARGS },
		{ "AddOfflineShopItemStock",		shopAddOfflineShopItemStock,		METH_VARARGS },
		{ "DelOfflineShopItemStock",		shopDelOfflineShopItemStock,		METH_VARARGS },
		{ "GetOfflineShopItemPriceReal",	shopGetOfflineShopItemPriceReal,	METH_VARARGS },
		{ "GetOfflineShopItemPrice2Real",	shopGetOfflineShopItemPriceReal2,	METH_VARARGS },
		{ "GetOfflineShopItemPriceTypeReal",shopGetOfflineShopItemPriceTypeReal,METH_VARARGS },
		{ "BuildOfflineShop",				shopBuildOfflineShop,				METH_VARARGS },
#endif

//Search:

	PyModule_AddIntConstant(poModule, "SHOP_COIN_TYPE_SECONDARY_COIN", SHOP_COIN_TYPE_SECONDARY_COIN);

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	PyModule_AddIntConstant(poModule, "OFFLINE_SHOP_SLOT_COUNT", OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
#endif