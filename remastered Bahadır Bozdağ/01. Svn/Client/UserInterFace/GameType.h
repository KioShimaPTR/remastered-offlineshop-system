//Search:

	SLOT_TYPE_DRAGON_SOUL_INVENTORY,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	SLOT_TYPE_OFFLINE_SHOP,
#endif

//Search:

typedef struct packet_shop_item
{
    DWORD       vnum;
    DWORD       price;
    BYTE        count;
	BYTE		display_pos;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TShopItemData;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct packet_shop_offline_item
{
    DWORD       vnum;
    DWORD       price;
	WORD		price2;
	BYTE		price_type;
    BYTE        count;
	BYTE		display_pos;
	BYTE		evolution;
	BYTE		bIsSold;
	char		buyerName[64];
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];

	packet_shop_offline_item() {
		buyerName[64] = '\0';
		bIsSold = 0;
	}
} TShopOfflineItemData;
#endif