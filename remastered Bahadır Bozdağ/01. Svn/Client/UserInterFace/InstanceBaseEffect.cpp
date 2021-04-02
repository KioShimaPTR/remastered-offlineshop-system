//Search:

	if (IsResource())
		return;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC())
		return;
#endif

//Search:

void CInstanceBase::__DetachSelectEffect()
{
	__EffectContainer_DetachEffect(EFFECT_SELECT);
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CInstanceBase::__AttachOfflineShopSelectEffect()
{
	__EffectContainer_AttachEffect(EFFECT_SELECT_OFFLINESHOP);
}

void CInstanceBase::__DetachOfflineShopSelectEffect()
{
	__EffectContainer_DetachEffect(EFFECT_SELECT_OFFLINESHOP);
}
#endif

//Search:

void CInstanceBase::__DetachTargetEffect()
{
	__EffectContainer_DetachEffect(EFFECT_TARGET);
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CInstanceBase::__AttachOfflineShopTargetEffect()
{
	__EffectContainer_AttachEffect(EFFECT_TARGET_OFFLINESHOP);
}

void CInstanceBase::__DetachOfflineShopTargetEffect()
{
	__EffectContainer_DetachEffect(EFFECT_TARGET_OFFLINESHOP);
}
#endif

//Search:

		return NAMECOLOR_NPC;

//Change:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		if(GetVirtualNumber() == 30000)
			return NAMECOLOR_OFFLINESHOP;
		else
			return NAMECOLOR_NPC;
#else
		return NAMECOLOR_NPC;
#endif