//Search:

	HEADER_CG_MYSHOP							= 32,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	HEADER_CG_OFFLINE_SHOP						= 33,
	HEADER_CG_MY_OFFLINE_SHOP					= 34,
#endif

//Search:

	HEADER_GC_SHOP_SIGN							= 37,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	HEADER_GC_OFFLINE_SHOP						= 38,
	HEADER_GC_OFFLINE_SHOP_SIGN					= 39,
#endif

//Search:

	HEADER_GG_CHECK_AWAKENESS		= 29,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE			= 30,
#endif

//Search:

	SHOP_SUBHEADER_CG_SELL2,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	SHOP_SUBHEADER_CG_DESTROY_OFFLINE_SHOP,
	SHOP_SUBHEADER_CG_REFRESH_PRICE,
	SHOP_SUBHEADER_CG_WITHDRAW_MONEY,
	SHOP_SUBHEADER_CG_WITHDRAW_BAR_1,
	SHOP_SUBHEADER_CG_WITHDRAW_BAR_2,
	SHOP_SUBHEADER_CG_WITHDRAW_BAR_3,
	SHOP_SUBHEADER_CG_UNSOLD_ITEMS,
	SHOP_SUBHEADER_CG_FETCH_ITEM,
	SHOP_SUBHEADER_CG_TELEPORT,
#endif

//Search:

struct packet_shop_item
{   
	DWORD       vnum;
	DWORD       price;
	BYTE        count;
	BYTE		display_pos;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
struct packet_shop_offline_item
{
	DWORD       vnum;
	DWORD       price;
	WORD		price2;
	BYTE		price_type;
	BYTE        count;
	BYTE		display_pos;
	BYTE		evolution;
	BYTE		bIsSold;
	char		buyerName[CHARACTER_NAME_MAX_LEN];
	long		alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};
#endif

//Search:

typedef struct packet_shop_update_item
{
	BYTE			pos;
	struct packet_shop_item	item;
} TPacketGCShopUpdateItem;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct packet_shop_update_offline_item
{
	BYTE	pos;
	struct packet_shop_offline_item	item;
} TPacketGCShopUpdateOfflineItem;
#endif

//Search:

typedef struct SPacketCGMyShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE	bCount;
} TPacketCGMyShop;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct SOfflineShopItemPrice
{
	long long	llMoney;
	DWORD	dwBar1;
	DWORD	dwBar2;
	DWORD	dwBar3;
} TPacketGCOfflineShopPrice;

typedef struct TPacketCGMyOfflineShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE	bCount;
	BYTE	bTime;
} TPacketCGMyOfflineShop;
#endif

//Add to end:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct SPacketGGOfflineShopMessage
{
	BYTE	bHeader;
	DWORD	dwTargetPID;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	char	szItemName[ITEM_NAME_MAX_LEN + 1];
} TPacketGGOfflineShopMessage;
#endif