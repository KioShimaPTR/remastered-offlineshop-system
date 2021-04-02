//Search:

typedef struct SShopItemTable
{
	DWORD		vnum;
	BYTE		count;

	TItemPos	pos;
	DWORD		price;
	BYTE		display_pos;
} TShopItemTable;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct SShopItemTable2
{
	DWORD		vnum;
	BYTE		count;

	TItemPos	pos;
	DWORD		price;
	WORD		price2;
	BYTE		price_type;
	BYTE		display_pos;
} TShopItemTable2;
#endif

//Search:

typedef struct SBanwordTable
{
	char szWord[BANWORD_MAX_LEN + 1];
} TBanwordTable;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
typedef struct SSpawnOfflineShopTable
{
	BYTE	bChannel;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	long	lMapIndex, x, y;
	int		iTime;
} TSpawnOfflineShopTable;
#endif

//Searc:

typedef struct SChannelStatus
{
	short nPort;
	BYTE bStatus;
} TChannelStatus;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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
#endif