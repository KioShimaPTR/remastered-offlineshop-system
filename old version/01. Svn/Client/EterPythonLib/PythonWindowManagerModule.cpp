// arat: PyObject * wndMgrShowOverInWindowName(PyObject * poSelf, PyObject * poArgs)
// altına ekle:
// BEGIN_OFFLINE_SHOP
PyObject * wndMgrActivateOfflineShopSlot(PyObject * poSelf, PyObject * poArgs)
{
	UI::CWindow * pWin;
	if (!PyTuple_GetWindow(poArgs, 0, &pWin))
		return Py_BuildException();

	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSlotIndex))
		return Py_BuildException();

	if (!pWin->IsType(UI::CSlotWindow::Type()))
		return Py_BuildException();

	UI::CSlotWindow * pSlotWin = (UI::CSlotWindow *)pWin;
	pSlotWin->ActivateOfflineShopSlot(iSlotIndex);
	return Py_BuildNone();
}

PyObject * wndMgrDeactivateOfflineShopSlot(PyObject * poSelf, PyObject * poArgs)
{
	UI::CWindow * pWin;
	if (!PyTuple_GetWindow(poArgs, 0, &pWin))
		return Py_BuildException();

	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSlotIndex))
		return Py_BuildException();

	if (!pWin->IsType(UI::CSlotWindow::Type()))
		return Py_BuildException();

	UI::CSlotWindow * pSlotWin = (UI::CSlotWindow *)pWin;
	pSlotWin->DeactivateOfflineShopSlot(iSlotIndex);
	return Py_BuildNone();
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: static PyMethodDef s_methods[] =
// içerisine ekle:
		// BEGIN_OFFLINE_SHOP
		{ "ActivateOfflineShopSlot",	wndMgrActivateOfflineShopSlot,		METH_VARARGS },
		{ "DeactivateOfflineShopSlot",	wndMgrDeactivateOfflineShopSlot,	METH_VARARGS },
		// END_OF_OFFLINE_SHOP