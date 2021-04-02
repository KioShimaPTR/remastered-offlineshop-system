//Search:

BOOL CInstanceBase::IsInvisibility()
{
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;

	return false;
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
BOOL CInstanceBase::IsOfflineShop()
{
	if (IsAffect(AFFECT_SHOPOWNER))
		return true;

	return false;
}
#endif

//Search:

	__AttachSelectEffect();

//Change:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC())
		__AttachOfflineShopSelectEffect();
	else
		__AttachSelectEffect();
#else
	__AttachSelectEffect();
#endif

//Search:

	__DetachSelectEffect();

//Change:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC())
		__DetachOfflineShopSelectEffect();
	else
		__DetachSelectEffect();
#else
	__DetachSelectEffect();
#endif

//Search:

	__AttachTargetEffect();

//Change:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC())
		__AttachOfflineShopTargetEffect();
	else
		__AttachTargetEffect();
#else
	__AttachTargetEffect();
#endif

//Search:

	__DetachTargetEffect();

//Change:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC())
		__DetachOfflineShopTargetEffect();
	else
		__DetachTargetEffect();
#else
	__DetachTargetEffect();
#endif

//Search:

BOOL CInstanceBase::IsPoly()
{
	return m_GraphicThingInstance.IsPoly();
}

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
BOOL CInstanceBase::IsOfflineShopNPC()
{
	if (GetVirtualNumber() == 30000)
	{
		return true;
	}
	return false;
}
#endif