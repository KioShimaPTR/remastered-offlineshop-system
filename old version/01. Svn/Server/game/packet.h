// arat: HEADER_CG_MYSHOP
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	HEADER_CG_OFFLINE_SHOP			= 56,
	HEADER_CG_MY_OFFLINE_SHOP		= 57,
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: HEADER_GC_QUEST_CONFIRM
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	HEADER_GC_OFFLINE_SHOP						= 47,
	HEADER_GC_OFFLINE_SHOP_SIGN					= 48,
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: HEADER_GG_CHECK_AWAKENESS
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE			= 30,
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: SHOP_SUBHEADER_CG_SELL2
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	SHOP_SUBHEADER_CG_PUT_ITEM,
	SHOP_SUBHEADER_CG_DESTROY_OFFLINE_SHOP,
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: struct packet_shop_item
// altına ekle:
// BEGIN_OFFLINE_SHOP
struct packet_shop_offline_item
{
	DWORD       vnum;
	DWORD       price;
	BYTE        count;
	BYTE		display_pos;
	bool		bIsSold;
	char		buyerName[CHARACTER_NAME_MAX_LEN];
	long		alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: typedef struct packet_shop_start
// altına ekle:
// BEGIN_OFFLINE_SHOP
typedef struct packet_offline_shop_start
{
	DWORD	owner_vid;
	struct packet_shop_offline_item items[OFFLINE_SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCOfflineShopStart;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: typedef struct packet_shop_update_item
// altına ekle:
// BEGIN_OFFLINE_SHOP
typedef struct packet_shop_update_offline_item
{
	BYTE	pos;
	struct packet_shop_offline_item	item;
} TPacketGCShopUpdateOfflineItem;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: typedef struct SPacketCGMyShop
// altına ekle:
// BEGIN_OFFLINE_SHOP
typedef struct SOfflineShopItemTable
{
	TItemPos	item_pos;
	int			iPrice;
} TOfflineShopItemTable;

typedef struct TPacketCGMyOfflineShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE	bCount;
	BYTE	bTime;
} TPacketCGMyOfflineShop;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: typedef struct SPacketGCStateCheck
// altına ekle:
// BEGIN_OFFLINE_SHOP
typedef struct SPacketGGOfflineShopMessage
{
	BYTE	bHeader;
	DWORD	dwTargetPID;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	char	szItemName[ITEM_NAME_MAX_LEN + 1];
} TPacketGGOfflineShopMessage;
// END_OF_OFFLINE_SHOP