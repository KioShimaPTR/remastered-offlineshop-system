#pragma once

enum
{
	OFFLINE_SHOP_MAX_DISTANCE = 1500,
};

class COfflineShop
{
	public:

		typedef struct offline_shop_item
		{
			DWORD	owner_id;
			BYTE	pos;
			BYTE	count;
			long	price;
			DWORD	vnum;
			bool	bIsSold;
			
			char					buyerName[CHARACTER_NAME_MAX_LEN];
			long					alSockets[ITEM_SOCKET_MAX_NUM];
			TPlayerItemAttribute	aAttr[ITEM_ATTRIBUTE_MAX_NUM];

			offline_shop_item()
			{
				owner_id = 0;
				pos = 0;
				count = 0;
				price = 0;
				vnum = 0;
				bIsSold = false;

				memset(buyerName, 0, sizeof(buyerName));
				memset(alSockets, 0, sizeof(alSockets));
				memset(aAttr, 0, sizeof(aAttr));
			}
		} OFFLINE_SHOP_ITEM;

		std::vector<OFFLINE_SHOP_ITEM>		m_offlineShopItemVector;

		COfflineShop();
		~COfflineShop();

		virtual void	SetOfflineShopNPC(LPCHARACTER npc);
		virtual bool	IsOfflineShopNPC(){ return m_pkOfflineShopNPC ? true : false; }

		virtual int		Buy(LPCHARACTER ch, BYTE bPos);
		
		virtual bool	AddGuest(LPCHARACTER ch, LPCHARACTER npc);
		void			RemoveGuest(LPCHARACTER ch);
		void			RemoveAllGuest();

		void			BroadcastUpdateItem(BYTE bPos);
		void			SetShopItems(TShopItemTable * pTable, BYTE bItemCount, LPCHARACTER ch);

		void			PutItem(LPCHARACTER ch, TItemPos item_pos, int iPrice);

		void			Destroy(LPCHARACTER npc);
		void			GiveBackMyItems(LPCHARACTER ch);

		std::string shopSign;
		const char *		GetShopSign() { return shopSign.c_str(); };
		void				SetShopSign(const char * c) { shopSign = c; };

	protected:
		void			Broadcast(const void * data, int bytes);

	private:
		// Grid
		CGrid *				m_pGrid;

		// Guest Map
		typedef TR1_NS::unordered_map<LPCHARACTER, bool> GuestMapType;
		GuestMapType m_map_guest;
		// End Of Guest Map

		LPCHARACTER m_pkOfflineShopNPC;
};

