// arat: void CInstanceBase::OnSelected()
// değiştir:
void CInstanceBase::OnSelected()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	// BEGIN_OFFLINE_SHOP
	if (IsOfflineShopNPC())
		__AttachOfflineShopSelectEffect();
	else
		__AttachSelectEffect();
	// END_OF_OFFLINE_SHOP
}
/*******************************************************************************/
// arat: void CInstanceBase::OnUnselected()
// değiştir:
void CInstanceBase::OnUnselected()
{
	// BEGIN_OFFLINE_SHOP
	if (IsOfflineShopNPC())
		__DetachOfflineShopSelectEffect();
	else
		__DetachSelectEffect();
	// END_OF_OFFLINE_SHOP
}
/*******************************************************************************/
// arat: void CInstanceBase::OnTargeted()
// değiştir:
void CInstanceBase::OnTargeted()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	// BEGIN_OFFLINE_SHOP
	if (IsOfflineShopNPC())
		__AttachOfflineShopTargetEffect();
	else
		__AttachTargetEffect();
	// END_OF_OFFLINE_SHOP
}
/*******************************************************************************/
// arat: void CInstanceBase::OnUntargeted()
// değiştir:
void CInstanceBase::OnUntargeted()
{
	// BEGIN_OFFLINE_SHOP
	if (IsOfflineShopNPC())
		__DetachOfflineShopTargetEffect();
	else
		__DetachTargetEffect();
	// END_OF_OFFLINE_SHOP
}
/*******************************************************************************/
// arat: BOOL CInstanceBase::IsPoly()
// üstüne ekle:
// BEGIN_OFFLINE_SHOP
BOOL CInstanceBase::IsOfflineShopNPC()
{
	if (GetVirtualNumber() == 30000)
		return true;

	return false;
}
// END_OF_OFFLINE_SHOP