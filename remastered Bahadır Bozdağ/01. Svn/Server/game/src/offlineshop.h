#pragma once

enum
{
	OFFLINE_SHOP_MAX_DISTANCE = 1500,
};

class CGrid;

class COfflineShop
{
	public:

		typedef struct offline_shop_item
		{
			DWORD	owner_id;
			BYTE	pos;
			BYTE	count;
			long	price;
			WORD	price2;
			BYTE	price_type;
			DWORD	vnum;
			BYTE	evolution;
			BYTE	bIsSold;
			char	buyerName[CHARACTER_NAME_MAX_LEN];
			long	alSockets[ITEM_SOCKET_MAX_NUM];
			TPlayerItemAttribute	aAttr[ITEM_ATTRIBUTE_MAX_NUM];
			char	szName[ITEM_NAME_MAX_LEN + 1];
			DWORD	shop_id;

			offline_shop_item()
			{
				owner_id = 0;
				pos = 0;
				count = 0;
				price = 0;
				price2 = 0;
				price_type = 0;
				vnum = 0;
				evolution = 0;
				bIsSold = 0;
				memset(buyerName, 0, sizeof(buyerName));
				memset(alSockets, 0, sizeof(alSockets));
				memset(aAttr, 0, sizeof(aAttr));
				memset(szName, 0, sizeof(szName));
				shop_id = 0;
			}
		} OFFLINE_SHOP_ITEM;

		std::vector<OFFLINE_SHOP_ITEM>		m_offlineShopItemVector;

		COfflineShop();
		~COfflineShop();

		virtual void	SetOfflineShopNPC(LPCHARACTER npc);
		virtual bool	IsOfflineShopNPC(){ return m_pkOfflineShopNPC ? true : false; }
		LPCHARACTER		GetOfflineShopNPC() { return m_pkOfflineShopNPC; }

		void			CreateTable(DWORD dwOwnerPID);

		virtual int		Buy(LPCHARACTER ch, BYTE bPos);
		
		virtual bool	AddGuest(LPCHARACTER ch, LPCHARACTER npc);
		void			RemoveGuest(LPCHARACTER ch);
		void			RemoveAllGuest();

		void			BroadcastUpdateItem(BYTE bPos);
		void			SetShopItems(TShopItemTable2 * pTable, BYTE bItemCount, LPCHARACTER ch);

		void			Destroy(LPCHARACTER npc);

		BYTE			GetLeftItemCount();

		std::string shopSign;
		const char *		GetShopSign() { return shopSign.c_str(); };
		void				SetShopSign(const char * c) { shopSign = c; };

#ifdef ENABLE_SHOP_MARK_SYSTEM
		std::vector<OFFLINE_SHOP_ITEM>	GetItemVector() { return m_offlineShopItemVector; }
#endif

	protected:
		void			Broadcast(const void * data, int bytes);

	private:
		// Grid
		CGrid *				m_pGrid;

		// Guest Map
		typedef TR1_NS::unordered_map<LPCHARACTER, bool> GuestMapType;
		GuestMapType m_map_guest;
		// End Of Guest Map

		std::vector<DWORD> looked;

		LPCHARACTER m_pkOfflineShopNPC;
};

