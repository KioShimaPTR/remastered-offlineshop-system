# arat: self.AtlasShowButton = 0
# altına ekle:
		# BEGIN_OFFLINE_SHOP
		self.playerCount = 0
		self.shopCount = 0
		# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: self.serverInfo = self.GetChild("ServerInfo")
# altına ekle:
			# BEGIN_OFFLINE_SHOP
			self.playerCount = self.GetChild("PlayerCount")
			self.shopCount = self.GetChild("ShopCount")
			# END_OF_OFFLINE_SHOP
############################################
############################################
# arat: def ToggleAtlasWindow(self):
# altına ekle:
	# BEGIN_OFFLINE_SHOP
	def UpdateOnlinePlayerCount(self, playerCount):
		self.playerCount.SetText(localeInfo.PrettyNumber(playerCount) + " Online Oyuncu")

	def UpdateOnlineShopCount(self, shopCount):
		self.shopCount.SetText(localeInfo.PrettyNumber(shopCount) + " Online Tezg\xe2h")
	# END_OF_OFFLINE_SHOP