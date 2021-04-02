#pragma once
class COfflineShopManager : public singleton<COfflineShopManager>
{
	public:
		typedef std::map<DWORD, COfflineShop *> TShopMap;
		typedef std::map<DWORD, DWORD> TOfflineShopMap;
#ifdef ENABLE_MARKET_CONTROL_SYSTEM
		typedef std::map<DWORD, int> TMarketControl;
#endif
	public:
		COfflineShopManager();
		~COfflineShopManager();

		void ResetOfflineShopStatus(LPCHARACTER ch);

		LPOFFLINESHOP CreateOfflineShop(LPCHARACTER npc, DWORD dwOwnerPID, bool bBoot);
		LPOFFLINESHOP FindOfflineShop(DWORD dwVID);
		DWORD FindMyOfflineShop(DWORD dwPID);

		bool MapCheck(DWORD mapIndex);
		bool ChannelCheck(DWORD dwChannel);
		bool SearchOfflineShop(LPCHARACTER ch);
		
		bool HaveOfflineShopOnAccount(DWORD aID);
		void InsertOfflineShopToAccount(DWORD aID);
		void DeleteOfflineShopOnAccount(DWORD aID);

		bool StartShopping(LPCHARACTER pkChr, LPCHARACTER pkChrShopKeeper);
		void StopShopping(LPCHARACTER ch);

		void Buy(LPCHARACTER ch, BYTE bPos);

		void DestroyOfflineShop(LPCHARACTER ch, DWORD dwVID, bool pcMode = false);

		bool WithdrawMoney(LPCHARACTER ch, DWORD dwRequiredMoney);
		bool WithdrawBar1(LPCHARACTER ch, DWORD dwRequiredBar);
		bool WithdrawBar2(LPCHARACTER ch, DWORD dwRequiredBar);
		bool WithdrawBar3(LPCHARACTER ch, DWORD dwRequiredBar);
		void FetchMyItems(LPCHARACTER ch, BYTE bPos);
		void RefreshPrice(LPCHARACTER ch);
		void UnsoldItems(LPCHARACTER ch);

	private:
		TOfflineShopMap	m_Map_pkOfflineShopByNPC2;
		TShopMap		m_map_pkOfflineShopByNPC;
#ifdef ENABLE_MARKET_CONTROL_SYSTEM
		TMarketControl	m_map_MarketControl;
#endif
};

