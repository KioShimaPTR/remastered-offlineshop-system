//Search:

		{
			if (iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���λ��� ����� %d�� �̳����� ��ȯ��,��ȯ���θ� ����� �� �����ϴ�."), g_nPortalLimitTime);
				return false;
			}
		}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		if (iPulse - GetMyOfflineShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���λ��� ����� %d�� �̳����� ��ȯ��,��ȯ���θ� ����� �� �����ϴ�."), g_nPortalLimitTime);
			return false;
		}
#endif

//Search:

	if (iPulse		- GetSafeboxLoadTime()		< PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse	- GetRefineTime()			< PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse	- GetMyShopTime()			< PASSES_PER_SEC(g_nPortalLimitTime))

//Change:

	if (iPulse		- GetSafeboxLoadTime()		< PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse	- GetRefineTime()			< PASSES_PER_SEC(g_nPortalLimitTime)
		|| iPulse	- GetMyShopTime()			< PASSES_PER_SEC(g_nPortalLimitTime)
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		|| iPulse	- GetMyOfflineShopTime()	< PASSES_PER_SEC(g_nPortalLimitTime)
#endif
		)

//Search:

	if (m_bIsObserver)	return false;
	if (GetShop())		return false;
	if (GetMyShop())	return false;
	if (m_bUnderRefine)	return false;
	if (IsWarping())	return false;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (GetOfflineShop()) return false;
#endif