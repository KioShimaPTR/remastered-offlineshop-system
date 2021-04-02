// arat: else if (!strcmpi(szCmd, "ObserverCount"))
// altına ekle:
	// BEGIN_OFFLINE_SHOP
	else if (!strcmpi(szCmd, "CurrentPlayerOnline"))
	{
		if (2 != TokenVector.size())
		{
			TraceError("CPythonNetworkStream::ServerCommand(c_szCommand=%s) - Strange Parameter Count : %s", c_szCommand);
			return;
		}

		int uPlayerCount = atoi(TokenVector[1].c_str());

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], 
			"BINARY_OfflineShop_UpdateOnlinePlayerCount", 
			Py_BuildValue("(i)", uPlayerCount)
		);
	}
	else if (!strcmpi(szCmd, "CurrentShopOnline"))
	{
		if (2 != TokenVector.size())
		{
			TraceError("CPythonNetworkStream::ServerCommand(c_szCommand=%s) - Strange Parameter Count : %s", c_szCommand);
			return;
		}

		int uShopCount = atoi(TokenVector[1].c_str());

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], 
			"BINARY_OfflineShop_UpdateOnlineShopCount", 
			Py_BuildValue("(i)", uShopCount)
		);
	}
	// END_OF_OFFLINE_SHOP