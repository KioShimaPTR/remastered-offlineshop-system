# arat: import uiPrivateShopBuilder
# altına ekle:
# BEGIN_OFFLINE_SHOP
import uiOfflineShopBuilder
import uiOfflineShop
# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: self.costumeButton = self.GetChild2("CostumeButton")
# altına ekle:
			# BEGIN_OFFLINE_SHOP
			self.offlineShopButton = self.GetChild2("OfflineShopButton")
			# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: if self.costumeButton:
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		if self.offlineShopButton:
			self.offlineShopButton.SetEvent(ui.__mem_func__(self.ClickOfflineShopButton))
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def ClickCostumeButton(self):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def ClickOfflineShopButton(self):
		net.SendChatPacket('/open_offlineshop')
	# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: elif player.SLOT_TYPE_PRIVATE_SHOP == attachedSlotType:
# altına ekle:
			# BEGIN_OFFLINE_SHOP
			elif player.SLOT_TYPE_OFFLINE_SHOP == attachedSlotType:
				mouseModule.mouseController.RunCallBack('INVENTORY')
			# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def __SendUseItemToItemPacket(self, srcSlotPos, dstSlotPos):
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		if uiOfflineShopBuilder.IsBuildingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_OFFLINE_SHOP)
			return

		if uiOfflineShop.IsEditingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_OFFLINE_SHOP)
			return
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def __SendUseItemPacket(self, slotPos):
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		if uiOfflineShopBuilder.IsBuildingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_OFFLINE_SHOP)
			return

		if uiOfflineShop.IsEditingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_OFFLINE_SHOP)
			return
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def __SendMoveItemPacket(self, srcSlotPos, dstSlotPos, srcItemCount):
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		if uiOfflineShopBuilder.IsBuildingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_OFFLINE_SHOP)
			return

		if uiOfflineShop.IsEditingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_OFFLINE_SHOP)
			return
		# END_OF_OFFLINE_SHOP