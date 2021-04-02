# arat: def __init__(self, *args, **kwargs):
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		self.isOfflineShopItem = False
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def ClearToolTip(self): (py dosyasında en yukardan aratırsanız ikincisi)
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		self.isOfflineShopItem = False
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def SetInventoryItem(self, slotIndex, window_type = player.INVENTORY):
# içerisinde değiştir: if not shop.IsPrivateShop():
			# BEGIN_OFFLINE_SHOP
			if not shop.IsPrivateShop() or not shop.IsOfflineShop():
			# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def SetInventoryItem(self, slotIndex, window_type = player.INVENTORY):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def SetOfflineShopBuilderItem(self, invenType, invenPos, offlineShopIndex):
		itemVnum = player.GetItemIndex(invenType, invenPos)
		if itemVnum == 0:
			return
		item.SelectItem(itemVnum)
		self.ClearToolTip()
		self.AppendSellingPrice(shop.GetOfflineShopItemPrice2(invenType, invenPos))
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(invenPos, i))

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(invenPos, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot)

	def SetOfflineShopItem(self, slotIndex):
		itemVnum = shop.GetOfflineShopItemID(slotIndex)
		if itemVnum == 0:
			return
		price = shop.GetOfflineShopItemPrice(slotIndex)
		self.ClearToolTip()
		self.isOfflineShopItem = True
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetOfflineShopItemMetinSocket(slotIndex, i))

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetOfflineShopItemAttribute(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot)
		self.AppendPrice(price)
		status, who = shop.GetOfflineShopItemStatus(slotIndex)
		if status:
			self.AppendItemBuyer(who)
	# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def GetPriceColor(self, price):
# üstüne ekle:
	# BEGIN_OFFLINE_SHOP
	def AppendItemBuyer(self, itemBuyer):
		self.AppendSpace(5)
		self.AppendTextLine('Sat\xfdn Alan: %s' % itemBuyer, self.DISABLE_COLOR)
	# END_OF_OFFLINE_SHOP