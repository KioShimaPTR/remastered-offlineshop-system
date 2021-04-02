#Add imports

import uiOfflineShopBuilder
import uiOfflineShop

#Search:

		uiPrivateShopBuilder.Clear()

#Add below:

		uiOfflineShopBuilder.Clear()

#Search:

				if not uiPrivateShopBuilder.IsBuildingPrivateShop():

#Change:

				if not uiPrivateShopBuilder.IsBuildingPrivateShop() or not uiOfflineShopBuilder.IsBuildingOfflineShop():

#Search:

	def SetShopSellingPrice(self, Price):
		pass

#Add below:

	def StartOfflineShop(self, vid, looked):
		if looked:
			self.interface.OpenOfflineShopDialog(vid, looked)
		else:
			self.interface.OpenOfflineShopDialog(vid, 0)

		background.DeletePrivateShopPos(vid)

	def EndOfflineShop(self):
		self.interface.CloseOfflineShopDialog()

	def RefreshOfflineShop(self):
		self.interface.RefreshOfflineShopDialog()

#Search:

		if uiPrivateShopBuilder.IsBuildingPrivateShop():			
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return

###### May be more than one, make the same arrangement for all.######

#Add below:

		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_OFFLINE_SHOP)
			return

		if (uiOfflineShop.IsEditingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_OFFLINE_SHOP)
			return

#Search:

			"OpenPrivateShop"		: self.__PrivateShop_Open,

#Add below:

			"OpenOfflineShop"		: self.__OfflineShop_Open,
			"CloseOfflineShopBuilderWindow"	: self.__CloseOfflineShopBuilderWindow,

#Search

	def BINARY_PrivateShop_Disappear(self, vid):
		self.interface.DisappearPrivateShop(vid)

#Add below:

	def __OfflineShop_Open(self):
		self.interface.OpenOfflineShopInputNameDialog()

	def BINARY_OfflineShop_Appear(self, vid, text):
		if chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_NPC:
			self.interface.AppearOfflineShop(vid, text)

	def BINARY_OfflineShop_Disappear(self, vid):
		if chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_NPC:
			self.interface.DisappearOfflineShop(vid)

	def __CloseOfflineShopBuilderWindow(self):
		self.interface.CloseOfflineShopBuilder()