// arat: m_WarpPositionVector.clear();
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	m_OffShopPositionVector.clear();
	// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: else if (pkInstEach->IsNPC())
// değiştir:
		// BEGIN_OFFLINE_SHOP
		else if (pkInstEach->IsNPC())
		{
			aMarkPosition.m_fX = ( m_fWidth - (float)m_WhiteMark.GetWidth() ) / 2.0f + fDistanceFromCenterX + m_fScreenX;
			aMarkPosition.m_fY = ( m_fHeight - (float)m_WhiteMark.GetHeight() ) / 2.0f + fDistanceFromCenterY + m_fScreenY;

			if (pkInstEach->GetRace() == 30000)
				m_OffShopPositionVector.push_back(aMarkPosition);
			else
				m_NPCPositionVector.push_back(aMarkPosition);
		}
		// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: aIterator = m_NPCPositionVector.begin();
// kod bloğundan sonra ekle:
	// BEGIN_OFFLINE_SHOP
	STATEMANAGER.SetRenderState(D3DRS_TEXTUREFACTOR, CInstanceBase::GetIndexedNameColor(CInstanceBase::NAMECOLOR_OFFLINESHOP));
	aIterator = m_OffShopPositionVector.begin();
	while (aIterator != m_OffShopPositionVector.end())
	{
		TMarkPosition & rPosition = *aIterator;
		m_WhiteMark.SetPosition(rPosition.m_fX, rPosition.m_fY);
		m_WhiteMark.Render();
		++aIterator;
	}
	// END_OF_OFFLINE_SHOP