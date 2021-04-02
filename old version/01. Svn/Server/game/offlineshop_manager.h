#pragma once
class COfflineShopManager : public singleton<COfflineShopManager>
{
	public:
		typedef std::map<DWORD, COfflineShop *> TShopMap;
		typedef std::map<DWORD, DWORD> TOfflineShopMap;
	public:
		COfflineShopManager();
		~COfflineShopManager();

		void ResetOfflineShopStatus(LPCHARACTER ch);

		LPOFFLINESHOP CreateOfflineShop(LPCHARACTER npc, DWORD dwOwnerPID);
		LPOFFLINESHOP FindOfflineShop(DWORD dwVID);
		DWORD FindMyOfflineShop(DWORD dwPID);

		bool MapCheck(DWORD mapIndex, DWORD empire);
		bool ChannelCheck(DWORD dwChannel);
		bool SearchOfflineShop(LPCHARACTER ch);
		
		bool HaveOfflineShopOnAccount(DWORD aID);
		void InsertOfflineShopToAccount(DWORD aID);
		void DeleteOfflineShopOnAccount(DWORD aID);

		bool StartShopping(LPCHARACTER pkChr, LPCHARACTER pkChrShopKeeper);
		void StopShopping(LPCHARACTER ch);

		void Buy(LPCHARACTER ch, BYTE bPos);
		void PutItem(LPCHARACTER ch, TItemPos item_pos, int iPrice);

		void DestroyOfflineShop(LPCHARACTER ch, DWORD dwVID, bool pcMode = false);

		bool WithdrawAllMoney(LPCHARACTER ch);
		void FetchMyItems(LPCHARACTER ch);

	private:
		TOfflineShopMap	m_Map_pkOfflineShopByNPC2;
		TShopMap		m_map_pkOfflineShopByNPC;
};

