// arat: if (IsResource())
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (IsOfflineShopNPC())
		return;
	// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void CInstanceBase::__DetachSelectEffect()
// altına ekle:
// BEGIN_OFFLINE_SHOP
void CInstanceBase::__AttachOfflineShopSelectEffect()
{
	__EffectContainer_AttachEffect(EFFECT_SELECT_OFFLINESHOP);
}

void CInstanceBase::__DetachOfflineShopSelectEffect()
{
	__EffectContainer_DetachEffect(EFFECT_SELECT_OFFLINESHOP);
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: void CInstanceBase::__DetachTargetEffect()
// altına ekle:
// BEGIN_OFFLINE_SHOP
void CInstanceBase::__AttachOfflineShopTargetEffect()
{
	__EffectContainer_AttachEffect(EFFECT_TARGET_OFFLINESHOP);
}

void CInstanceBase::__DetachOfflineShopTargetEffect()
{
	__EffectContainer_DetachEffect(EFFECT_TARGET_OFFLINESHOP);
}
// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: else if (IsNPC())
// değiştir:
	// BEGIN_OFFLINE_SHOP
	else if (IsNPC())
	{
		if(GetVirtualNumber() == 30000)
			return NAMECOLOR_OFFLINESHOP;
		else
			return NAMECOLOR_NPC;
	}
	// END_OF_OFFLINE_SHOP