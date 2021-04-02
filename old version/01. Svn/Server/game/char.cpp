// arat: #include "DragonSoul.h"
// altına ekle:
// BEGIN_OFFLINE_SHOP
#include "offlineshop.h"
#include "offlineshop_manager.h"
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: m_pkPartyRequestEvent = NULL;
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	m_pkOfflineShop = NULL;
	m_pkChrOfflineShopOwner = NULL;
	m_pkOfflineShopUpdateEvent = NULL;
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: m_iSyncHackCount = 0;
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	m_iMyOfflineShopTime = 0;
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: sys_log(3, "EntityInsert %s (RaceNum %d) (%d %d) TO %s",
// if bloğunun üstüne ekle:
	// BEGIN_OFFLINE_SHOP
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
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: void CHARACTER::Disconnect(const char * c_pszReason)
// içerisinde "if (GetShop())" bloğunun altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (GetOfflineShop())
	{
		GetOfflineShop()->RemoveGuest(this);
		SetOfflineShop(NULL);
	}
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: m_pkMiningEvent = event_create(kill_ore_load_event, info, PASSES_PER_SEC(number(7 * 60, 15 * 60)));
// if bloğunun altına ekle:
	// BEGIN_OFFLINE_SHOP
	SetOfflineShopVID(COfflineShopManager::instance().FindMyOfflineShop(GetPlayerID()));
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: void CHARACTER::SetShop(LPSHOP pkShop)
// kod bloğunun altına ekle:
// BEGIN_OFFLINE_SHOP
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

void CHARACTER::OpenMyOfflineShop(const char * c_pszSign, TShopItemTable * pTable, BYTE bItemCount, BYTE bTime)
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
	
	if (!COfflineShopManager::instance().MapCheck(GetMapIndex(), GetEmpire()))
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

	int iOpenPrice = 10000, iOpenLevel = 15;

	if (GetGold() < iOpenPrice)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough gold to open an offline shop! (%d)"), iOpenPrice);
		return;
	}

	if (GetLevel() < iOpenLevel)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have to be atleast 15. level to open shop! (%d)"), iOpenLevel);
		return;
	}
	
	// BUG_FIX
	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery),
		"SELECT pos, count, vnum, socket0, socket1, socket2, "
		"attrtype0, attrvalue0, attrtype1, attrvalue1,"
		"attrtype2, attrvalue2, attrtype3, attrvalue3,"
		"attrtype4, attrvalue4, attrtype5, attrvalue5,"
		"attrtype6, attrvalue6 "
		"FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), GetPlayerID());

	std::auto_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows > 0)
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
	
	LPOFFLINESHOP pkOfflineShop = COfflineShopManager::instance().CreateOfflineShop(npc, GetPlayerID());

	if (!pkOfflineShop)
		return;

	//LogManager::instance().OfflineShopLog(GetDesc()->GetAccountTable().id, "", "OPEN");
	COfflineShopManager::instance().InsertOfflineShopToAccount(GetDesc()->GetAccountTable().id);

	AddAffect(AFFECT_SHOPOWNER, POINT_NONE, 0, AFF_SHOPOWNER, INFINITE_AFFECT_DURATION, 0, true);
	PointChange(POINT_GOLD, -iOpenPrice);
	
	npc->SetOfflineShop(pkOfflineShop);
	pkOfflineShop->SetShopItems(pTable, bItemCount, this);

	char szSign[SHOP_SIGN_MAX_LEN + 1];
	strlcpy(szSign, c_pszSign, sizeof(szSign));
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

	// COUNTER_UPDATE
	TPacketUpdateOfflineShopsCount pCount;
	pCount.bIncrease = true;
	db_clientdesc->DBPacket(HEADER_GD_UPDATE_OFFLINESHOP_COUNT, 0, &pCount, sizeof(pCount));
	// COUNTER_UPDATE

	ChatPacket(CHAT_TYPE_COMMAND, "CloseOfflineShopBuilderWindow");
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
		M2_DESTROY_CHARACTER(npc);
		return 0;
	}

	if (npc->GetOfflineShopSaveTime() >= g_OfflineShopSaveTime)
		npc->SetOfflineShopTimer(1);

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
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if ((GetExchange() || IsOpenSafebox() || GetShopOwner()) || IsCubeOpen())
// değiştir:
// BEGIN_OFFLINE_SHOP
if ((GetExchange() || IsOpenSafebox() || GetShopOwner()) || IsCubeOpen() || GetOfflineShopOwner())
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if ((pkChrCauser->GetExchange() || pkChrCauser->IsOpenSafebox() || pkChrCauser->GetMyShop() || pkChrCauser->GetShopOwner()) || pkChrCauser->IsCubeOpen())
// değiştir:
// BEGIN_OFFLINE_SHOP
if ((pkChrCauser->GetExchange() || pkChrCauser->IsOpenSafebox() || pkChrCauser->GetMyShop() || pkChrCauser->GetShopOwner()) || pkChrCauser->IsCubeOpen() || pkChrCauser->GetOfflineShopOwner())
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if ((GetExchange() || IsOpenSafebox() || IsCubeOpen()))
// değiştir:
// BEGIN_OFFLINE_SHOP
if ((GetExchange() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner()))
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: sys_err("%s.OnClickFailure(%s) - target is PC", pkChrCauser->GetName(), GetName());
// "return; } }" dan sonra ekle:
	// BEGIN_OFFLINE_SHOP
	if (IsOfflineShopNPC() && !pkChrCauser->IsDead())
	{
		pkChrCauser->SetOfflineShopOwner(this);
		GetOfflineShop()->AddGuest(pkChrCauser, this);
		return;
	}
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
// değiştir:
// BEGIN_OFFLINE_SHOP
if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (iPulse - GetRefineTime() < PASSES_PER_SEC(limittime))
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if (iPulse - GetMyOfflineShopTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 개량후 %d초 이내에는 귀환부,귀환기억부를 사용할 수 없습니다."), limittime);
		return true; 
	}
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if ((iPulse - GetRefineTime()) < limit_time)
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	if ((iPulse - GetMyOfflineShopTime()) < limit_time)
		return false;
	// END_OF_OFFLINE_SHOP
/***********************************************************************************************/
// arat: if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
// değiştir:
// BEGIN_OFFLINE_SHOP
if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
// END_OF_OFFLINE_SHOP
/***********************************************************************************************/