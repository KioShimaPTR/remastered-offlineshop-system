import uiScriptLocale
window = {
	"name": "OfflineShopDialog",
	"x": SCREEN_WIDTH - 400,
	"y": 10,
	"style": ("movable", "float"),
	"width": 280,
	"height": 328,
	"children":
	(
		{
			"name": "board",
			"type": "board",
			"style": ("attach",),
			"x": 0,
			"y": 0,
			"width": 280,
			"height": 328,
			"children":
			(
				{
					"name": "TitleBar",
					"type": "titlebar",
					"style": ("attach",),
					"x": 8,
					"y": 8,
					"width": 265,
					"color": "gray",
					"children":
					(
						{
							"name": "TitleName",
							"type": "text",
							"x": 135,
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
					"x_count": 8,
					"y_count": 8,
					"x_step": 32,
					"y_step": 32,
					"image": "d:/ymir work/ui/public/Slot_Base.sub"
				},
				{
					"name": "BuyButton",
					"type": "toggle_button",
					"x": 10,
					"y": 295,
					"text": uiScriptLocale.SHOP_BUY,
					"default_image": "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image": "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image": "d:/ymir work/ui/public/middle_button_03.sub"
				},
				{
					"name": "DestroyButton",
					"type": "button",
					"x": 80,
					"y": 295,
					"text": "Yok Et",
					"default_image": "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image": "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image": "d:/ymir work/ui/public/middle_button_03.sub"
				}
			)
		},
	)
}
