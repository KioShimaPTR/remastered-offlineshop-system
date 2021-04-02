# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: offlineshopbankdialog.py

import uiScriptlocale
window = {
	"name" : "OfflineShopBankWindow",
	"style" : ("movable", "float", ),
	"x" : (SCREEN_WIDTH  - 190) /2,
	"y" : (SCREEN_HEIGHT - 320) /2,
	"width" : 190,
	"height" : 235,
	"children" :
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",
			"style" : ("attach", ),
			"x" : 0,
			"y" : 0,
			"width" : 190,
			"height" : 175,
			"title" : "Tezg\xe2h Bankasý",
			"children" :
			(	
				## Info
				{
					"name":"Info_Text",
					"type":"text",
					"x":22,
					"y":35,
					"text" : uiScriptlocale.BANK_INFO_TEXT,
				},
				## Yang Icon
				{
					"name":"Money_Icon",
					"type":"image",
					"x":14,
					"y":55,
					"image":"d:/ymir work/ui/game/windows/money_icon.sub",
				},
				## Yang Icon
				{
					"name" : "Money_Slot",
					"type":"button",
					"x" : 37,
					"y" : 53,
					"default_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"over_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"down_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"children" : 
					(
						{
							"name" : "Money",
							"type" : "text",
							"x" : 3,
							"y" : 3,
							"horizontal_align" : "right",
							"text_horizontal_align" : "right",
							"text" : "123456789",
						},					
					),
				},
				## Bar1 Icon
				{
					"name":"Bar1_Icon",
					"type":"image",
					"x":14,
					"y":80,
					"image":"locale/tr/offline_shop/bar1.tga",
				},
				## Bar1 Slot
				{
					"name" : "Bar1_Slot",
					"type":"button",
					"x" : 37,
					"y" : 78,
					"default_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"over_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"down_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"children" : 
					(
						{
							"name" : "Bar1",
							"type" : "text",
							"x" : 3,
							"y" : 3,
							"horizontal_align" : "right",
							"text_horizontal_align" : "right",
							"text" : "123456789",
						},					
					),
				},
				## Bar2 Icon
				{
					"name":"Bar2_Icon",
					"type":"image",
					"x":14,
					"y":105,
					"image":"locale/tr/offline_shop/bar2.tga",
				},
				## Bar2 Slot
				{
					"name" : "Bar2_Slot",
					"type":"button",
					"x" : 37,
					"y" : 103,
					"default_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"over_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"down_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"children" : 
					(
						{
							"name" : "Bar2",
							"type" : "text",
							"x" : 3,
							"y" : 3,
							"horizontal_align" : "right",
							"text_horizontal_align" : "right",
							"text" : "123456789",
						},					
					),
				},
				## Bar3 Icon
				{
					"name":"Bar3_Icon",
					"type":"image",
					"x":14,
					"y":130,
					"image":"locale/tr/offline_shop/bar3.tga",
				},
				## Bar3 Slot
				{
					"name" : "Bar3_Slot",
					"type":"button",
					"x" : 37,
					"y" : 128,
					"default_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"over_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"down_image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
					"children" : 
					(
						{
							"name" : "Bar3",
							"type" : "text",
							"x" : 3,
							"y" : 3,
							"horizontal_align" : "right",
							"text_horizontal_align" : "right",
							"text" : "123456789",
						},					
					),
				},
			),
		},
	),
}