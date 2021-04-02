#Add imports:

import uiOfflineShopBuilder
import uiOfflineShop

#Search:

		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_PRIVATE_SHOP)
			return

###### May be more than one, make the same arrangement for all.######

#Add below:

		if uiOfflineShopBuilder.IsBuildingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_OFFLINE_SHOP)
			return

		if uiOfflineShop.IsEditingOfflineShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_ITEM_FAILURE_OFFLINE_SHOP)
			return