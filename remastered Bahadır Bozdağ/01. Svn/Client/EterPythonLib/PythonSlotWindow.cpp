//Search:

void CSlotWindow::DeactivateSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bActive = FALSE;
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CSlotWindow::ActivateOfflineShopSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bActiveOfflineShop = TRUE;
}

void CSlotWindow::DeactivateOfflineShopSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bActiveOfflineShop = FALSE;
}
#endif

//Search:

void CSlotWindow::ClearSlot(TSlot* pSlot)
{
	pSlot->bActive = FALSE;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	pSlot->bActiveOfflineShop = FALSE;
#endif

//Search:

	if (m_pSlotActiveEffect)
		m_pSlotActiveEffect->Update();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	for (int i = 0; i < 3; ++i)
	{
		if (m_apOfflineShopSlotActiveEffect[i])
			m_apOfflineShopSlotActiveEffect[i]->Update();
	}
#endif

//Search:

			if (m_pSlotActiveEffect)
			{
				m_pSlotActiveEffect->SetPosition(ix, iy);
				m_pSlotActiveEffect->Render();
			}
		}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		if (rSlot.bActiveOfflineShop)
		{
			CPythonGraphic::Instance().SetDiffuseColor(1.0f, 0.0f, 0.0f, 0.3f);
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + rSlot.ixPosition,
				m_rect.top + rSlot.iyPosition,
				m_rect.left + rSlot.ixPosition + rSlot.byxPlacedItemSize * ITEM_WIDTH,
				m_rect.top + rSlot.iyPosition + rSlot.byyPlacedItemSize * ITEM_HEIGHT);
		}
#endif

//Search:

	m_pSlotActiveEffect = NULL;
	m_pBaseImageInstance = NULL;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	for (int i = 0; i < 3; ++i)
		m_apOfflineShopSlotActiveEffect[i] = NULL;
#endif