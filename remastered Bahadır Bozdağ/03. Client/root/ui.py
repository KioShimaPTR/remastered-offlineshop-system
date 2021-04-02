#Search:

	def DeactivateSlot(self, slotNumber):
		wndMgr.DeactivateSlot(self.hWnd, slotNumber)

#Add below:

	def ActivateOfflineShopSlot(self, slotNumber):
		wndMgr.ActivateOfflineShopSlot(self.hWnd, slotNumber)

	def DeactivateOfflineShopSlot(self, slotNumber):
		wndMgr.DeactivateOfflineShopSlot(self.hWnd, slotNumber)