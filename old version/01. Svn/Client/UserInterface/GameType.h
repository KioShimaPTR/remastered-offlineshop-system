// arat: SLOT_TYPE_DRAGON_SOUL_INVENTORY,
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	SLOT_TYPE_OFFLINE_SHOP,
	// END_OF_OFFLINE_SHOP
/*******************************************************************************/
// arat: typedef struct packet_shop_item
// altına ekle:
// BEGIN_OFFLINE_SHOP
typedef struct packet_shop_offline_item
{
    DWORD       vnum;
    DWORD       price;
    BYTE        count;
	BYTE		display_pos;
	bool		bIsSold;
	char		buyerName[24];
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];

	packet_shop_offline_item() {
		buyerName[24] = '\0';
		bIsSold = false;
	}
} TShopOfflineItemData;
// END_OF_OFFLINE_SHOP