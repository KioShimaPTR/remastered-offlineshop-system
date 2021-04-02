// arat: void LogManager::DragonSlayLog(DWORD dwGuildID, DWORD dwDragonVnum, DWORD dwStartTime, DWORD dwEndTime)
// altına ekle:
// BEGIN_OFFLINE_SHOP
void LogManager::OfflineShopLog(DWORD dwAID, const char * pszItem, const char * action)
{
	Query("INSERT INTO offlineshop_log (account_id, action, item, time) VALUES ('%d', '%s', '%s', NOW())", dwAID, action, pszItem);
}
// END_OF_OFFLINE_SHOP