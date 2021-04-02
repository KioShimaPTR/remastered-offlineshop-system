# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: uiofflineshop.py

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
import uiPickMoney
import uiAffectShower

g_isEditingOfflineShop = False

def IsEditingOfflineShop():
	global g_isEditingOfflineShop
	if g_isEditingOfflineShop:
		return True
	else:
		return False

class TextBoard(ui.ThinBoardCircle):
	def __init__(self):
		ui.ThinBoardCircle.__init__(self)
		self.lineHeight = 8
		self.childrenList = []

	def __del__(self):
		ui.ThinBoardCircle.__del__(self)

	def Clear(self):
		self.lineHeight = 8
		self.childrenList = []

	def AppendTextLine(self, text):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(localeInfo.UI_DEF_FONT)
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
		self.hour = 0
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
			self.acceptButton = self.GetChild('AgreeButton')
			self.CancelButton = self.GetChild('CancelButton')
			self.inputValue = self.GetChild('InputValue')
			self.inputTime0 = self.GetChild("InputTime0")
			self.inputTime1 = self.GetChild("InputTime1")
		except:
			import exception
			exception.Abort('OfflineShopInputDialog.LoadWindow.BindObject')

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.CancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.inputTime0.SetEvent(ui.__mem_func__(self.__ChangeInputTime0))
		self.inputTime1.SetEvent(ui.__mem_func__(self.__ChangeInputTime1))

	def __ChangeInputTime0(self):
		self.hour = 48
		self.GetChild("InputTime0").Down()
		self.GetChild("InputTime1").SetUp()
	
	def __ChangeInputTime1(self):
		self.hour = 72
		self.GetChild("InputTime0").SetUp()
		self.GetChild("InputTime1").Down()

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.ClearDictionary()
		self.acceptButton = None
		self.CancelButton = None
		self.inputValue = None
		self.inputTime0 = None
		self.inputTime1 = None
		self.hour = 0
		self.Hide()
		return

	def OnPressExitKey(self):
		self.Close()
		return True

	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event

	def SetCancelEvent(self, event):
		self.inputValue.OnPressEscapeKey = event

	def GetTitle(self):
		return self.inputValue.GetText()

	def GetTime(self):
		return self.hour

class OfflineShopAdminPanel(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		return

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, 'UIScript/offlineshopadminpanel.py')
		except:
			import exception
			exception.Abort('OfflineShopInputDialog.LoadWindow.LoadObject')

		try:
			self.titleBar = self.GetChild('TitleBar')
			self.openButton = self.GetChild('openButton')
			self.bankButton = self.GetChild('BankButton')
			self.closeOfflineShopButton = self.GetChild('CloseOfflineShop')
			self.btnTeleport = self.GetChild("Teleport")
			self.btnGetBack = self.GetChild("GetBackButton")
		except:
			import exception
			exception.Abort('OfflineShopInputDialog.LoadWindow.BindObject')

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.openButton.SetEvent(ui.__mem_func__(self.OpenMyOfflineShop))
		self.bankButton.SetEvent(ui.__mem_func__(self.ClickMyBankButton))
		self.closeOfflineShopButton.SetEvent(ui.__mem_func__(self.ClickCloseOfflineShopButton))
		self.btnTeleport.SetEvent(ui.__mem_func__(self.TeleportShop))
		self.btnGetBack.SetEvent(ui.__mem_func__(self.ClickGetBackButton))

		self.textBoard = TextBoard()
		self.textBoard.SetParent(self)
		self.textBoard.SetPosition(11, 32)
		self.textBoard.SetSize(345, 118)
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
		lines = self.SplitDescription('Market Tezg\xe2h\xfd sistemi sen oyunda olmasan da senin belirledi\xf0in s\xfcre boyunca nesnelerinin ticaretinin yap\xfdlmas\xfdn\xfd sa\xf0lar. Bu sistem sadece CH1 de kullan\xfdlabilir. Oyundan \xe7\xfdkman tezg\xe2h\xfdn\xfd kapatmaz. Diledi\xf0in zaman Tezg\xe2h Bankasýndan sat\xfdlan nesnelerinin paras\xfdn\xfd alabilirsin. Satýlmayanlar butonu tezg\xe2h\xfdný kapattýðýn vakit satýlmayan itemlerini Market Tezg\xe2h\xfd depondan geri \xe7ekmek i\xe7in kullan\xfdl\xfdr.', 75)
		if not lines:
			return
		for line in lines:
			self.textBoard.AppendTextLine(line)

	def OpenMyOfflineShop(self):
		self.Close()
		net.SendChatPacket("/open_offlineshop 1")

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
		self.closeQuestionDialog = None
		return

	def ClickMyBankButton(self):
		wndOfflineShopMyBank = OfflineShopBankDialog()
		self.wndOfflineShopMyBank = wndOfflineShopMyBank
		self.Close()
		self.wndOfflineShopMyBank.Open()
		return True

	def Open(self):
		net.SendChatPacket("/open_offlineshop 0")
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def TeleportShop(self):
		self.Close()
		net.SendOfflineShopTeleportPacket()

	def ClickGetBackButton(self):
		wndOfflineShopGetBack = OfflineShopGetBackWindow()
		self.wndOfflineShopGetBack = wndOfflineShopGetBack
		self.Close()
		self.wndOfflineShopGetBack.SetTop()
		self.wndOfflineShopGetBack.SetCenterPosition()
		self.wndOfflineShopGetBack.Open()
		return True

	def OnUpdate(self):
		if chr.IsOfflineShopPC(player.GetMainCharacterIndex()):
			self.openButton.Hide()
			self.btnGetBack.Hide()
			self.closeOfflineShopButton.Show()
			self.btnTeleport.Show()
		else:
			self.openButton.Show()
			self.btnGetBack.Show()
			self.closeOfflineShopButton.Hide()
			self.btnTeleport.Hide()

	def Close(self):
		self.ClearDictionary()
		self.openButton = None
		self.bankButton = None
		self.closeOfflineShopButton = None
		self.btnTeleport = None
		self.btnGetBack = None
		self.textBoard = None
		self.Hide()
		return

	def OnPressExitKey(self):
		self.Close()
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True

class OfflineShopBankDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.updateTime = 0
		self.withdrawMoneyTime = 0
		self.LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShopBankDialog.py")
		except:
			import exception
			exception.Abort("OfflineShopBankDialog.LoadWindow.LoadScript")
			
		try:
			self.Board = self.GetChild("Board")
			self.wndMoney = self.GetChild("Money")
			self.wndMoneySlot = self.GetChild("Money_Slot")
			self.wndBar1 = self.GetChild("Bar1")
			self.wndBar1Slot = self.GetChild("Bar1_Slot")
			self.wndBar2 = self.GetChild("Bar2")
			self.wndBar2Slot = self.GetChild("Bar2_Slot")
			self.wndBar3 = self.GetChild("Bar3")
			self.wndBar3Slot = self.GetChild("Bar3_Slot")
		except:
			import exception
			exception.Abort("OfflineShopBankDialog.LoadWindow.BindObject")
			
		self.Board.SetCloseEvent(ui.__mem_func__(self.Close))
		self.wndMoneySlot.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog))
		self.wndBar1Slot.SetEvent(ui.__mem_func__(self.OpenPickBar1Dialog))
		self.wndBar2Slot.SetEvent(ui.__mem_func__(self.OpenPickBar2Dialog))
		self.wndBar3Slot.SetEvent(ui.__mem_func__(self.OpenPickBar3Dialog))
		
	def Close(self):
		self.Hide()
		
	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		
		net.SendRefreshOfflineShopPrice()
		self.wndMoney.SetText(localeInfo.NumberToMoneyString(player.GetCurrentOfflineShopMoney()))
		self.wndBar1.SetText(localeInfo.NumberToBar1String(player.GetCurrentOfflineShopBar1()))
		self.wndBar2.SetText(localeInfo.NumberToBar2String(player.GetCurrentOfflineShopBar2()))
		self.wndBar3.SetText(localeInfo.NumberToBar3String(player.GetCurrentOfflineShopBar3()))
		self.Show()
		
	def OpenPickMoneyDialog(self):
		moneyDialog = uiPickMoney.PickMoneyDialog()
		moneyDialog.LoadDialog()
		moneyDialog.SetMax(10)
		moneyDialog.SetTitleName(localeInfo.MONETARY_UNIT0)
		moneyDialog.SetAcceptEvent(ui.__mem_func__(self.OnWithdrawMoney))
		moneyDialog.Open(player.GetCurrentOfflineShopMoney())
		self.moneyDialog = moneyDialog
		
	def OnWithdrawMoney(self, money):
		net.SendOfflineShopWithdrawMoney(money)
		net.SendRefreshOfflineShopPrice()
		self.withdrawMoneyTime = app.GetTime()

	def OpenPickBar1Dialog(self):
		moneyDialog = uiPickMoney.PickMoneyDialog()
		moneyDialog.LoadDialog()
		moneyDialog.SetMax(3)
		moneyDialog.SetTitleName(localeInfo.MONETARY_UNIT_BAR1)
		moneyDialog.SetAcceptEvent(ui.__mem_func__(self.OnWithdrawBar1))
		moneyDialog.Open(player.GetCurrentOfflineShopBar1())
		self.moneyDialog = moneyDialog
		
	def OnWithdrawBar1(self, bar):
		net.SendOfflineShopWithdrawBar1(bar)
		net.SendRefreshOfflineShopPrice()
		self.withdrawMoneyTime = app.GetTime()
		
	def OpenPickBar2Dialog(self):
		moneyDialog = uiPickMoney.PickMoneyDialog()
		moneyDialog.LoadDialog()
		moneyDialog.SetMax(3)
		moneyDialog.SetTitleName(localeInfo.MONETARY_UNIT_BAR2)
		moneyDialog.SetAcceptEvent(ui.__mem_func__(self.OnWithdrawBar2))
		moneyDialog.Open(player.GetCurrentOfflineShopBar2())
		self.moneyDialog = moneyDialog
		
	def OnWithdrawBar2(self, bar):
		net.SendOfflineShopWithdrawBar2(bar)
		net.SendRefreshOfflineShopPrice()
		self.withdrawMoneyTime = app.GetTime()
		
	def OpenPickBar3Dialog(self):
		moneyDialog = uiPickMoney.PickMoneyDialog()
		moneyDialog.LoadDialog()
		moneyDialog.SetMax(3)
		moneyDialog.SetTitleName(localeInfo.MONETARY_UNIT_BAR3)
		moneyDialog.SetAcceptEvent(ui.__mem_func__(self.OnWithdrawBar3))
		moneyDialog.Open(player.GetCurrentOfflineShopBar3())
		self.moneyDialog = moneyDialog
		
	def OnWithdrawBar3(self, bar):
		net.SendOfflineShopWithdrawBar3(bar)
		net.SendRefreshOfflineShopPrice()
		self.withdrawMoneyTime = app.GetTime()

	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def OnPressExitKey(self):
		self.Close()
		return True
		
	def OnUpdate(self):
		try:
			if (app.GetTime() < self.updateTime + 1):
				return
			
			self.updateTime = app.GetTime()
			net.SendRefreshOfflineShopPrice()
			self.wndMoney.SetText(localeInfo.NumberToMoneyString(player.GetCurrentOfflineShopMoney()))
			self.wndBar1.SetText(localeInfo.NumberToBar1String(player.GetCurrentOfflineShopBar1()))
			self.wndBar2.SetText(localeInfo.NumberToBar2String(player.GetCurrentOfflineShopBar2()))
			self.wndBar3.SetText(localeInfo.NumberToBar3String(player.GetCurrentOfflineShopBar3()))
		except:
			pass

class OfflineShopDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.CurrentShopVID = 0
		self.tooltipItem = 0
		self.questionDialog = None
		self.popup = None
		self.itemBuyQuestionDialog = None
		self.wndOfflineShopGetBack = None
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
			self.looked = self.GetChild("looked")
			self.lookedslot = self.GetChild("lookedslot")
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

	def Open(self, vid, looked):
		shop.Open(False, False, True)
		self.SetTop()
		self.Refresh()
		self.Show()
		self.lookedslot.Show()
		self.looked.Show()
		self.looked.SetText(str(looked) + " kez incelendi.")
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
		itemPrice2 = shop.GetOfflineShopItemPrice2(slotPos)
		itemPriceType = shop.GetOfflineShopItemPriceType(slotPos)
		itemCount = shop.GetOfflineShopItemCount(slotPos)
		item.SelectItem(itemIndex)
		itemName = item.GetItemName()
		if itemPrice2 > 0:
			itemBuyQuestionDialog = uiCommon.QuestionDialog2()
			itemBuyQuestionDialog.SetText1(localeInfo.DO_YOU_BUY_ITEM(itemName, itemCount, localeInfo.NumberToMoneyString(itemPrice)))
			itemBuyQuestionDialog.SetText2(localeInfo.DO_YOU_BUY_ITEM_EX(itemPrice2, itemPriceType))
		else:
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

class OfflineShopGetBackWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.tooltipItem = None
		self.questionDialog = None
		self.title = ""
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Refresh(self):
		net.SendUnsoldItems()
		iCount = 0
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			if (shop.GetOfflineShopItemID(i) == -842150451):
				iCount = iCount + 1
				
		if (iCount == shop.OFFLINE_SHOP_SLOT_COUNT):
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Satýþta iteminiz bulunmuyor.")
			return
			
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			itemCount = shop.GetOfflineShopItemCount(i)
			if (itemCount <= 1):
				itemCount = 0
					
			self.itemSlot.SetItemSlot(i, shop.GetOfflineShopItemID(i), itemCount)
		
		wndMgr.RefreshSlot(self.itemSlot.GetWindowHandle())
		
	def SetItemData(self, pos, itemID, itemCount, itemPrice):
		shop.SetOfflineShopItemData(pos, itemID, itemCount, itemPrice)
		
	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShopBuilder.py")
		except:
			import exception
			exception.Abort("OfflineShopRemoveItemWindow.LoadWindow.LoadObject")
			
		try:
			self.itemSlot = self.GetChild("ItemSlot")
			self.btnOk = self.GetChild("OkButton")
			self.btnClose = self.GetChild("CloseButton")
			self.board = self.GetChild("Board")
		except:
			import exception
			exception.Abort("OfflineShopRemoveItemWindow.LoadWindow.BindObject")
			
		self.btnOk.Hide()
		self.btnClose.Hide()
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		
		self.btnOk.SetEvent(ui.__mem_func__(self.AllGetBackItem))
		self.btnClose.SetEvent(ui.__mem_func__(self.Close))
		self.btnOk.SetText("Hepsini Al")
		self.btnOk.Show()
		self.btnClose.Show()
		
		self.itemSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlot.SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))				
				
	def Destroy(self):
		self.ClearDictionary()

		self.itemSlot = None
		self.btnOk = None
		self.btnClose = None
		self.board = None
		
	def Open(self):
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()
		self.board.SetTitleName("Satýlmayan Nesneler")
		self.Refresh()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()
		
		global g_isEditingOfflineShop
		g_isEditingOfflineShop = True
		
	def Close(self):
		global g_isEditingOfflineShop
		g_isEditingOfflineShop = False
		
		if (self.questionDialog):
			self.questionDialog.Close()
			self.questionDialog = None

		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def OnPressExitKey(self):
		self.Close()
		return True

	def AllGetBackItem(self):
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			id = shop.GetOfflineShopItemID(i)
			if (id > 0):
				net.SendOfflineShopFetchItem(i)
				net.SendUnsoldItems();

	def UnselectItemSlot(self, selectedSlotPos):
		net.SendOfflineShopFetchItem(selectedSlotPos)
		net.SendUnsoldItems();
		
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem
		
	def OverInItem(self, slotIndex):
		if (mouseModule.mouseController.isAttached()):
			return
			
		if (self.tooltipItem != 0):
			self.tooltipItem.SetOfflineShopItem(slotIndex)
			
	def OverOutItem(self):
		if (self.tooltipItem != 0):
			self.tooltipItem.HideToolTip()
			
	def OnUpdate(self):
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			itemCount = shop.GetOfflineShopItemCount(i)
			if (itemCount <= 1):
				itemCount
			self.itemSlot.SetItemSlot(i, shop.GetOfflineShopItemID(i), itemCount)
		wndMgr.RefreshSlot(self.itemSlot.GetWindowHandle())	
