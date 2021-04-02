# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: inputdialogshop.py

import uiScriptLocale

ROOT_PATH = "d:/ymir work/ui/public/"

window = {
	"name" : "InputDialogShop",

	"x" : 0,
	"y" : 0,

	"style" : ("movable", "float",),

	"width" : 170,
	"height" : 90 + 30,

	"children" :
	(
		{
			"name" : "Board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : 170,
			"height" : 90 + 30,

			"title" : "",

			"children" :
			(

				## Input Slot
				{
					"name" : "InputSlot",
					"type" : "slotbar",

					"x" : 0,
					"y" : 34,
					"width" : 90,
					"height" : 18,
					"horizontal_align" : "center",

					#"type" : "image",
					#"image" : "d:/ymir work/ui/public/Parameter_Slot_03.sub",

					"children" :
					(
						{
							"name" : "InputValue",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 90,
							"height" : 18,

							"input_limit" : 12,
						},
					),
				},
				
				{
					"name" : "InputTimeText",
					"type" : "text",
					
					"x" : 20,
					"y" : 58,
					
					"text" : uiScriptLocale.OFFLINE_SHOP_TIME,
				},
				
				{
					"name" : "InputTime0",
					"type" : "radio_button",
					
					"x" : 45,
					"y" : 58,
					
					"tooltip_text" : uiScriptLocale.OFFLINE_SHOP_TIME_0,
					
					"default_image" : "locale/tr/offlineshop/default_image.tga",
					"over_image" : "locale/tr/offlineshop/down_image.tga",
					"down_image" : "locale/tr/offlineshop/over_image.tga",
				},
				{
					"name" : "InputTime1",
					"type" : "radio_button",
					
					"x" : 115,
					"y" : 58,
					
					"tooltip_text" : uiScriptLocale.OFFLINE_SHOP_TIME_1,
					
					"default_image" : "locale/tr/offlineshop/default_image.tga",
					"over_image" : "locale/tr/offlineshop/down_image.tga",
					"down_image" : "locale/tr/offlineshop/over_image.tga",
				},
				{
					"name" : "InputTime2",
					"type" : "radio_button",
					
					"x" : 195,
					"y" : 58,
					
					"tooltip_text" : uiScriptLocale.OFFLINE_SHOP_TIME_2,
					
					"default_image" : "locale/tr/offlineshop/default_image.tga",
					"over_image" : "locale/tr/offlineshop/down_image.tga",
					"down_image" : "locale/tr/offlineshop/over_image.tga",
				},
				{
					"name" : "InputTime3",
					"type" : "radio_button",
					
					"x" : 265,
					"y" : 58,
					
					"tooltip_text" : uiScriptLocale.OFFLINE_SHOP_TIME_3,
					
					"default_image" : "locale/tr/offlineshop/default_image.tga",
					"over_image" : "locale/tr/offlineshop/down_image.tga",
					"down_image" : "locale/tr/offlineshop/over_image.tga",
				},
			
				## Button
				{
					"name" : "AcceptButton",
					"type" : "button",

					"x" : - 61 - 5 + 30,
					"y" : 58 + 30,
					"horizontal_align" : "center",

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "CancelButton",
					"type" : "button",

					"x" : 5 + 30,
					"y" : 58 + 30,
					"horizontal_align" : "center",

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}