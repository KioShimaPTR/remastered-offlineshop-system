# arat: import uiPrivateShopBuilder
# altına ekle:
# BEGIN_OFFLINE_SHOP
import uiOfflineShopBuilder
# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: if player.SLOT_TYPE_PRIVATE_SHOP == attachedSlotType:
# altına ekle:
			# BEGIN_OFFLINE_SHOP
			elif (player.SLOT_TYPE_OFFLINE_SHOP == attachedSlotType):
				mouseModule.mouseController.RunCallBack("INVENTORY")
			# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def UseItemSlot(self, slotIndex):
# içerisine ekle:
				# BEGIN_OFFLINE_SHOP
				if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
					return
				# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def __SendMoveItemPacket(self, srcSlotWindow, srcSlotPos, dstSlotWindow, dstSlotPos, srcItemCount):
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def __UseEquipItem(self, slotIndex):
# içerisine ekle:
		# BEGIN_OFFLINE_SHOP
		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def __SelectRefineEmptySlot(self, selectedSlotPos):
# içerisine ekle:
				# BEGIN_OFFLINE_SHOP
				if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
					return
				# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def __SelectRefineItemSlot(self, selectedSlotPos):
# içerisine ekle:
					# BEGIN_OFFLINE_SHOP
					if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_ITEM_FAILURE_PRIVATE_SHOP)
						return
					# END_OF_OFFLINE_SHOP