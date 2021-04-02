// arat: bool	g_bNoPasspod = false;
// altına ekle:
// BEGIN_OFFLINE_SHOP
bool	g_bIsOfflineShopServer = false;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: bool g_BlockCharCreation = false;
// altına ekle:
// BEGIN_OFFLINE_SHOP
int g_OfflineShopSaveTime = 5;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: TOKEN("no_regen")
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		TOKEN("offline_shop_server")
		{
			str_to_number(g_bIsOfflineShopServer, value_string);
			continue;
		}
		// END_OF_OFFLINE_SHOP