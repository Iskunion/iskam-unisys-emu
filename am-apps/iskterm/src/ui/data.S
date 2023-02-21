.section .data
.global wallpaper, wallpaper_width, wallpaper_height
.p2align 2
wallpaper_width:
  .word 320
wallpaper_height:
  .word 240
.p2align 2
.p2align 3
wallpaper:
  .incbin "resources/wallpaper.bin" 