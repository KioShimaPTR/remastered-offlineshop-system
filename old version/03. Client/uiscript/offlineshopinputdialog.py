import uiScriptLocale
PATH = "d:/ymir work/ui/"
WINDOW_WIDTH = 550
WINDOW_HEIGHT = 420
DESC_WINDOW_WIDTH = WINDOW_WIDTH - 150
DESC_WINDOW_HEIGHT = WINDOW_HEIGHT - 220
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
					"name": "board_select_open_header_1",
					"type": "expanded_image",
					"style": ("attach",),
					"x": 8,
					"y": 279,
					# "image": PATH + "offlineshop_admin.tga"
				},
				{
					"name": "board_select_open_text_1",
					"type": "text",
					"x": 52,
					"y": 283,
					"text": "Market Tezg\xe2h\xfd A\xe7"
				},
				{
					"name": "board_select_open_header_2",
					"type": "expanded_image",
					"style": ("attach",),
					"x": 187,
					"y": 279,
					# "image": PATH + "offlineshop_admin.tga"
				},
				{
					"name": "board_select_open_text_2",
					"type": "text",
					"x": 223,
					"y": 283,
					"text": "Market Tezg\xe2h\xfd Y\xf6net"
				},
				{
					"name": "board_select_open_header_2",
					"type": "expanded_image",
					"style": ("attach",),
					"x": 366,
					"y": 279,
					# "image": PATH + "offlineshop_admin.tga"
				},
				{
					"name": "board_select_open_text_2",
					"type": "text",
					"x": 384,
					"y": 283,
					"text": "Market Tezg\xe2h\xfd Di\xf0er Se\xe7enekler"
				},
				{
					"name": "board_select",
					"type": "expanded_image",
					"x": 8,
					"y": 304,
					"image": "locale/tr/offline_shop/offlineshop_bg.tga",
					"children":
					(
						{
							"name": "OfflineShopTime",
							"type": "sliderbar",
							"x": 2,
							"y": 15
						},
						{
							"name": "time_value",
							"type": "text",
							"x": 38,
							"y": 25,
							"text": "Mevcut S\xfcre:"
						},
						{
							"name": "TimeInputValue",
							"type": "text",
							"x": 100,
							"y": 25,
							"text": "10 Saat",
							"color": 4284940032L
						},
						{
							"name": "shop_name",
							"type": "text",
							"x": 62,
							"y": 40,
							"text": "Tezg\xe2h Ad\xfd:"
						},
						{
							"name": "InputSlot",
							"type": "slotbar",
							"x": -178,
							"y": 57,
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
							"name": "AgreeButton",
							"type": "button",
							"x": 42,
							"y": 77,
							"default_image": "d:/ymir work/ui/public/large_button_01.sub",
							"over_image": "d:/ymir work/ui/public/large_button_02.sub",
							"down_image": "d:/ymir work/ui/public/large_button_03.sub",
							"text": "Onayla"
						},
						{
							"name": "add_item_text",
							"type": "text",
							"x": 220,
							"y": 18,
							"text": "Nesne Ekle:",
							"color": 4284940032L
						},
						{
							"name": "ItemSlot",
							"type": "grid_table",
							"x": 284,
							"y": 10,
							"start_index": 0,
							"x_count": 1,
							"y_count": 1,
							"x_step": 32,
							"y_step": 32,
							"image": "d:/ymir work/ui/public/Slot_Base.sub"
						},
						{
							"name": "WatchButton",
							"type": "button",
							"x": 230,
							"y": 62,
							"default_image": "d:/ymir work/ui/public/large_button_01.sub",
							"over_image": "d:/ymir work/ui/public/large_button_02.sub",
							"down_image": "d:/ymir work/ui/public/large_button_03.sub",
							"text": "Tezg\xe2ha Bak"
						},
						{
							"name": "BankButton",
							"type": "button",
							"x": 408,
							"y": 18,
							"default_image": "d:/ymir work/ui/public/large_button_01.sub",
							"over_image": "d:/ymir work/ui/public/large_button_02.sub",
							"down_image": "d:/ymir work/ui/public/large_button_03.sub",
							"text": "Yang \xc7ek"
						},
						{
							"name": "RetrieveItems",
							"type": "button",
							"x": 408,
							"y": 40,
							"default_image": "d:/ymir work/ui/public/large_button_01.sub",
							"over_image": "d:/ymir work/ui/public/large_button_02.sub",
							"down_image": "d:/ymir work/ui/public/large_button_03.sub",
							"text": "Nesneleri Getir"
						},
						{
							"name": "CloseOfflineShop",
							"type": "button",
							"x": 408,
							"y": 62,
							"default_image": "d:/ymir work/ui/public/large_button_01.sub",
							"over_image": "d:/ymir work/ui/public/large_button_02.sub",
							"down_image": "d:/ymir work/ui/public/large_button_03.sub",
							"text": "Tezg\xe2h\xfd Kapat"
						}
					)
				}
			)
		},
	)
}
