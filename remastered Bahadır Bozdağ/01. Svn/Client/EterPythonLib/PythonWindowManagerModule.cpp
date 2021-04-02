//Search:

PyObject* wndMgrEnableSlot(PyObject* poSelf, PyObject* poArgs)

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
PyObject* wndMgrActivateOfflineShopSlot(PyObject* poSelf, PyObject* poArgs)
{
	UI::CWindow* pWin;
	if (!PyTuple_GetWindow(poArgs, 0, &pWin))
		return Py_BuildException();

	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSlotIndex))
		return Py_BuildException();

	if (!pWin->IsType(UI::CSlotWindow::Type()))
		return Py_BuildException();

	UI::CSlotWindow* pSlotWin = (UI::CSlotWindow*)pWin;
	pSlotWin->ActivateOfflineShopSlot(iSlotIndex);
	return Py_BuildNone();
}

PyObject* wndMgrDeactivateOfflineShopSlot(PyObject* poSelf, PyObject* poArgs)
{
	UI::CWindow* pWin;
	if (!PyTuple_GetWindow(poArgs, 0, &pWin))
		return Py_BuildException();

	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSlotIndex))
		return Py_BuildException();

	if (!pWin->IsType(UI::CSlotWindow::Type()))
		return Py_BuildException();

	UI::CSlotWindow* pSlotWin = (UI::CSlotWindow*)pWin;
	pSlotWin->DeactivateOfflineShopSlot(iSlotIndex);
	return Py_BuildNone();
}
#endif

//Search:

		{ "ShowOverInWindowName",		wndMgrShowOverInWindowName,			METH_VARARGS },

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		{ "ActivateOfflineShopSlot",	wndMgrActivateOfflineShopSlot,		METH_VARARGS },
		{ "DeactivateOfflineShopSlot",	wndMgrDeactivateOfflineShopSlot,	METH_VARARGS },
#endif