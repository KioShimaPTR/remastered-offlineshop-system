//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offlineshop.h"
#include "offlineshop_manager.h"
#endif

//Search:

	m_pkPartyRequestEvent = NULL;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_pkOfflineShop = NULL;
	m_pkChrOfflineShopOwner = NULL;
	m_pkOfflineShopUpdateEvent = NULL;
#endif

//Search:

	memset(&m_tvLastSyncTime, 0, sizeof(m_tvLastSyncTime));

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_iMyOfflineShopTime = 0;
#endif

//Search:

	if (GetMyShop())
	{
		TPacketGCShopSign p;

		p.bHeader = HEADER_GC_SHOP_SIGN;
		p.dwVID = GetVID();
		strlcpy(p.szSign, m_stShopSign.c_str(), sizeof(p.szSign));

		d->Packet(&p, sizeof(TPacketGCShopSign));
	}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC() && GetRaceNum() == 30000)
	{
		if (entity->IsType(ENTITY_CHARACTER))
		{
			LPOFFLINESHOP shop = GetOfflineShop();

			if (shop)
			{
				TPacketGCShopSign p;
				p.bHeader	= HEADER_GC_OFFLINE_SHOP_SIGN;
				p.dwVID		= GetVID();
				strlcpy(p.szSign, shop->GetShopSign(), sizeof(p.szSign));
				d->Packet(&p, sizeof(TPacketGCShopSign));
			}
		}
	}
#endif

//Search:

	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(NULL);
	}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (GetOfflineShop())
	{
		GetOfflineShop()->RemoveGuest(this);
		SetOfflineShop(NULL);
	}
#endif

//Search function:

void CHARACTER::SetPlayerProto(const TPlayerTable * t)

//Add at the bottom of the function:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	SetOfflineShopVID(COfflineShopManager::instance().FindMyOfflineShop(GetPlayerID()));
#endif

//Search:

void CHARACTER::SetShop(LPSHOP pkShop)
{
	if ((m_pkShop = pkShop))
		SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP);
	else
	{
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP); 
		SetShopOwner(NULL);
	}
}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
void CHARACTER::SetOfflineShop(LPOFFLINESHOP pkOfflineShop)
{
	if ((m_pkOfflineShop = pkOfflineShop))
	{
		SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_OFFLINE_SHOP);
	}
	else
	{
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_OFFLINE_SHOP);
		SetOfflineShopOwner(NULL);
	}
}

void CHARACTER::OpenMyOfflineShop(const char * c_pszSign, TShopItemTable2 * pTable, BYTE bItemCount, BYTE bTime)
{
	if (thecore_pulse() - GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	SetMyOfflineShopTime();

	quest::PC * pPC = quest::CQuestManager::Instance().GetPC(GetPlayerID());

	if (pPC->IsRunning())
		return;

	if (IsOpenSafebox() || GetShop() || IsCubeOpen() || IsDead() || GetExchange() || GetOfflineShop() || GetMyShop() || bItemCount <= 0 || bItemCount > OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Make sure you don't have any open windows!"));
		return;
	}
	
	if (!COfflineShopManager::instance().MapCheck(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that map!"));
		return;
	}

	if (!COfflineShopManager::instance().ChannelCheck(g_bChannel))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> You can't do this in that channel!"));
		return;
	}
	
	if (COfflineShopManager::instance().HaveOfflineShopOnAccount(GetDesc()->GetAccountTable().id))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open twice offline shop from an account!"));
		return;
	}

	if (IsAffectFlag(AFF_SHOPOWNER))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have already offline shop. You can't open again!"));
		return;
	}

	int iOpenPrice = 0;

	if (bTime == 72)
		iOpenPrice = 25000000;

	if (GetGold() < iOpenPrice)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough gold to open an offline shop!"));
		return;
	}

	if (bTime != 48 && bTime != 72)
		return;

	const std::string& m_stOfflineShopSign = std::string(c_pszSign); 
	char szSign[SHOP_SIGN_MAX_LEN + 1];
	DBManager::instance().EscapeString(szSign, 256, c_pszSign, strlen(c_pszSign));

	if (m_stOfflineShopSign.length() == 0)
		return;

	if (strchr(szSign, '%') || strstr(szSign, "--") || strchr(szSign, ';'))
	{
		ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Özel karakterler kullanýlamaz.");
		return;
	}

	char tab[15] = {'/', '>', '<', '|', ';', ':', '}', '{', '[', ']', '%', '#', '@', '^', '&'};
	size_t x = 0;
	for(int i = 0; i < 15;i++)
	{
		x = m_stOfflineShopSign.find(tab[i]);
		if (x != std::string::npos)
		{
			ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Özel karakterler kullanýlamaz.");
			return;
		}
	}

	if (CBanwordManager::instance().CheckString(m_stOfflineShopSign.c_str(), m_stOfflineShopSign.length()))
	{
		ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Yasaklý kelime kullanýlamaz.");
		return;
	}

	for (BYTE i = 0; i < bItemCount; ++i)
	{
		LPITEM pkItem = GetItem((pTable + i)->pos);
		if (pkItem)
		{
			if (pkItem->IsEquipped())
			{
				ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Giydiðin itemi pazara koyamazsýn.");
				return;
			}

			if (pkItem->isLocked())
			{
				ChatPacket(CHAT_TYPE_INFO, "<Market Tezgâhý> Kilitli itemi pazara koyamazsýn.");
				return;
			}

			if (pkItem->GetOwner() != this)
				return;

#ifdef ENABLE_ITEM_LOCK_SYSTEM
			if (pkItem->GetItemLock() == true)
			{
				ChatPacket(CHAT_TYPE_INFO, "[Bilgi] Kilitli itemi pazara koyamazsýn.");
				return;
			}
#endif

			if (pkItem->GetWindow() != INVENTORY && pkItem->GetWindow() != DRAGON_SOUL_INVENTORY
#ifdef ENABLE_SPECIAL_STORAGE
			&& pkItem->GetWindow() != UPGRADE_INVENTORY && pkItem->GetWindow() != BOOK_INVENTORY && pkItem->GetWindow() != STONE_INVENTORY
#endif
			)
			{
				sys_err("Wrong window (%d) when setting item %lu on shop", pkItem->GetWindow(), pkItem->GetID());
				return;
			}

			if ((pTable + i)->price_type == 1 && (pTable + i)->price2 > 200 || (pTable + i)->price_type == 1 && pkItem->GetVnum() == OFFLINE_SHOP_PRICE_GOLD_BAR_1)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("OFFLINE_SHOP_PRICE_CORRECT"));
				return;
			}
			
			if ((pTable + i)->price_type == 2 && (pTable + i)->price2 > 200 || (pTable + i)->price_type == 2 && pkItem->GetVnum() == OFFLINE_SHOP_PRICE_GOLD_BAR_2)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("OFFLINE_SHOP_PRICE_CORRECT"));
				return;
			}
			
			if ((pTable + i)->price_type == 3 && (pTable + i)->price2 > 200 || (pTable + i)->price_type == 3 && pkItem->GetVnum() == OFFLINE_SHOP_PRICE_GOLD_BAR_3)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("OFFLINE_SHOP_PRICE_CORRECT"));
				return;
			}
		}
	}

	// BUG_FIX
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM %soffline_shop_item WHERE owner_id = %u and sold = 0", get_table_postfix(), GetPlayerID()));
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

	BYTE bResult = 0;
	str_to_number(bResult, row[0]);

	if (bResult)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have items in the store, fetch them first!"));
		return;
	}
	// BUG_FIX
	
	LPCHARACTER npc = CHARACTER_MANAGER::instance().SpawnMob(30000, GetMapIndex(), GetX(), GetY(), GetZ(), false, -1, false, true, GetPlayerID(), GetDesc()->GetAccountTable().id);

	if (!npc)
		return;

	npc->SetOfflineShopTimer(bTime * 60 * 60);
	npc->StartOfflineShopUpdateEvent();

	char szQuery2[512];
	snprintf(szQuery2, sizeof(szQuery2), "INSERT INTO %soffline_shop_npc(owner_id, account_id, sign, name, time, x, y, z, mapIndex, channel, level, align, guild) VALUES(%u, %u, '%s', '%s', %d, %ld, %ld, %ld, %ld, %d, %d, %d, '%s')", get_table_postfix(), GetPlayerID(), GetDesc()->GetAccountTable().id, szSign, GetName(), bTime * 60 * 60, GetX(), GetY(), GetZ(), GetMapIndex(), g_bChannel, GetLevel(), GetAlignment(), GetGuild() ? GetGuild()->GetName() : "NONAME");
	DBManager::Instance().DirectQuery(szQuery2);

	LPOFFLINESHOP pkOfflineShop = COfflineShopManager::instance().CreateOfflineShop(npc, GetPlayerID(), false);

	if (!pkOfflineShop)
		return;

	//LogManager::instance().OfflineShopLog(GetDesc()->GetAccountTable().id, "", "OPEN");
	COfflineShopManager::instance().InsertOfflineShopToAccount(GetDesc()->GetAccountTable().id);

	AddAffect(AFFECT_SHOPOWNER, POINT_NONE, 0, AFF_SHOPOWNER, INFINITE_AFFECT_DURATION, 0, true);
	PointChange(POINT_GOLD, -iOpenPrice);
	
	npc->SetOfflineShop(pkOfflineShop);
	pkOfflineShop->SetShopItems(pTable, bItemCount, this);

	npc->SetLevel(this->GetLevel());
#ifdef ENABLE_ALIGN_RENEWAL_SYSTEM
	npc->UpdateAlignmentNew(this->GetAlignment());
#else
	npc->UpdateAlignment(this->GetAlignment());
#endif
	CGuild* pGuild = this->GetGuild();
	if (pGuild != NULL)
		npc->SetGuild(pGuild);

	pkOfflineShop->SetShopSign(szSign);

	npc->SetName(GetName());
	SetOfflineShopVID(npc->GetVID());
	npc->Show(GetMapIndex(), GetX(), GetY(), GetZ(), true);

	TPacketGCShopSign p;
	p.bHeader = HEADER_GC_OFFLINE_SHOP_SIGN;
	p.dwVID = npc->GetVID();
	strlcpy(p.szSign, c_pszSign, sizeof(p.szSign));
	PacketAround(&p, sizeof(TPacketGCShopSign));

	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> Your offline shop's time is expired after %d hour(s)"), bTime);
}

EVENTFUNC(offline_shop_update_event)
{
	char_event_info* pInfo = dynamic_cast<char_event_info*>(event->info);

	if (pInfo == NULL)
	{
		sys_err("offline_shop_update_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER npc = pInfo->ch;

	if (npc == NULL)
		return 0;
	
	int remain = npc->GetOfflineShopTimer() - processing_time / passes_per_sec;

	if (remain <= 0)
	{
		sys_log(0, "offline_shop_update_event: expired %s", npc->GetName());

		COfflineShopManager::instance().DestroyOfflineShop(NULL, npc->GetVID(), false);
		DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), npc->GetOfflineShopRealOwner());
		M2_DESTROY_CHARACTER(npc);
		return 0;
	}

	if (npc->GetOfflineShopSaveTime() >= g_OfflineShopSaveTime)
	{
		DBManager::instance().DirectQuery("UPDATE %soffline_shop_npc SET time = %d WHERE owner_id = %u", get_table_postfix(), npc->GetOfflineShopTimer(), npc->GetOfflineShopRealOwner());
		npc->SetOfflineShopTimer(1);
	}

	npc->SetOfflineShopTimer(remain);
	npc->SetOfflineShopSaveTime(npc->GetOfflineShopSaveTime() + 1);
	return PASSES_PER_SEC(MIN(60, remain));
}

void CHARACTER::StartOfflineShopUpdateEvent()
{
	if (m_pkOfflineShopUpdateEvent)
		return;

	if (IsPC() || IsMonster())
		return;

	char_event_info * pInfo = AllocEventInfo<char_event_info>();
	pInfo->ch = this;
	m_pkOfflineShopUpdateEvent = event_create(offline_shop_update_event, pInfo, PASSES_PER_SEC(60));
}

void CHARACTER::StopOfflineShopUpdateEvent()
{
	m_pointsInstant.bSaveTime = 0;
	m_pointsInstant.leftTime = 0;
	event_cancel(&m_pkOfflineShopUpdateEvent);
}

void CHARACTER::DestroyOfflineShop()
{
	COfflineShopManager::instance().DestroyOfflineShop(NULL, GetVID(), false);
}
#endif

//Search:

	pkChrCauser->SetQuestNPCID(GetVID());

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC() && !pkChrCauser->IsDead())
	{
		pkChrCauser->SetOfflineShopOwner(this);
		GetOfflineShop()->AddGuest(pkChrCauser, this);
		return;
	}
#endif

//Search:

	if (iPulse - GetRefineTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ °³·®ÈÄ %dÃÊ ÀÌ³»¿¡´Â ±ÍÈ¯ºÎ,±ÍÈ¯±â¾ïºÎ¸¦ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), limittime);
		return true; 
	}

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (iPulse - GetMyOfflineShopTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛ °³·®ÈÄ %dÃÊ ÀÌ³»¿¡´Â ±ÍÈ¯ºÎ,±ÍÈ¯±â¾ïºÎ¸¦ »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), limittime);
		return true; 
	}
#endif

//Search:

	if ((iPulse - GetRefineTime()) < limit_time)
		return false;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if ((iPulse - GetMyOfflineShopTime()) < limit_time)
		return false;
#endif