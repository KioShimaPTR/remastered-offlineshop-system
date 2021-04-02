# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: offlineshopadminpanel.py

import uiScriptLocale
PATH = "d:/ymir work/ui/"
WINDOW_WIDTH = 367
WINDOW_HEIGHT = 250
window = {
	"name": "OfflineShopInputDialog",
	"x": 0,
	"y": 0,
	"style": ("movable", "float"),
	"width": WINDOW_WIDTH,
	"height": WINDOW_HEIGHT,
	"children":
	(
		{
			"name": "board",
			"type": "board",
			"style": ("attach",),
			"x": 0,
			"y": 0,
			"width": WINDOW_WIDTH,
			"height": WINDOW_HEIGHT,
			"children":
			(
				{
					"name": "TitleBar",
					"type": "titlebar",
					"style": ("attach",),
					"x": 8,
					"y": 7,
					"width": WINDOW_WIDTH - 14,
					"color": "yellow",
					"children":
					(
						{
							"name": "TitleName",
							"type": "text",
							"x": WINDOW_WIDTH / 2,
							"y": 3,
							"text": "Market Tezg\xe2h Sistemi",
							"text_horizontal_align": "center"
						},
					)
				},
				{
					"name": "openButton",
					"type": "button",
					"x": 0,
					"y": 155,
					"horizontal_align" : "center",
					"default_image" : "d:/ymir work/ui/Public/xlarge_button_01.sub",
					"over_image" 	: "d:/ymir work/ui/Public/xlarge_button_02.sub",
					"down_image" 	: "d:/ymir work/ui/Public/xlarge_button_03.sub",
					"text": "Tezg\xe2h Aç"
				},
				{
					"name": "GetBackButton",
					"type": "button",
					"x": 0,
					"y": 183,
					"horizontal_align" : "center",
					"default_image" : "d:/ymir work/ui/Public/xlarge_button_01.sub",
					"over_image" 	: "d:/ymir work/ui/Public/xlarge_button_02.sub",
					"down_image" 	: "d:/ymir work/ui/Public/xlarge_button_03.sub",
					"text": "Satýlmayanlar"
				},
				{
					"name": "BankButton",
					"type": "button",
					"x": 0,
					"y": 211,
					"horizontal_align" : "center",
					"default_image" : "d:/ymir work/ui/Public/xlarge_button_01.sub",
					"over_image" 	: "d:/ymir work/ui/Public/xlarge_button_02.sub",
					"down_image" 	: "d:/ymir work/ui/Public/xlarge_button_03.sub",
					"text": "Tezg\xe2h Bankasý"
				},
				{
					"name": "CloseOfflineShop",
					"type": "button",
					"x": 0,
					"y": 155,
					"horizontal_align" : "center",
					"default_image" : "d:/ymir work/ui/Public/xlarge_button_01.sub",
					"over_image" 	: "d:/ymir work/ui/Public/xlarge_button_02.sub",
					"down_image" 	: "d:/ymir work/ui/Public/xlarge_button_03.sub",
					"text": "Tezg\xe2h\xfd Kapat"
				},
				## Teleport
				{
					"name" : "Teleport",
					"type" : "button",
					"x" : 0,
					"y" : 183,
					"horizontal_align" : "center",
					"default_image" : "d:/ymir work/ui/Public/xlarge_button_01.sub",
					"over_image" 	: "d:/ymir work/ui/Public/xlarge_button_02.sub",
					"down_image" 	: "d:/ymir work/ui/Public/xlarge_button_03.sub",
					"text": "Tezg\xe2ha Iþýnlan"
				},
			)
		},
	)
}
