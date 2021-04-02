# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: uiofflineshopbuilder.py

import ui
import snd
import shop
import mouseModule
import player
import chr
import net
import uiCommon
import localeInfo
import chat
import systemSetting
import item
import app
import constInfo

g_isBuildingOfflineShop = False
g_itemPriceDict = {}
g_offlineShopAdvertisementBoardDict = {}

def Clear():
	global g_isBuildingOfflineShop
	global g_itemPriceDict
	global g_offlineShopAdvertisementBoardDict
	g_itemPriceDict = {}
	g_isBuildingOfflineShop = False
	g_offlineShopAdvertisementBoardDict = {}

def IsOfflineShopItemPriceList():
	if g_itemPriceDict:
		return True
	else:
		return False

def IsBuildingOfflineShop():
	if g_isBuildingOfflineShop:
		return True
	else:
		return False

def SetOfflineShopItemPrice(itemVnum, itemPrice):
	g_itemPriceDict[int(itemVnum)] = itemPrice

def GetOfflineShopItemPrice(itemVnum):
	try:
		return g_itemPriceDict[itemVnum]
	except KeyError:
		return 0

def UpdateADBoard():
	for key in g_offlineShopAdvertisementBoardDict.keys():
		g_offlineShopAdvertisementBoardDict[key].Show()


def DeleteADBoard(vid):
	if not g_offlineShopAdvertisementBoardDict.has_key(vid):
		return
	del g_offlineShopAdvertisementBoardDict[vid]

class OfflineShopAdvertisementBoard(ui.ThinBoard):

	def __init__(self):
		ui.ThinBoard.__init__(self, 'UI_BOTTOM')
		self.vid = None
		self.offlineShopAdvertismentBoardSeen = []
		self.__MakeTextLine()
		return

	def __del__(self):
		ui.ThinBoard.__del__(self)

	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.Show()

	def Open(self, vid, text):
		self.vid = vid
		self.textLine.SetText(text)
		if vid in self.offlineShopAdvertismentBoardSeen:
			self.textLine.SetFontColor(1.0, 0.5, 0.1)
		self.textLine.UpdateRect()
		self.SetSize(len(text) * 6 + 20, 20)
		self.Show()
		g_offlineShopAdvertisementBoardDict[vid] = self

	def OnMouseLeftButtonUp(self):
		if not self.vid:
			return
		if self.vid != player.GetMainCharacterIndex():
			self.textLine.SetFontColor(253, 300, 5)
			self.offlineShopAdvertismentBoardSeen.append(self.vid)
		net.SendOnClickPacket(self.vid)
		return True

	def OnUpdate(self):
		if (not self.vid):
			return
		
		if (systemSetting.IsShowSalesText() and systemSetting.IsShowOfflineShop()):
			if not app.ENABLE_SHOPNAMES_RANGE:
				self.Show()
				(x, y) = chr.GetProjectPosition(self.vid, 220)
				self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
			else:
				if systemSetting.GetShopNamesRange() == 1.000:
					self.Show()
					(x, y) = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
				else:
					LIMIT_RANGE = abs(constInfo.SHOPNAMES_RANGE * systemSetting.GetShopNamesRange())
					(to_x, to_y, to_z) = chr.GetPixelPosition(self.vid)
					(my_x, my_y, my_z) = player.GetMainCharacterPosition()
					if abs(my_x - to_x) <= LIMIT_RANGE and abs(my_y - to_y) <= LIMIT_RANGE:
						(x, y) = chr.GetProjectPosition(self.vid, 220)
						self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
						self.Show()
					else:
						self.Hide()
						self.SetPosition(-70, 0)
		else:
			for key in g_offlineShopAdvertisementBoardDict.keys():
				g_offlineShopAdvertisementBoardDict[key].Hide()

class OfflineShopBuilder(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.itemStock = {}
		self.tooltipItem = None
		self.priceInputBoard = None
		self.title = ''
		self.time = -1
		self.lastPrice = 0
		return

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, 'UIScript/OfflineShopBuilder.py')
		except:
			import exception
			exception.Abort('OfflineShopBuilderWindow.LoadWindow.LoadObject')

		try:
			self.itemSlot = self.GetChild('ItemSlot')
			self.btnOk = self.GetChild('OkButton')
			self.btnClose = self.GetChild('CloseButton')
			self.board = self.GetChild('Board')
		except:
			import exception
			exception.Abort('OfflineShopBuilderWindow.LoadWindow.BindObject')

		self.btnOk.SetEvent(ui.__mem_func__(self.OnOk))
		self.btnClose.SetEvent(ui.__mem_func__(self.OnClose))
		self.board.SetCloseEvent(ui.__mem_func__(self.OnClose))
		self.itemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.itemSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))

	def Destroy(self):
		self.ClearDictionary()
		self.itemSlot = None
		self.btnOk = None
		self.btnClose = None
		self.board = None
		self.priceInputBoard = None
		return

	def Open(self, title, time):
		global g_isBuildingOfflineShop
		self.title = title
		self.time = time
		self.itemStock = {}
		shop.ClearOfflineShopStock()
		self.SetCenterPosition()
		self.SetTop()
		self.Refresh()
		self.Show()
		g_isBuildingOfflineShop = True

	def Close(self):
		global g_isBuildingOfflineShop
		g_isBuildingOfflineShop = False
		self.title = ''
		self.time = -1
		shop.ClearOfflineShopStock()
		self.Hide()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def Refresh(self):
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			if not self.itemStock.has_key(i):
				self.itemSlot.ClearSlot(i)
				continue
			pos = self.itemStock[i]
			itemCount = player.GetItemCount(*pos)
			if itemCount <= 1:
				itemCount = 0
			self.itemSlot.SetItemSlot(i, player.GetItemIndex(*pos), itemCount)

		self.itemSlot.RefreshSlot()

	def OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType and player.SLOT_TYPE_UPGRADE_INVENTORY != attachedSlotType and player.SLOT_TYPE_BOOK_INVENTORY != attachedSlotType and player.SLOT_TYPE_STONE_INVENTORY != attachedSlotType:
					return
			else:
				if player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType:
					return

			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)
	
			if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PRIVATE_SHOP_CANNOT_SELL_ITEM)
				return

			priceInputBoard = uiCommon.MoneyInputDialog2()
			priceInputBoard.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_PRICE_DIALOG_TITLE)
			priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
			priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
			priceInputBoard.Open()
			itemPrice = GetOfflineShopItemPrice(itemVNum)

			if itemPrice > 0:
				priceInputBoard.SetValue(itemPrice)

			priceInputBoard.SetValue(self.lastPrice)
			self.priceInputBoard = priceInputBoard
			self.priceInputBoard.itemVNum = itemVNum
			self.priceInputBoard.sourceWindowType = attachedInvenType
			self.priceInputBoard.sourceSlotPos = attachedSlotPos
			self.priceInputBoard.targetSlotPos = selectedSlotPos

	def OnSelectItemSlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			snd.PlaySound('sound/ui/loginfail.wav')
			mouseModule.mouseController.DeattachObject()
		else:
			if selectedSlotPos not in self.itemStock:
				return
	
			invenType, invenPos = self.itemStock[selectedSlotPos]
			shop.DelOfflineShopItemStock(invenType, invenPos)
			snd.PlaySound('sound/ui/drop.wav')
			del self.itemStock[selectedSlotPos]
			self.Refresh()

	def AcceptInputPrice(self):
		if not self.priceInputBoard:
			return True
		else:
			text = self.priceInputBoard.GetText()
			text2 = self.priceInputBoard.GetText2()
			text2type = self.priceInputBoard.GetText2Type()
			if not text:
				return True

			if not text.isdigit():
				return True

			if int(text) <= 0:
				return True

			if text2 and int(text2) >= 200:
				chat.AppendChat(1, localeInfo.OFFLINE_SHOP_MAXCOUNT_200)
				return True

			itvnm = self.priceInputBoard.itemVNum
			item.SelectItem(itvnm)
			itemname = item.GetItemName()
			if int(text2type) == 1:
				if int(itvnm) == 80005:
					self.uyarim(itemname)
					return True
			elif int(text2type) == 2:
				if int(itvnm) == 80006:
					self.uyarim(itemname)
					return True
			elif int(text2type) == 3:
				if int(itvnm) == 80007:
					self.uyarim(itemname)
					return True

			attachedInvenType = self.priceInputBoard.sourceWindowType
			sourceSlotPos = self.priceInputBoard.sourceSlotPos
			targetSlotPos = self.priceInputBoard.targetSlotPos
			for privatePos, (itemWindowType, itemSlotIndex) in self.itemStock.items():
				if itemWindowType == attachedInvenType and itemSlotIndex == sourceSlotPos:
					shop.DelOfflineShopItemStock(itemWindowType, itemSlotIndex)
					del self.itemStock[privatePos]

			price = int(self.priceInputBoard.GetText())
			price2 = int(self.priceInputBoard.GetText2())
			price2type = int(self.priceInputBoard.GetText2Type())

			if IsOfflineShopItemPriceList():
				SetOfflineShopItemPrice(self.priceInputBoard.itemVNum, price)

			shop.AddOfflineShopItemStock(attachedInvenType, sourceSlotPos, targetSlotPos, price, price2, price2type)
			self.itemStock[targetSlotPos] = (attachedInvenType, sourceSlotPos)
			snd.PlaySound('sound/ui/drop.wav')
			self.lastPrice = price
			self.Refresh()
			self.priceInputBoard.Close()
			self.priceInputBoard = None
			return True

	def CancelInputPrice(self):
		if self.priceInputBoard:
			self.priceInputBoard.Close()
		self.priceInputBoard = None
		return True

	def uyarim(self, itemad):
		# chat.AppendChat(1, translate.uyarim %(str(itemad),str(itemad)))
		# self.priceInputBoard.Close()
		self.uyari2 = uiCommon.PopupDialog()
		self.uyari2.SetText("s ile %s satamazsÄ±n." %(str(itemad),str(itemad)))
		self.uyari2.Open()

	def OnOk(self):
		if not self.title:
			chat.AppendChat(chat.CHAT_TYPE_INFO, 'Market ba\xfel\xfd\xf0\xfd belirtilmedi!')
			return

		if len(self.itemStock) == 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, 'Sat\xfdlacak hi\xe7 bir nesne koyulmad\xfd!')
			return

		if self.time < 0 or self.time == 0:
			chat.AppendChat(chat.CHAT_TYPE_INFO, 'Hatal\xfd Market Tezgah\xfd s\xfcresi!')
			return

		shop.BuildOfflineShop(self.title, self.time)
		self.Close()

	def OnClose(self):
		self.Close()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnOverInItem(self, slotIndex):
		if self.tooltipItem:
			if self.itemStock.has_key(slotIndex):
				self.tooltipItem.SetOfflineShopBuilderItem(*(self.itemStock[slotIndex] + (slotIndex,)))

	def OnOverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
