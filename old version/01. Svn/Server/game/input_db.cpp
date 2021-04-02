// arat: #define MAPNAME_DEFAULT	"none"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "offlineshop_manager.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (ch->GetShopOwner() || ch->GetExchange() || ch->GetMyShop() || ch->IsCubeOpen())
// değiştir:
	// BEGIN_OFFLINE_SHOP
	if (ch->GetShopOwner() || ch->GetExchange() || ch->GetMyShop() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: void CInputDB::WeddingEnd(TPacketWeddingEnd* p)
// altına ekle:
// BEGIN_OFFLINE_SHOP
void CInputDB::HandleCurrentOnline(TPacketOnlineSize * p)
{
	DESC_MANAGER::instance().BroadcastOnlineCount(p->dwOnlinePlayers, p->dwOnlineShops);
}
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: case HEADER_DG_RESPOND_CHANNELSTATUS:
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	case HEADER_DG_TOTAL_ONLINE:
		HandleCurrentOnline((TPacketOnlineSize*)c_pData);
		break;
	// END_OF_OFFLINE_SHOP