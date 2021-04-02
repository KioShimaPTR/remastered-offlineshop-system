//Add includes:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offlineshop.h"
#include "offlineshop_manager.h"
#endif

//Search:

	// castle_boot
	castle_boot();

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	MYSQL_ROW row;
	std::auto_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT * FROM %soffline_shop_npc", get_table_postfix()));

	if (pMsg->Get()->uiNumRows != 0)
	{
		while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			char szName[CHARACTER_NAME_MAX_LEN + 1];
			char szSign[SHOP_SIGN_MAX_LEN + 1];
			DWORD dwOwnerID = 0;
			DWORD dwAccountID = 0;
			long lMapIndex = 0, x = 0, y = 0, z = 0;
			int iTime = 0;
			BYTE bChannel = 0;
			int ilevel = 0;
			int iAlignment = 0;
			char szGuild[GUILD_NAME_MAX_LEN + 1];

			str_to_number(dwOwnerID, row[0]);
			str_to_number(dwAccountID, row[1]);
			snprintf(szSign, sizeof(szSign), "%s", row[2]);
			snprintf(szName, sizeof(szName), "%s", row[3]);
			str_to_number(iTime, row[4]);
			str_to_number(x, row[5]);
			str_to_number(y, row[6]);
			str_to_number(z, row[7]);
			str_to_number(lMapIndex, row[8]);
			str_to_number(bChannel, row[9]);
			str_to_number(ilevel, row[10]);
			str_to_number(iAlignment, row[11]);
			snprintf(szGuild, sizeof(szGuild), "%s", row[12]);

			if (g_bChannel == bChannel)
			{
				auto npc = CHARACTER_MANAGER::instance().SpawnMob(30000, lMapIndex, x, y, z, false, -1, false, true, dwOwnerID, dwAccountID);
				COfflineShopManager::instance().InsertOfflineShopToAccount(dwAccountID);
				if (npc)
				{
					npc->SetName(szName);
					npc->SetLevel(ilevel);
#ifdef ENABLE_ALIGN_RENEWAL_SYSTEM
					npc->UpdateAlignmentNew(iAlignment);
#else
					npc->UpdateAlignment(iAlignment);
#endif
					auto pGuild = CGuildManager::instance().FindGuildByName(szGuild);
					if (pGuild != NULL)
						npc->SetGuild(pGuild);

					if (iTime != 0)
					{
						npc->SetOfflineShopTimer(iTime);
						npc->StartOfflineShopUpdateEvent();
					}

					auto pkOfflineShop = COfflineShopManager::instance().CreateOfflineShop(npc, dwOwnerID, true);

					if (pkOfflineShop)
						pkOfflineShop->SetShopSign(szSign);

					if (pkOfflineShop)
						npc->SetOfflineShop(pkOfflineShop);

					npc->Show(lMapIndex, x, y, z, true);
				}
			}
		}
	}
#endif