# arat: import uiPrivateShopBuilder
# altına ekle:
# BEGIN_OFFLINE_SHOP
import uiOfflineShopBuilder
import uiOfflineShop
# END_OF_OFFLINE_SHOP

# arat: uiPrivateShopBuilder.Clear()
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		uiOfflineShopBuilder.Clear()
		# END_OF_OFFLINE_SHOP

# arat: def	__PressJKey(self):
# değiştir:
	def	__PressJKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if player.IsMountingHorse():
				net.SendChatPacket("/unmount")
			# BEGIN_OFFLINE_SHOP
			elif not uiPrivateShopBuilder.IsBuildingPrivateShop() or not uiOfflineShopBuilder.IsBuildingOfflineShop():
			# END_OF_OFFLINE_SHOP
				if not uiPrivateShopBuilder.IsBuildingPrivateShop():
					for i in xrange(player.INVENTORY_PAGE_SIZE):
						if player.GetItemIndex(i) in (71114, 71116, 71118, 71120):
							net.SendItemUsePacket(i)
							break

# arat: def SetShopSellingPrice(self, Price):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def StartOfflineShop(self, vid):
		self.interface.OpenOfflineShopDialog(vid)

	def EndOfflineShop(self):
		self.interface.CloseOfflineShopDialog()

	def RefreshOfflineShop(self):
		self.interface.RefreshOfflineShopDialog()
	# END_OF_OFFLINE_SHOP

# arat: def __DropMoney(self, attachedType, attachedMoney):
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return

		if (uiOfflineShop.IsEditingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_OFFLINE_SHOP

# arat: def __DropItem(self, attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount):
#içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return

		if (uiOfflineShop.IsEditingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_OFFLINE_SHOP

# arat: def __SendDropItemPacket(self, itemVNum, itemCount, itemInvenType = player.INVENTORY):
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return

		if (uiOfflineShop.IsEditingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_OFFLINE_SHOP

# arat: "item_mall"				: self.__ItemMall_Open,
# altına ekle:
			# BEGIN_OFFLINE_SHOP
			"OpenOfflineShop"		: self.__OfflineShop_Open,
			"CloseOfflineShopBuilderWindow"	: self.__CloseOfflineShopBuilderWindow,
			# END_OF_OFFLINE_SHOP

# arat: def BINARY_PrivateShop_Disappear(self, vid):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def __OfflineShop_Open(self):
		self.interface.OpenOfflineShopInputNameDialog()

	def BINARY_OfflineShop_Appear(self, vid, text):
		if chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_NPC:
			self.interface.AppearOfflineShop(vid, text)

	def BINARY_OfflineShop_Disappear(self, vid):
		if chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_NPC:
			self.interface.DisappearOfflineShop(vid)

	def BINARY_OfflineShop_UpdateOnlinePlayerCount(self, playerCount):
		if self.interface:
			self.interface.wndMiniMap.UpdateOnlinePlayerCount(playerCount)

	def BINARY_OfflineShop_UpdateOnlineShopCount(self, shopCount):
		if self.interface:
			self.interface.wndMiniMap.UpdateOnlineShopCount(shopCount)

	def __CloseOfflineShopBuilderWindow(self):
		self.interface.CloseOfflineShopBuilder()
	# END_OF_OFFLINE_SHOP