# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: offlineshopinputdialog.py

import uiScriptLocale
PATH = "d:/ymir work/ui/"
WINDOW_WIDTH = 300
WINDOW_HEIGHT = 135
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
					"name": "time_name",
					"type": "text",
					"x": 30,
					"y": 40,
					"text": "Tezg\xe2h Süresi:"
				},
				{
					"name": "input0_name",
					"type": "text",
					"x": 133,
					"y": 41,
					"text": "|cffffff0048 Saat"
				},
				{
					"name" : "InputTime0",
					"type" : "radio_button",
					
					"x" : 110,
					"y" : 40,
					"tooltip_text" : "|cffffff00(Ücretsiz)",
					
					"default_image" : "locale/tr/offline_shop/default_image.tga",
					"over_image" : "locale/tr/offline_shop/down_image.tga",
					"down_image" : "locale/tr/offline_shop/over_image.tga",
				},
				{
					"name" : "InputTime1",
					"type" : "radio_button",
					
					"x" : 190,
					"y" : 40,
					"tooltip_text" : "|cffffff0025M Yang",
					
					"default_image" : "locale/tr/offline_shop/default_image.tga",
					"over_image" : "locale/tr/offline_shop/down_image.tga",
					"down_image" : "locale/tr/offline_shop/over_image.tga",
				},
				{
					"name": "input1_name",
					"type": "text",
					"x": 213,
					"y": 41,
					"text": "|cffffff0072 Saat"
				},
				{
					"name": "shop_name",
					"type": "text",
					"x": 30,
					"y": 70,
					"text": "Tezg\xe2h Ad\xfd:"
				},
				{
					"name": "InputSlot",
					"type": "slotbar",
					"x": 40,
					"y": 70,
					"width": 165,
					"height": 15,
					"horizontal_align": "center",
					"children":
					(
						{
							"name": "InputValue",
							"type": "editline",
							"x": 1,
							"y": 1,
							"width": 165,
							"height": 15,
							"input_limit": 32
						},
					)
				},
				{
					"name": "CancelButton",
					"type": "button",
					"x": 170,
					"y": 93,
					"default_image": "d:/ymir work/ui/public/large_button_01.sub",
					"over_image": "d:/ymir work/ui/public/large_button_02.sub",
					"down_image": "d:/ymir work/ui/public/large_button_03.sub",
					"text": "Vazgeç"
				},
				{
					"name": "AgreeButton",
					"type": "button",
					"x": 70,
					"y": 93,
					"default_image": "d:/ymir work/ui/public/large_button_01.sub",
					"over_image": "d:/ymir work/ui/public/large_button_02.sub",
					"down_image": "d:/ymir work/ui/public/large_button_03.sub",
					"text": "Onayla"
				},
			)
		},
	)
}
