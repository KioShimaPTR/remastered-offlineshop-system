# Author: Bahadýr Bozdað or Cosby or Kioshima
# File name: offlineshopdialog.py

import uiScriptLocale
window = {
	"name": "OfflineShopDialog",
	"x": SCREEN_WIDTH - 400,
	"y": 10,
	"style": ("movable", "float"),
	"width": 184,
	"height": 369,
	"children":
	(
		{
			"name": "board",
			"type": "board",
			"style": ("attach",),
			"x": 0,
			"y": 0,
			"width": 184,
			"height": 369,
			"children":
			(
				{
					"name": "TitleBar",
					"type": "titlebar",
					"style": ("attach",),
					"x": 8,
					"y": 8,
					"width": 169,
					"color": "gray",
					"children":
					(
						{
							"name": "TitleName",
							"type": "text",
							"x": 84,
							"y": 4,
							"text": uiScriptLocale.SHOP_TITLE,
							"text_horizontal_align": "center"
						},
					)
				},
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
					"image": "d:/ymir work/ui/public/Slot_Base.sub"
				},
				{
					"name" : "lookedslot",
					"type" : "slotbar",

					"x" : 30,
					"y" : 332,

					"width" : 95,
					"height" : 18,

					"horizontal_align" : "center",

					"children" :
					(
						{
							"name" : "looked",
							"type" : "text",

							"x" : 12,
							"y" : 3,

							"width" : 120,
							"height" : 18,
						},
					),
				},
				{
					"name": "BuyButton",
					"type": "toggle_button",
					"x": 10,
					"y": 332,
					"text": uiScriptLocale.SHOP_BUY,
					"default_image": "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image": "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image": "d:/ymir work/ui/public/middle_button_03.sub"
				},
				{
					"name": "DestroyButton",
					"type": "button",
					"x": 80,
					"y": 332,
					"text": "Yok Et",
					"default_image": "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image": "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image": "d:/ymir work/ui/public/middle_button_03.sub"
				}
			)
		},
	)
}
