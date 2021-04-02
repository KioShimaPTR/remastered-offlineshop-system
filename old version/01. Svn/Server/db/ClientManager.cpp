// arat: DWORD g_dwUsageAvg = 0;
// altına ekle:
// BEGIN_OFFLINE_SHOP
DWORD dwOfflineShopCount = 0;
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: case HEADER_GD_REQUEST_CHANNELSTATUS:
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			case HEADER_GD_ADD_OFFLINESHOP_ITEM:
				QUERY_ADD_OFFLINESHOP_ITEM((TPlayerOfflineShopAddItem*) data, peer);
				break;

			case HEADER_GD_UPDATE_OFFLINESHOP_COUNT:
				QUERY_UPDATE_OFFLINE_SHOP_COUNT((TPacketUpdateOfflineShopsCount*)data);
				break;
			// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: DWORD dwCount = CClientManager::instance().GetUserCount();
// altına ekle:
			// BEGIN_OFFLINE_SHOP
			TPacketOnlineSize p;
			p.dwOnlinePlayers = dwCount;
			p.dwOnlineShops = dwOfflineShopCount;
			HandleCurrentOnline(&p);
			// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: DWORD CClientManager::GetUserCount()
// üstüne ekle:
// BEGIN_OFFLINE_SHOP
void CClientManager::HandleCurrentOnline(TPacketOnlineSize * p)
{
	ForwardPacket(HEADER_DG_TOTAL_ONLINE, p, sizeof(TPacketOnlineSize));
}
// END_OF_OFFLINE_SHOP