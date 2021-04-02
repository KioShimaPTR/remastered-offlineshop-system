import uiScriptLocale
window = {
	"name": "OfflineShopAdminPanelWindow",
	"style": ("movable", "float"),
	"x": 0,
	"y": 0,
	"width": 140,
	"height": 100,
	"children":
	(
		{
			"name": "Board",
			"type": "board_with_titlebar",
			"style": ("attach",),
			"x": 0,
			"y": 0,
			"width": 140,
			"height": 100,
			"title": "Market Tezg\xe2h\xfdm",
			"children":
			(
				{
				"name": "CloseOfflineShopButton",
				"type": "button",
				"x": 25,
				"y": 35,
				"text": "Tezg\xe2h\xfd Kapat",
				"default_image": "d:/ymir work/ui/public/large_button_01.sub",
				"over_image": "d:/ymir work/ui/public/large_button_02.sub",
				"down_image": "d:/ymir work/ui/public/large_button_03.sub"
				},
				{
					"name": "MyBankButton",
					"type": "button",
					"x": 25,
					"y": 60,
					"text": "Banka Hesab\xfd",
					"default_image": "d:/ymir work/ui/public/large_button_01.sub",
					"over_image": "d:/ymir work/ui/public/large_button_02.sub",
					"down_image": "d:/ymir work/ui/public/large_button_03.sub"
				}
			)
		},
	)
}
