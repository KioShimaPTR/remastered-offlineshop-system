# arat: def Open(self, vid, name):
# direk altına ekle (içerisinde):
		# BEGIN_OFFLINE_SHOP
		# if chr.GetVirtualNumber(vid) == 30000 and chr.GetInstanceType(vid) != chr.INSTANCE_TYPE_PLAYER:
			# try:
				# name = str(name).rsplit("'", 1)[0] # rsplit is correct way but it's support 2.4+
			# except:
				# name = str(name).split("'")[0]
		# END_OF_OFFLINE_SHOP

# arat: if not player.IsSameEmpire(self.vid):
# altına ekle:
			# BEGIN_OFFLINE_SHOP
			# if chr.GetVirtualNumber(self.vid) == 30000 and chr.GetInstanceType(self.vid) != chr.INSTANCE_TYPE_PLAYER:
				# self.__HideButton(localeInfo.TARGET_BUTTON_EXCHANGE)
				# self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)
				# self.__HideButton(localeInfo.TARGET_BUTTON_EMOTION_ALLOW)
				# self.__HideButton(localeInfo.TARGET_BUTTON_FRIEND)
				# self.__HideButton(localeInfo.TARGET_BUTTON_INVITE_PARTY)
				# self.__HideButton(localeInfo.TARGET_BUTTON_INVITE_GUILD)
			# END_OF_OFFLINE_SHOP