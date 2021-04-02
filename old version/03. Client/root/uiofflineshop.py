import app
import ui
import uiCommon
import uiToolTip
import mouseModule
import localeInfo
import constInfo
import net
import player
import item
import snd
import shop
import wndMgr
import chat
import chr

g_isEditingOfflineShop = False

def IsEditingOfflineShop():
	global g_isEditingOfflineShop
	if g_isEditingOfflineShop:
		return True
	else:
		return False

class TextBoard(ui.ThinBoard):

	def __init__(self):
		ui.ThinBoard.__init__(self)
		self.lineHeight = 12
		self.childrenList = []

	def __del__(self):
		ui.ThinBoard.__del__(self)

	def Clear(self):
		self.lineHeight = 12
		self.childrenList = []

	def AppendTextLine(self, text):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
		textLine.SetText(str(text))
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()
		textLine.SetPosition(10, self.lineHeight)
		self.childrenList.append(textLine)
		self.lineHeight += 17
		return textLine

class OfflineShopInputDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.hour = 10
		self.pos = 0
		self.priceInputBoard = None
		self.LoadWindow()
		return

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, 'UIScript/OfflineShopInputDialog.py')
		except:
			import exception
			exception.Abort('OfflineShopInputDialog.LoadWindow.LoadObject')

		try:
			self.titleBar = self.GetChild('TitleBar')
			self.offlineShopTime = self.GetChild('OfflineShopTime')
			self.bankButton = self.GetChild('BankButton')
			self.retrieveItemButton = self.GetChild('RetrieveItems')
			self.closeOfflineShopButton = self.GetChild('CloseOfflineShop')
			self.acceptButton = self.GetChild('AgreeButton')
			self.inputValue = self.GetChild('InputValue')
			self.timeInputValue = self.GetChild('TimeInputValue')
			self.itemSlot = self.GetChild('ItemSlot')
			self.watchButton = self.GetChild('WatchButton')
		except:
			import exception
			exception.Abort('OfflineShopInputDialog.LoadWindow.BindObject')

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.offlineShopTime.SetSliderPos(1.0)
		self.offlineShopTime.SetEvent(ui.__mem_func__(self.OnChangeOfflineShopTime))
		self.bankButton.SetEvent(ui.__mem_func__(self.ClickMyBankButton))
		self.retrieveItemButton.SetEvent(ui.__mem_func__(self.ClickRetrieveItem))
		self.closeOfflineShopButton.SetEvent(ui.__mem_func__(self.ClickCloseOfflineShopButton))
		self.itemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.watchButton.SetEvent(ui.__mem_func__(self.OnClickWatchButton))
		self.textBoard = TextBoard()
		self.textBoard.SetParent(self)
		self.textBoard.SetPosition(11, 32)
		self.textBoard.SetSize(528, 245)
		self.textBoard.AddFlag('not_pick')
		self.textBoard.Show()
		self.RefreshText()

	def SplitDescription(self, desc, limit):
		total_tokens = desc.split()
		line_tokens = []
		line_len = 0
		lines = []
		for token in total_tokens:
			if '|' in token:
				sep_pos = token.find('|')
				line_tokens.append(token[:sep_pos])
				lines.append(' '.join(line_tokens))
				line_len = len(token) - (sep_pos + 1)
				line_tokens = [token[sep_pos + 1:]]
			else:
				line_len += len(token)
				if len(line_tokens) + line_len > limit:
					lines.append(' '.join(line_tokens))
					line_len = len(token)
					line_tokens = [token]
				else:
					line_tokens.append(token)

		if line_tokens:
			lines.append(' '.join(line_tokens))
		return lines

	def RefreshText(self):
		self.textBoard.Clear()
		lines = self.SplitDescription('Market Tezg\xe2h\xfd sistemi sen oyunda olmasan da senin belirledi\xf0in s\xfcre boyunca nesnelerinin ticaretinin yap\xfdlmas\xfdn\xfd sa\xf0lar. Bu sistem sadece CH1 ve kendi birinci k\xf6y\xfcnde kullan\xfdlabilir. Oyundan \xe7\xfdkman tezg\xe2h\xfdn\xfd kapatmaz. Market Tezg\xe2h\xfdn\xfd kurabilmek i\xe7in en az 15. seviye ve 10K yang gerekir. Diledi\xf0in zaman Yang \xc7ek butonuna t\xfdklay\xfdp sat\xfdlan nesnelerinin paras\xfdn\xfd alabilirsin. Nesneleri Getir butonu tezg\xe2h\xfdn otomatik olarak kapand\xfd\xf0\xfd vakit giden itemlerini Market Tezg\xe2h\xfd depondan geri \xe7ekmek i\xe7in kullan\xfdl\xfdr. Market Tezg\xe2h\xfdn\xfd kurduktan sonra e\xf0er ekstra olarak nesne eklemek istersen, envanterinden itemi a\xfea\xf0\xfddaki bo\xfe slota s\xfcr\xfckleyip b\xfdrak ve fiyat\xfdn\xfd gir, sistem Market Tezg\xe2h\xfdnda en yak\xfdn bo\xfe yeri otomatik olarak bulup nesneni senin i\xe7in sat\xfd\xfea \xe7\xfdkaracakt\xfdr, unutma bu i\xfelemi yapabilmek i\xe7in Market Tezg\xe2h\xfdn\xfdn yan\xfdnda olman gerekiyor. Tezg\xe2ha Bak butonu ile kanal ya da harita fark etmeksizin Market Tezg\xe2h\xfdn\xfdn son durumunu g\xf6rebilirsin. [Yap\xfdm A\xfeamas\xfdnda] [A\xe7\xfdklama daha fazla geni\xfeletilecek.]', 94)
		if not lines:
			return
		for line in lines:
			self.textBoard.AppendTextLine(line)

	def OnSelectEmptySlot(self, selectedSlotPos):
		chat.AppendChat(chat.CHAT_TYPE_INFO, '<Market Tezg\xe2h\xfd> Bu \xf6zellik art\xfdk kullan\xfdlmamaktad\xfdr!')

	def OnClickWatchButton(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, '<Market Tezg\xe2h\xfd> Bu \xf6zellik hen\xfcz yap\xfdm a\xfeamas\xfdnda!')

	def ClickCloseOfflineShopButton(self):
		self.Close()
		closeQuestionDialog = uiCommon.QuestionDialog()
		closeQuestionDialog.SetText('Market Tezgah\xfdn\xfd kapatmak istiyor musun?')
		closeQuestionDialog.SetAcceptEvent(lambda arg = True: self.AnswerCloseOfflineShop(arg))
		closeQuestionDialog.SetCancelEvent(lambda arg = False: self.AnswerCloseOfflineShop(arg))
		closeQuestionDialog.Open()
		self.closeQuestionDialog = closeQuestionDialog

	def AnswerCloseOfflineShop(self, flag):
		if flag:
			net.SendDestroyOfflineShop()
			shop.ClearOfflineShopStock()
		else:
			self.Show()
		self.closeQuestionDialog = None
		return

	def ClickRetrieveItem(self):
		self.Close()
		net.SendChatPacket('/retrieve_offline_shop_item')

	def ClickMyBankButton(self):
		self.Close()
		withdrawQuestionDialog = uiCommon.QuestionDialog()
		withdrawQuestionDialog.SetText('Hesab\xfdndaki t\xfcm paray\xfd \xe7ekmek istiyor musun?')
		withdrawQuestionDialog.SetAcceptEvent(lambda arg = True: self.AnswerMyBankQuestion(arg))
		withdrawQuestionDialog.SetCancelEvent(lambda arg = False: self.AnswerMyBankQuestion(arg))
		withdrawQuestionDialog.Open()
		self.withdrawQuestionDialog = withdrawQuestionDialog

	def AnswerMyBankQuestion(self, flag):
		if flag:
			net.SendChatPacket('/withdraw_offline_shop_money')
		self.withdrawQuestionDialog = None
		return True

	def OnChangeOfflineShopTime(self):
		self.pos = self.offlineShopTime.GetSliderPos() * 100 / 10
		conv = str(int(self.pos))
		if str(0) == conv:
			self.offlineShopTime.SetSliderPos(1.0)
			self.timeInputValue.SetText('10 Saat')
			self.hour = 10
			return True
		self.timeInputValue.SetText(conv + ' Saat')
		self.hour = self.pos

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def OnPressExitKey(self):
		self.Close()
		return True

	def Close(self):
		self.ClearDictionary()
		self.acceptButton = None
		self.inputValue = None
		self.priceInputBoard = None
		self.Hide()
		return

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event

	def SetCancelEvent(self, event):
		self.inputValue.OnPressEscapeKey = event

	def GetTitle(self):
		return self.inputValue.GetText()

	def GetTime(self):
		return self.hour

class OfflineShopDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.CurrentShopVID = 0
		self.tooltipItem = 0
		self.questionDialog = None
		self.popup = None
		self.itemBuyQuestionDialog = None
		return

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Refresh(self):
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			itemCount = shop.GetOfflineShopItemCount(i)
			if itemCount <= 1:
				itemCount = 0
			self.itemSlotWindow.SetItemSlot(i, shop.GetOfflineShopItemID(i), itemCount)
			status, who = shop.GetOfflineShopItemStatus(i)
			if status:
				self.itemSlotWindow.ActivateOfflineShopSlot(i)
			else:
				self.itemSlotWindow.DeactivateOfflineShopSlot(i)

		wndMgr.RefreshSlot(self.itemSlotWindow.GetWindowHandle())

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, 'UIScript/OfflineShopDialog.py')
		except:
			import exception
			exception.Abort('OfflineShopDialog.LoadDialog.LoadObject')

		try:
			self.itemSlotWindow = self.GetChild('ItemSlot')
			self.btnBuy = self.GetChild('BuyButton')
			self.btnDestroy = self.GetChild('DestroyButton')
			self.titleBar = self.GetChild('TitleBar')
			self.titleName = self.GetChild('TitleName')
		except:
			import exception
			exception.Abort('OfflineShopDialog.LoadDialog.BindObject')

		self.itemSlotWindow.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlotWindow.SAFE_SetButtonEvent('LEFT', 'EXIST', self.SelectItemSlot)
		self.itemSlotWindow.SAFE_SetButtonEvent('RIGHT', 'EXIST', self.UnselectItemSlot)
		self.itemSlotWindow.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlotWindow.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.btnBuy.SetToggleUpEvent(ui.__mem_func__(self.CancelShopping))
		self.btnBuy.SetToggleDownEvent(ui.__mem_func__(self.OnBuy))
		if player.GetName().find('[') != -1:
			self.btnDestroy.SetEvent(ui.__mem_func__(self.OnDestroyOfflineShop))
		else:
			self.btnDestroy.Hide()
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.Refresh()

	def OnDestroyOfflineShop(self):
		net.SendChatPacket('/destroy_offline_shop ' + str(self.CurrentShopVID))

	def Destroy(self):
		self.Close()
		self.ClearDictionary()
		self.CurrentShopVID = 0
		self.tooltipItem = 0
		self.itemSlotWindow = 0
		self.btnBuy = 0
		self.titleBar = 0
		self.questionDialog = None
		self.popup = None
		return

	def Open(self, vid):
		shop.Open(False, False, True)
		self.SetTop()
		self.Refresh()
		self.Show()
		self.CurrentShopVID = vid
		self.titleName.SetText(chr.GetNameByVID(vid))

	def Close(self):
		if self.itemBuyQuestionDialog:
			self.itemBuyQuestionDialog.Close()
			self.itemBuyQuestionDialog = None
		if self.questionDialog:
			self.OnCloseQuestionDialog()
		shop.Close()
		net.SendOfflineShopEndPacket()
		self.CancelShopping()
		self.tooltipItem.HideToolTip()
		self.Hide()
		return

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnPressExitKey(self):
		self.Close()
		return True

	def OnBuy(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, '<Market Tezg\xe2h\xfd> Her nesneyi onay olmadan sat\xfdn alabilirsin! L\xfctfen dikkatli ol.')
		app.SetCursor(app.BUY)

	def CancelShopping(self):
		self.btnBuy.SetUp()
		app.SetCursor(app.NORMAL)

	def OnCloseQuestionDialog(self):
		if not self.questionDialog:
			return
		else:
			self.questionDialog.Close()
			self.questionDialog = None
			return

	def UnselectItemSlot(self, selectedSlotPos):
		self.AskBuyItem(selectedSlotPos)

	def SelectItemSlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if not isAttached:
			curCursorNum = app.GetCursor()
			if app.BUY == curCursorNum:
				net.SendOfflineShopBuyPacket(selectedSlotPos)
			else:
				selectedItemID = shop.GetOfflineShopItemID(selectedSlotPos)
				itemCount = shop.GetOfflineShopItemCount(selectedSlotPos)
				type = player.SLOT_TYPE_OFFLINE_SHOP
				mouseModule.mouseController.AttachObject(self, type, selectedSlotPos, selectedItemID, itemCount)
				mouseModule.mouseController.SetCallBack('INVENTORY', ui.__mem_func__(self.DropToInventory))
				snd.PlaySound('sound/ui/pick.wav')

	def DropToInventory(self):
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		self.AskBuyItem(attachedSlotPos)

	def AskBuyItem(self, slotPos):
		itemIndex = shop.GetOfflineShopItemID(slotPos)
		itemPrice = shop.GetOfflineShopItemPrice(slotPos)
		itemCount = shop.GetOfflineShopItemCount(slotPos)
		item.SelectItem(itemIndex)
		itemName = item.GetItemName()
		itemBuyQuestionDialog = uiCommon.QuestionDialog()
		itemBuyQuestionDialog.SetText(localeInfo.DO_YOU_BUY_ITEM(itemName, itemCount, localeInfo.NumberToMoneyString(itemPrice)))
		itemBuyQuestionDialog.SetAcceptEvent(lambda arg = True: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.SetCancelEvent(lambda arg = False: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.Open()
		itemBuyQuestionDialog.pos = slotPos
		self.itemBuyQuestionDialog = itemBuyQuestionDialog

	def AnswerBuyItem(self, flag):
		if flag:
			pos = self.itemBuyQuestionDialog.pos
			net.SendOfflineShopBuyPacket(pos)
		self.itemBuyQuestionDialog.Close()
		self.itemBuyQuestionDialog = None
		return

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def OverInItem(self, slotIndex):
		if mouseModule.mouseController.isAttached():
			return
		if self.tooltipItem != 0:
			self.tooltipItem.SetOfflineShopItem(slotIndex)

	def OverOutItem(self):
		if self.tooltipItem != 0:
			self.tooltipItem.HideToolTip()
