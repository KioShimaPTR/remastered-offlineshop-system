//Search:

			case HEADER_GC_SHOP_SIGN:
				ret = RecvShopSignPacket();
				break;

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
			case HEADER_GC_OFFLINE_SHOP:
				ret = RecvOfflineShopPacket();
				break;

			case HEADER_GC_OFFLINE_SHOP_SIGN:
				ret = RecvOfflineShopSignPacket();
				break;
#endif

//Search:

bool CPythonNetworkStream::RecvTimePacket()

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
/////////////////////////////////////////////////////////////////////////
// MyOfflineShop
void CPythonNetworkStream::DisappearOfflineShopSign(DWORD dwVID)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OfflineShop_Disappear", Py_BuildValue("(i)", dwVID));
}

void CPythonNetworkStream::AppearOfflineShopSign(DWORD dwVID, std::string stSign)
{
	if (stSign.empty())
		for (auto it = m_mapOfflineShopSign.begin(); it != m_mapOfflineShopSign.end(); ++it)
			if (dwVID == it->first)
				stSign = it->second;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OfflineShop_Appear", Py_BuildValue("(is)", dwVID, stSign.c_str()));
}

bool CPythonNetworkStream::SendBuildOfflineShopPacket(const char * c_szName, const std::vector<TShopItemTable2> & c_rSellingItemStock, BYTE bTime)
{
	TPacketCGMyOfflineShop packet;
	packet.bHeader = HEADER_CG_MY_OFFLINE_SHOP;
	strncpy(packet.szSign, c_szName, SHOP_SIGN_MAX_LEN);
	packet.bCount = c_rSellingItemStock.size();
	packet.bTime = bTime;
	if (!Send(sizeof(packet), &packet))
		return false;

	for (std::vector<TShopItemTable2>::const_iterator itor = c_rSellingItemStock.begin(); itor < c_rSellingItemStock.end(); ++itor)
	{
		const TShopItemTable2 & c_rItem = *itor;
		if (!Send(sizeof(c_rItem), &c_rItem))
			return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvOfflineShopSignPacket()
{
	TPacketGCShopSign p;
	if (!Recv(sizeof(TPacketGCShopSign), &p))
		return false;

	if (0 == strlen(p.szSign))
		DisappearOfflineShopSign(p.dwVID);
	else
	{
		//PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OfflineShop_Appear", Py_BuildValue("(is)", p.dwVID, p.szSign));
		CInstanceBase * pInstance = CPythonCharacterManager::instance().GetInstancePtr(p.dwVID);
		if (pInstance && (pInstance->IsPC() || pInstance->GetRace() == 30000))
		{
			if (!m_mapOfflineShopSign.empty() && m_mapOfflineShopSign.find(p.dwVID) != m_mapOfflineShopSign.end())
				m_mapOfflineShopSign.erase(p.dwVID);
			m_mapOfflineShopSign[p.dwVID] = p.szSign;
			AppearOfflineShopSign(p.dwVID, p.szSign);
		}
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////

bool CPythonNetworkStream::RecvOfflineShopPacket()
{
	std::vector<char> vecBuffer;
	vecBuffer.clear();

	TPacketGCShop  packet_shop;
	if (!Recv(sizeof(packet_shop), &packet_shop))
		return false;

	int iSize = packet_shop.size - sizeof(packet_shop);
	if (iSize > 0)
	{
		vecBuffer.resize(iSize);
		if (!Recv(iSize, &vecBuffer[0]))
			return false;
	}

	switch (packet_shop.subheader)
	{
		case SHOP_SUBHEADER_GC_START:
		{
			CPythonShop::Instance().Clear();

			DWORD dwVID = *(DWORD *)&vecBuffer[0];

			TPacketGCOfflineShopStart * pShopStartPacket = (TPacketGCOfflineShopStart *)&vecBuffer[4];
			for (BYTE iItemIndex = 0; iItemIndex < OFFLINE_SHOP_HOST_ITEM_MAX_NUM; ++iItemIndex)
			{
				CPythonShop::Instance().SetOfflineShopItemData(iItemIndex, pShopStartPacket->items[iItemIndex]);
			}

			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartOfflineShop", Py_BuildValue("(ii)", dwVID, pShopStartPacket->looked));
		}
		break;

		case SHOP_SUBHEADER_GC_END:
			CPythonShop::instance().Clear();
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "EndOfflineShop", Py_BuildValue("()"));
			break;

		case SHOP_SUBHEADER_GC_UPDATE_ITEM:
		{
			TPacketGCShopUpdateOfflineShopItem * pShopUpdateItemPacket = (TPacketGCShopUpdateOfflineShopItem *)&vecBuffer[0];
			CPythonShop::Instance().SetOfflineShopItemData(pShopUpdateItemPacket->pos, pShopUpdateItemPacket->item);
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshOfflineShop", Py_BuildValue("()"));
		}
		break;

		case SHOP_SUBHEADER_GC_UPDATE_ITEM2:
		{
			CPythonShop::Instance().Clear();

			TPacketGCOfflineShopStart * pShopStartPacket = (TPacketGCOfflineShopStart *)&vecBuffer[4];

			for (BYTE iItemIndex = 0; iItemIndex < OFFLINE_SHOP_HOST_ITEM_MAX_NUM; ++iItemIndex)
			{
				CPythonShop::Instance().SetOfflineShopItemData(iItemIndex, pShopStartPacket->items[iItemIndex]);
			}
		}
		break;

		case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY"));
			break;

		case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY_EX"));
			break;

		case SHOP_SUBHEADER_GC_NOT_ENOUGH_BAR_1:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_BAR_1"));
			break;
			
		case SHOP_SUBHEADER_GC_NOT_ENOUGH_BAR_2:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_BAR_2"));
			break;
			
		case SHOP_SUBHEADER_GC_NOT_ENOUGH_BAR_3:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_BAR_3"));
			break;

		case SHOP_SUBHEADER_GC_SOLD_OUT:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "SOLDOUT"));
			break;

		case SHOP_SUBHEADER_GC_INVENTORY_FULL:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVENTORY_FULL"));
			break;

		case SHOP_SUBHEADER_GC_INVALID_POS:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVALID_POS"));
			break;

		case SHOP_SUBHEADER_GC_REFRESH_PRICE:
		{
			TPacketGCOfflineShopPrice * pOfflineShopMoney = (TPacketGCOfflineShopPrice *)&vecBuffer[0];
			CPythonPlayer::Instance().SetCurrentOfflineShopMoney(pOfflineShopMoney->llMoney);
			CPythonPlayer::Instance().SetCurrentOfflineShopBar1(pOfflineShopMoney->dwBar1);
			CPythonPlayer::Instance().SetCurrentOfflineShopBar2(pOfflineShopMoney->dwBar2);
			CPythonPlayer::Instance().SetCurrentOfflineShopBar3(pOfflineShopMoney->dwBar3);
			break;
		}

		default:
			TraceError("CPythonNetworkStream::RecvOfflineShopPacket: Unknown subheader %d\n", packet_shop.subheader);
			break;
	}

	return true;
}
#endif