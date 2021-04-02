# arat: def DeactivateSlot(self, slotNumber):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def ActivateOfflineShopSlot(self, slotNumber):
		wndMgr.ActivateOfflineShopSlot(self.hWnd, slotNumber)

	def DeactivateOfflineShopSlot(self, slotNumber):
		wndMgr.DeactivateOfflineShopSlot(self.hWnd, slotNumber)
	# END_OF_OFFLINE_SHOP