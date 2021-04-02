// arat: HEADER_GD_REQUEST_CHANNELSTATUS
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	HEADER_GD_ADD_OFFLINESHOP_ITEM		= 141,
	HEADER_GD_DEL_OFFLINESHOP_ITEM		= 142,
	HEADER_GD_GET_OFFLINESHOP_ITEM		= 143,
	HEADER_GD_UPDATE_OFFLINESHOP_COUNT	= 144,
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: HEADER_DG_NOTICE
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	HEADER_DG_TOTAL_ONLINE		= 2,
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: HEADER_DG_RESPOND_CHANNELSTATUS
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	HEADER_DG_ADD_OFFLINESHOP_ITEM = 182,
	HEADER_DG_DEL_OFFLINESHOP_ITEM = 183,
	HEADER_DG_GET_OFFLINESHOP_ITEM = 184,
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: typedef struct SBanwordTable
// altına ekle:
// BEGIN_OFFLINE_SHOP
typedef struct SSpawnOfflineShopTable
{
	BYTE	bChannel;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	long	lMapIndex, x, y;
	int		iTime;
} TSpawnOfflineShopTable;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: typedef struct SChannelStatus
// altına ekle:
// BEGIN_OFFLINE_SHOP
typedef struct SOfflineShopAddItem
{
	DWORD	owner;
	WORD	pos;
	DWORD	count;
	DWORD	price;
	DWORD	vnum;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute    aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TPlayerOfflineShopAddItem;

typedef struct
{
	DWORD dwOnlinePlayers;
	DWORD dwOnlineShops;
} TPacketOnlineSize;

typedef struct
{
	bool bIncrease;
} TPacketUpdateOfflineShopsCount;
// END_OF_OFFLINE_SHOP