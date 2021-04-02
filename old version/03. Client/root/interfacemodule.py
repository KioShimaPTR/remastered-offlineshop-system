# arat: import uiPrivateShopBuilder
# altına ekle:
# BEGIN_OFFLINE_SHOP
import uiOfflineShop
import uiOfflineShopBuilder
# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: self.equipmentDialogDict = {}
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		self.offlineShopAdvertisementBoardDict = {}
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: self.dlgShop.Hide()
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		self.dlgOfflineShop = uiOfflineShop.OfflineShopDialog()
		self.dlgOfflineShop.LoadDialog()
		self.dlgOfflineShop.Hide()
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: self.privateShopBuilder.Hide()
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		self.offlineShopBuilder = uiOfflineShopBuilder.OfflineShopBuilder()
		self.offlineShopBuilder.Hide()
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: self.privateShopBuilder.SetItemToolTip(self.tooltipItem)
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		self.dlgOfflineShop.SetItemToolTip(self.tooltipItem)
		self.offlineShopBuilder.SetItemToolTip(self.tooltipItem)
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: if self.wndItemSelect:
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		if self.dlgOfflineShop:
			self.dlgOfflineShop.Destroy()

		if self.offlineShopBuilder:
			self.offlineShopBuilder.Destroy()
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: del self.wndItemSelect
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		del self.dlgOfflineShop
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: self.equipmentDialogDict = {}
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		self.offlineShopAdvertisementBoardDict = {}
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def RefreshShopDialog(self):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def OpenOfflineShopDialog(self, vid):
		self.wndInventory.Show()
		self.wndInventory.SetTop()
		self.dlgOfflineShop.Open(vid)
		self.dlgOfflineShop.SetTop()

	def CloseOfflineShopDialog(self):
		self.dlgOfflineShop.Close()

	def RefreshOfflineShopDialog(self):
		self.dlgOfflineShop.Refresh()
	# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def DisappearPrivateShop(self, vid):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def OpenOfflineShopInputNameDialog(self):
		inputDialog = uiOfflineShop.OfflineShopInputDialog()
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OpenOfflineShopBuilder))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.CloseOfflineShopInputNameDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog

	def CloseOfflineShopInputNameDialog(self):
		self.inputDialog = None
		return True

	def OpenOfflineShopBuilder(self):
		if not self.inputDialog:
			return True
		if not len(self.inputDialog.GetTitle()):
			return True
		if self.inputDialog.GetTime() < 0 or self.inputDialog.GetTime() == 0:
			return True
		self.offlineShopBuilder.Open(self.inputDialog.GetTitle(), self.inputDialog.GetTime())
		self.CloseOfflineShopInputNameDialog()
		return True

	def CloseOfflineShopBuilder(self):
		self.offlineShopBuilder.Close()

	def AppearOfflineShop(self, vid, text):
		board = uiOfflineShopBuilder.OfflineShopAdvertisementBoard()
		board.Open(vid, text)
		self.offlineShopAdvertisementBoardDict[vid] = board

	def DisappearOfflineShop(self, vid):
		if not self.offlineShopAdvertisementBoardDict.has_key(vid):
			return
		del self.offlineShopAdvertisementBoardDict[vid]
		uiOfflineShopBuilder.DeleteADBoard(vid)
	# END_OF_OFFLINE_SHOP