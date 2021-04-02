//Search:

bool CPythonNetworkStream::SendItemUsePacket(TItemPos pos)

//Add on:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
bool CPythonNetworkStream::SendOfflineShopEndPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop packet_shop;
	packet_shop.header = HEADER_CG_OFFLINE_SHOP;
	packet_shop.subheader = SHOP_SUBHEADER_CG_END;

	if (!Send(sizeof(packet_shop), &packet_shop))
	{
		Tracef("SendOfflineShopEndPacket Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendOfflineShopBuyPacket(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_BUY;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendOfflineShopBuyPacket Error\n");
		return false;
	}

	BYTE bCount = 1;
	if (!Send(sizeof(BYTE), &bCount))
	{
		Tracef("SendOfflineShopBuyPacket Error\n");
		return false;
	}

	if (!Send(sizeof(BYTE), &bPos))
	{
		Tracef("SendOfflineShopBuyPacket Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendDestroyOfflineShop()
{
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_DESTROY_OFFLINE_SHOP;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendDestroyOfflineShop Packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendUnsoldItems()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	CInstanceBase* pkInstMain=rkChrMgr.GetMainInstancePtr();
	if (pkInstMain->IsOfflineShop())
		return true;

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_UNSOLD_ITEMS;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendUnsoldItems Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendRefreshOfflineShopPrice()
{
	static DWORD s_LastTime = timeGetTime() - 5001;

	if (timeGetTime() - s_LastTime < 5000)
		return true;

	s_LastTime = timeGetTime();

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_REFRESH_PRICE;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendRefreshOfflineShopPrice Packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendOfflineShopWithdrawMoney(DWORD dwMoney)
{
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_WITHDRAW_MONEY;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendOfflineShopWithdrawMoney Packet Error\n");
		return false;
	}

	if (!Send(sizeof(dwMoney), &dwMoney))
	{
		Tracef("SendOfflineShopWithdrawMoney Packet Error \n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendOfflineShopWithdrawBar1(WORD wBar1)
{
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_WITHDRAW_BAR_1;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendOfflineShopWithdrawBar1 Packet Error\n");
		return false;
	}

	if (!Send(sizeof(wBar1), &wBar1))
	{
		Tracef("SendOfflineShopWithdrawBar1 Packet Error \n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendOfflineShopWithdrawBar2(WORD wBar2)
{
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_WITHDRAW_BAR_2;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendOfflineShopWithdrawBar2 Packet Error\n");
		return false;
	}

	if (!Send(sizeof(wBar2), &wBar2))
	{
		Tracef("SendOfflineShopWithdrawBar2 Packet Error \n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendOfflineShopWithdrawBar3(WORD wBar3)
{
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_WITHDRAW_BAR_3;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendOfflineShopWithdrawBar3 Packet Error\n");
		return false;
	}

	if (!Send(sizeof(wBar3), &wBar3))
	{
		Tracef("SendOfflineShopWithdrawBar3 Packet Error \n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendOfflineShopFetchItem(BYTE bDisplayPos)
{
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_FETCH_ITEM;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendOfflineShopTakeItem Packet error\n");
		return false;
	}

	if (!Send(sizeof(bDisplayPos), &bDisplayPos))
	{
		Tracef("SendOfflineShopTakeItem Packet error\n");
		return false;
	}
	
	return SendSequence();
}

bool CPythonNetworkStream::SendOfflineShopTeleport()
{	
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_OFFLINE_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_TELEPORT;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendRefreshUnsoldItems Error\n");
		return false;
	}

	return SendSequence();
}
#endif