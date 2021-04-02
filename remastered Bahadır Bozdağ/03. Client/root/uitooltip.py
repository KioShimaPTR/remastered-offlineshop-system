#Search:

		self.isShopItem = False

#Add below:

		self.isOfflineShopItem = False

#Search:

			if not shop.IsPrivateShop():

#Change:

			if not shop.IsPrivateShop() or not shop.IsOfflineShop():

#Search:

	def SetShopItem(self, slotIndex):

#Add on:

	def SetOfflineShopBuilderItem(self, invenType, invenPos, offlineShopIndex):
		itemVnum = player.GetItemIndex(invenType, invenPos)
		if itemVnum == 0:
			return
		item.SelectItem(itemVnum)
		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(invenPos, i))

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(invenPos, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, 0, player.GetItemEvolution(invenType, invenPos), invenType, invenPos, systemSetting.IsPreviewModel())

		if shop.GetOfflineShopItemPrice2Real(invenType, invenPos) > 0:
			self.AppendSellingPrice2New(shop.GetOfflineShopItemPrice2Real(invenType, invenPos), shop.GetOfflineShopItemPriceTypeReal(invenType, invenPos))
		if shop.GetOfflineShopItemPriceReal(invenType, invenPos) > 0:
			self.AppendSellingPriceNew(shop.GetOfflineShopItemPriceReal(invenType, invenPos))

	def SetOfflineShopItem(self, slotIndex):
		itemVnum = shop.GetOfflineShopItemID(slotIndex)
		if itemVnum == 0:
			return
		price = shop.GetOfflineShopItemPrice(slotIndex)
		price2 = shop.GetOfflineShopItemPrice2(slotIndex)
		price_type = shop.GetOfflineShopItemPriceType(slotIndex)
		self.ClearToolTip()
		self.isOfflineShopItem = True
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetOfflineShopItemMetinSocket(slotIndex, i))

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetOfflineShopItemAttribute(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, 0, shop.GetOfflineShopItemEvolution(slotIndex), player.INVENTORY, -1, systemSetting.IsPreviewModel())
		if price2 > 0:
			self.AppendPrice2(price2, price_type)
		if price > 0:
			self.AppendPrice(price)
		status, who = shop.GetOfflineShopItemStatus(slotIndex)
		if status:
			self.AppendItemBuyer(who)

