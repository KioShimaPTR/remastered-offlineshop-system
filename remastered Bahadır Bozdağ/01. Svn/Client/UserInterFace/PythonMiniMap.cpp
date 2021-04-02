//Search:

	m_WarpPositionVector.clear();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_OffShopPositionVector.clear();
#endif

//Search:

	// NPC
	STATEMANAGER.SetRenderState(D3DRS_TEXTUREFACTOR, CInstanceBase::GetIndexedNameColor(CInstanceBase::NAMECOLOR_NPC));
	aIterator = m_NPCPositionVector.begin();
	while (aIterator != m_NPCPositionVector.end())
	{
		TMarkPosition & rPosition = *aIterator;
		m_WhiteMark.SetPosition(rPosition.m_fX, rPosition.m_fY);
		m_WhiteMark.Render();
		++aIterator;
	}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	// Offline Shop
	STATEMANAGER.SetRenderState(D3DRS_TEXTUREFACTOR, CInstanceBase::GetIndexedNameColor(CInstanceBase::NAMECOLOR_OFFLINESHOP));
	aIterator = m_OffShopPositionVector.begin();
	while (aIterator != m_OffShopPositionVector.end())
	{
		TMarkPosition & rPosition = *aIterator;
		m_WhiteMark.SetPosition(rPosition.m_fX, rPosition.m_fY);
		m_WhiteMark.Render();
		++aIterator;
	}
#endif