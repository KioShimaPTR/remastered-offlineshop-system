//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offlineshop_manager.h"
#endif

//Search:

void CInputP2P::IamAwake(LPDESC d, const char * c_pData)
{
	std::string hostNames;
	P2P_MANAGER::instance().GetP2PHostNames(hostNames);
	sys_log(0, "P2P Awakeness check from %s. My P2P connection number is %d. and details...\n%s", d->GetHostName(), P2P_MANAGER::instance().GetDescCount(), hostNames.c_str());
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
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
			strlcpy(pack.szNameFrom, "[Market Tezgahý]", sizeof(pack.szNameFrom));

			TEMP_BUFFER buf;

			buf.write(&pack, sizeof(TPacketGCWhisper));
			buf.write(msg, len);

			pkVictimDesc->Packet(buf.read_peek(), buf.size());
		}
	}
}
#endif

//Search:

		case HEADER_GG_CHECK_AWAKENESS:
			IamAwake(d, c_pData);
			break;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		case HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE:
			SendOfflineShopMessage(d, c_pData);
			break;
#endif