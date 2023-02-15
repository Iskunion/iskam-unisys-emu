.section .data
.global audio_payload, audio_payload_end
.global wallpaper, wallpaper_end
.p2align 3
audio_payload:
audio_payload_end:
.p2align 3
wallpaper:
.incbin "src/resources/wallpaper.bin"
wallpaper_end:
