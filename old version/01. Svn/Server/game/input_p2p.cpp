// arat: #include "threeway_war.h"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "offlineshop_manager.h"
#include "sectree_manager.h"
#include "buffer_manager.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: void CInputP2P::IamAwake(LPDESC d, const char * c_pData)
// altına ekle:
// BEGIN_OFFLINE_SHOP
void CInputP2P::SendOfflineShopMessage(LPDESC d, const char * c_pData)
{
	TPacketGGOfflineShopMessage * p = (TPacketGGOfflineShopMessage *)c_pData;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(p->dwTargetPID);
	
	if (ch)
	{
		LPDESC pkVictimDesc = ch->GetDesc();

		if (pkVictimDesc)
		{
			char msg[CHAT_MAX_LEN + 1];
			snprintf(msg, sizeof(msg), LC_TEXT("Your item %s sold, buyer is: %s."), p->szItemName, p->szName);

			TPacketGCWhisper pack;

			int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

			pack.bHeader = HEADER_GC_WHISPER;
			pack.wSize = sizeof(TPacketGCWhisper) + len;
			pack.bType = WHISPER_TYPE_SYSTEM;
			// pack.bLevel = 0;
			strlcpy(pack.szNameFrom, "[Market Tezgahı]", sizeof(pack.szNameFrom));

			TEMP_BUFFER buf;

			buf.write(&pack, sizeof(TPacketGCWhisper));
			buf.write(msg, len);

			pkVictimDesc->Packet(buf.read_peek(), buf.size());
		}
	}
}
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: case HEADER_GG_CHECK_AWAKENESS:
// altına ekle:
		// BEGIN_OFFLINE_SHOP
		case HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE:
			SendOfflineShopMessage(d, c_pData);
			break;
		// END_OF_OFFLINE_SHOP