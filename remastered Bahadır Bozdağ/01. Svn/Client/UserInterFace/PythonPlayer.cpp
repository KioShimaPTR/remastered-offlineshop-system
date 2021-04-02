//Search:

DWORD CPythonPlayer::GetPlayTime()
{
	return m_dwPlayTime;
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CPythonPlayer::SetCurrentOfflineShopMoney(long long llMoney)
{
	m_llCurrentOfflineShopMoney = llMoney;
}

long long CPythonPlayer::GetCurrentOfflineShopMoney()
{
	return m_llCurrentOfflineShopMoney;
}

void CPythonPlayer::SetCurrentOfflineShopBar1(DWORD dwBar1)
{
	m_dwCurrentOfflineShopBar1 = dwBar1;
}

DWORD CPythonPlayer::GetCurrentOfflineShopBar1()
{
	return m_dwCurrentOfflineShopBar1;
}

void CPythonPlayer::SetCurrentOfflineShopBar2(DWORD dwBar2)
{
	m_dwCurrentOfflineShopBar2 = dwBar2;
}

DWORD CPythonPlayer::GetCurrentOfflineShopBar2()
{
	return m_dwCurrentOfflineShopBar2;
}

void CPythonPlayer::SetCurrentOfflineShopBar3(DWORD dwBar3)
{
	m_dwCurrentOfflineShopBar3 = dwBar3;
}

DWORD CPythonPlayer::GetCurrentOfflineShopBar3()
{
	return m_dwCurrentOfflineShopBar3;
}
#endif