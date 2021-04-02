// arat: void CSlotWindow::DeactivateSlot(DWORD dwIndex)
// altına ekle:
// BEGIN_OFFLINE_SHOP
void CSlotWindow::ActivateOfflineShopSlot(DWORD dwIndex)
{
	TSlot * pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bActiveOfflineShop = TRUE;
}

void CSlotWindow::DeactivateOfflineShopSlot(DWORD dwIndex)
{
	TSlot * pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bActiveOfflineShop = FALSE;
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: pSlot->bActive = FALSE;
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	pSlot->bActiveOfflineShop = FALSE;
	// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: if (m_pSlotActiveEffect)
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	for (int i = 0; i < 3; ++i)
	if (m_apOfflineShopSlotActiveEffect[i])
		m_apOfflineShopSlotActiveEffect[i]->Update();
	// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: if (rSlot.bActive)
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		if (rSlot.bActiveOfflineShop)
		{
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + rSlot.ixPosition,
				m_rect.top + rSlot.iyPosition,
				m_rect.left + rSlot.ixPosition + rSlot.byxPlacedItemSize * ITEM_WIDTH,
				m_rect.top + rSlot.iyPosition + rSlot.byyPlacedItemSize * ITEM_HEIGHT);
		}
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: m_pBaseImageInstance = NULL;
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	for (int i = 0; i < 3; ++i)
		m_apOfflineShopSlotActiveEffect[i] = NULL;
	// END_OF_OFFLINE_SHOP