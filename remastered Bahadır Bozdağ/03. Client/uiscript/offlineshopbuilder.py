# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: offlineshopbuilder.py

import uiScriptLocale
window = {
	"name": "OfflineShopBuilder",
	"x": SCREEN_WIDTH - 400,
	"y": 10,
	"style": ("movable", "float"),
	"width": 184,
	"height": 369,
	"children":
	(
		{
			"name": "Board",
			"type": "board_with_titlebar",
			"style": ("attach",),
			"x": 0,
			"y": 0,
			"width": 184,
			"height": 369,
			"title": "Market Tezg\xe2h\xfd Kur",
			"children":
			(
				{
					"name": "ItemSlot",
					"type": "grid_table",
					"x": 12,
					"y": 34,
					"start_index": 0,
					"x_count": 5,
					"y_count": 9,
					"x_step": 32,
					"y_step": 32,
					"image": "d:/ymir work/ui/public/Slot_Base.sub"},
				{
					"name": "OkButton",
					"type": "button",
					"x": 10,
					"y": 332,
					"width": 61,
					"height": 21,
					"text": uiScriptLocale.OK,
					"default_image": "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image": "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image": "d:/ymir work/ui/public/middle_button_03.sub"},
				{
					"name": "CloseButton",
					"type": "button",
					"x": 80,
					"y": 332,
					"width": 61,
					"height": 21,
					"text": uiScriptLocale.CLOSE,
					"default_image": "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image": "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image": "d:/ymir work/ui/public/middle_button_03.sub"
				}
			)
		},
	)
}
