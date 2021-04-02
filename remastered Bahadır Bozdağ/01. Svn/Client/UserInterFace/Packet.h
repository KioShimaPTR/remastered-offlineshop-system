//Search:

	HEADER_CG_MYSHOP                            = 32,

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

	SHOP_HOST_ITEM_MAX_NUM = 40,

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	OFFLINE_SHOP_HOST_ITEM_MAX_NUM = 45,
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

typedef struct SShopItemTable
{
    DWORD		vnum;
    BYTE		count;

    TItemPos	pos;			// PC 상점에만 이용
    DWORD		price;			// PC 상점에만 이용
    BYTE		display_pos;	//	PC 상점에만 이용, 보일 위치.
} TShopItemTable;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct SShopItemTable2
{
    DWORD		vnum;
    BYTE		count;

    TItemPos	pos;			// PC 상점에만 이용
    DWORD		price;			// PC 상점에만 이용
	WORD		price2;
	BYTE		price_type;
    BYTE		display_pos;	//	PC 상점에만 이용, 보일 위치.
} TShopItemTable2;
#endif

//Search:

typedef struct SPacketCGMyShop
{
    BYTE        bHeader;
    char        szSign[SHOP_SIGN_MAX_LEN + 1];
    BYTE        bCount;	// count of TShopItemTable, max 39
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

//Search:

typedef struct packet_shop_start
{
	struct packet_shop_item		items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct packet_offline_shop_start
{
	struct packet_shop_offline_item		items[OFFLINE_SHOP_HOST_ITEM_MAX_NUM];
	DWORD	looked;
} TPacketGCOfflineShopStart;

typedef struct packet_shop_update_offline_item
{
	BYTE								pos;
	struct packet_shop_offline_item		item;
} TPacketGCShopUpdateOfflineShopItem;
#endif