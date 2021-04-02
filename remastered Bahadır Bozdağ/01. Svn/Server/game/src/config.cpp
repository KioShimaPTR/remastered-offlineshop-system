//Search:

bool	g_bNoRegen = false;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
bool	g_bIsOfflineShopServer = false;
#endif

//Search:

bool g_BlockCharCreation = false;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
int g_OfflineShopSaveTime = 5;
#endif

//Search:

		TOKEN("no_regen")
		{
			g_bNoRegen = true;
			continue;
		}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		TOKEN("offline_shop_server")
		{
			str_to_number(g_bIsOfflineShopServer, value_string);
			continue;
		}
#endif