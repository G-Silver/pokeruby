LilycoveCity_CoveLilyMotel_1F_MapBorder:: @ 82FC5FC
	.incbin "data/layouts/LilycoveCity_CoveLilyMotel_1F/border.bin"

LilycoveCity_CoveLilyMotel_1F_MapBlockdata:: @ 82FC604
	.incbin "data/layouts/LilycoveCity_CoveLilyMotel_1F/map.bin"

	.align 2
LilycoveCity_CoveLilyMotel_1F_Layout:: @ 82FC6DC
	.4byte 0xc
	.4byte 0x9
	.4byte LilycoveCity_CoveLilyMotel_1F_MapBorder
	.4byte LilycoveCity_CoveLilyMotel_1F_MapBlockdata
	.4byte gTileset_Building
	.4byte gTileset_GenericBuilding
