#Add imports

import uiOfflineShop
import uiOfflineShopBuilder

#Search:

		self.equipmentDialogDict = {}

#Add below:

		self.offlineShopAdvertisementBoardDict = {}

#Search:

		self.dlgShop.Hide()

#Add below:

		self.dlgOfflineShop = uiOfflineShop.OfflineShopDialog()
		self.dlgOfflineShop.LoadDialog()
		self.dlgOfflineShop.Hide()

#Search:

		self.privateShopBuilder.Hide()

#Add below:

		self.offlineShopBuilder = uiOfflineShopBuilder.OfflineShopBuilder()
		self.offlineShopBuilder.Hide()

#Search:

		self.privateShopBuilder.SetItemToolTip(self.tooltipItem)

#Add below:

		self.dlgOfflineShop.SetItemToolTip(self.tooltipItem)
		self.offlineShopBuilder.SetItemToolTip(self.tooltipItem)

#Search:

			self.wndItemSelect.Destroy()

#Add below:

		if self.dlgOfflineShop:
			self.dlgOfflineShop.Destroy()

		if self.offlineShopBuilder:
			self.offlineShopBuilder.Destroy()

#Search:
		del self.wndItemSelect

#Add below:

		del self.dlgOfflineShop

#Search:

		self.equipmentDialogDict = {}

#Add below:

		self.offlineShopAdvertisementBoardDict = {}

#Search:

	def RefreshShopDialog(self):
		self.dlgShop.Refresh()

#Add below:

	def OpenOfflineShopDialog(self, vid, looked):
		self.wndInventory.Show()
		self.wndInventory.SetTop()
		self.dlgOfflineShop.Open(vid, looked)
		self.dlgOfflineShop.SetTop()
		
	def CloseOfflineShopDialog(self):	
		self.dlgOfflineShop.Close()
		
	def RefreshOfflineShopDialog(self):
		self.dlgOfflineShop.Refresh()

#Search:

	def DisappearPrivateShop(self, vid):

		if not self.privateShopAdvertisementBoardDict.has_key(vid):
			return

		del self.privateShopAdvertisementBoardDict[vid]
		uiPrivateShopBuilder.DeleteADBoard(vid)

#Add below:

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