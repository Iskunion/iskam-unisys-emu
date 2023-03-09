#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct bmp_header{
  uint16_t bfType;
  uint32_t bfSize;
  uint32_t bfReserved;
  uint32_t bfOffBits;
}__attribute__((packed));

struct bitmap_info
{
  uint32_t biSize;
  uint32_t biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImages;
  int32_t  biXPelsPerMeter;
  int32_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
}__attribute((packed));

typedef struct bmp_header bmp_header;
typedef struct bitmap_info bitmap_info;

typedef struct rgb_8bit
{
  uint8_t blue  : 2;
  uint8_t green : 3;
  uint8_t red   : 3;
} rgb_8bit;

struct rgb_32bit
{
  uint8_t blue;
  uint8_t green;
  uint8_t red;
  uint8_t alpha;
}__attribute__((packed));

typedef struct rgb_32bit rgb_32bit;

static inline uint8_t turn256c(uint32_t *pixel) {
  rgb_8bit target = (rgb_8bit) {
    .blue   = ((rgb_32bit *) pixel)->blue >> 6,
    .green  = ((rgb_32bit *) pixel)->green >> 5,
    .red    = ((rgb_32bit *) pixel)->red >> 5
  };
  return *(uint8_t *) &target;
}

rgb_32bit palette[256];

int main(int argc, char *argv[]){
  printf("%d\n", sizeof(bitmap_info));
  if (argc <= 1) {
    puts("No picture!");
    return -1;
  }
  FILE *picture = fopen(argv[1], "rb");
  FILE *picbin = fopen("wallpaper.bin", "wb");
  FILE *npicture = fopen("wallpaper8bitnew.bmp", "wb");
  freopen("wallpaper.log", "w", stderr);
  bmp_header header;
  bitmap_info info;
  fread(&header, sizeof (bmp_header), 1, picture);
  fread(&info, sizeof (bitmap_info), 1, picture);

  fwrite(&header, sizeof (bmp_header), 1, npicture);
  fwrite(&info, sizeof (bitmap_info), 1, npicture);

  printf("This is a(n) %d bit Picture with ", info.biBitCount);
  printf("width: %d height: %d\n", info.biWidth, info.biHeight);

  printf("start at 0x%08x (%d)\n", ftell(picture), ftell(picture)); 

  //Load Palette
  if (info.biBitCount == 8) {
    if (!info.biClrUsed) info.biClrUsed = 256;
    fread(palette, 1, info.biClrUsed * 4, picture);
    
    fprintf(stderr, "loaded palette info:\n");
    // for (int i = 0; i < 256; i++) {
    //   palette[i] = (rgb_32bit) {.alpha = 0, .blue = 0xc0, }
    // }
    
    for (int i = 0; i < info.biClrUsed; i++) {
      fprintf(stderr, "color %d: 0x%08x\n", i, palette[i]);

      // if ((palette[i].blue & 0x20) && (palette[i].blue & 0xc0 < 0xc0))
      //   palette[i].blue += 0x10;
      // if (palette[i].green & 0x10 && (palette[i].green & 0xe0 < 0xe0))
      //   palette[i].green += 0x20;
      // if (palette[i].red & 0x10 && (palette[i].red & 0xe0 < 0xe0))
      //   palette[i].red += 0x20;

      palette[i].blue  &= 0xc0;
      palette[i].red   &= 0xe0;
      palette[i].green &= 0xe0;

      // palette[i].blue  += 0x3f;
      // palette[i].red   += 0x1f;
      // palette[i].green += 0x1f;
      fprintf(stderr, "reduced to 0x%08x\n", palette[i]);
    }
    fwrite(palette, sizeof (rgb_32bit), info.biClrUsed, npicture);
  }

  printf("start at 0x%08x (%d)\n", ftell(picture), ftell(picture));
  // fseek(picture, header.bfOffBits, SEEK_SET);
  uint32_t rowbytes = ((info.biBitCount / 8) * info.biWidth + 3) & ~0x3;
  printf("%d bytes per row\n", rowbytes);
  uint8_t *bitstream = (uint8_t *) malloc(rowbytes);
  uint8_t *binstream = (uint8_t *) malloc(info.biHeight * info.biWidth);

  if (info.biBitCount == 8) {
    if (info.biHeight > 0) {
      uint8_t *binstream_cursor = binstream + info.biHeight * info.biWidth;
      int nowheight = info.biHeight;
      while (nowheight--){
        fread(bitstream, sizeof (uint8_t), rowbytes, picture);
        binstream_cursor -= info.biWidth;
        for (int i = 0; i < info.biWidth; i++) {
          rgb_32bit color_32 = palette[bitstream[i]];
          rgb_8bit color = (rgb_8bit) {
            .blue = color_32.blue / 64,
            .green = color_32.green / 32,
            .red = color_32.red / 32
          };
          binstream_cursor[i] = *(uint8_t *) &color;
          fprintf(stderr, "turning %08x to %02x\n", color_32, color);
        }
        fwrite(bitstream, sizeof (uint8_t), rowbytes, npicture);
      }
      printf("write to binary stream %d\n", info.biHeight * info.biWidth);
      fwrite(binstream, sizeof (uint8_t), info.biHeight * info.biWidth, picbin);
    }
  } else {
    if (info.biHeight > 0){
      uint8_t *binstream_cursor = binstream + info.biHeight * info.biWidth;
      int nowheight = info.biHeight;
      while (nowheight--){
        fread(bitstream, sizeof (uint8_t), rowbytes, picture);
        binstream_cursor -= info.biWidth;
        for (int i = 0; i < info.biWidth; i++){
          fprintf(stderr, "%02x %02x %02x\n", bitstream[i * 3], bitstream[i * 3 + 1], bitstream[i * 3 + 2]);
          fprintf(stderr, "%02x %02x %02x\n", bitstream[i * 3] >> 6, bitstream[i * 3 + 1] >> 5, bitstream[i * 3 + 2] >> 5);
          rgb_8bit color = (rgb_8bit) {
            .blue = bitstream[i * 3] >> 6,
            .green = bitstream[i * 3 + 1] >> 5,
            .red = bitstream[i * 3 + 2] >> 5
          };
          fprintf(stderr, "%02x\n", color);
          binstream_cursor[i] = *(uint8_t *) &color;
        }
      }
    }
    fwrite(binstream, sizeof (uint8_t), info.biHeight * info.biWidth, picbin);
    fclose(picbin);
    fclose(picture);
  }

  
}
