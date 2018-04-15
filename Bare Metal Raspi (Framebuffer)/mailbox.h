
#include <stdint.h>

static volatile uint32_t * const MB0_READ   = (uint32_t *) 0x2000B880;
static volatile uint32_t * const MB0_WRITE  = (uint32_t *) 0x2000B8A0;
static volatile uint32_t * const MB0_STATUS = (uint32_t *) 0x2000B898;

static const uint32_t MAIL_FULL   = 0x80000000;
static const uint32_t MAIL_EMPTY  = 0x40000000;

static const uint32_t BUFF_ADDR_MASK = 0x40000000; 

static const uint32_t CHANNEL_MASK = 0xF;

static const uint32_t COLOR_WHITE = 0x00FFFFFF;

typedef enum {
  POWER_MANAGEMENT,
  FRAMEBUFFER,
  UART,
  VCHIQ,
  LED,
  BUTTON,
  TOUCH,
  __reserved__,
  ARM_TO_VC,
  VC_TO_ARM
}MB0_CHANNEL;

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t vWidth;
  uint32_t vHeight;
  uint32_t pitch;
  uint32_t depth;
  uint32_t xOffset;
  uint32_t yOffset;
  uint32_t fb;
  uint32_t fbSize; 
} fb_info_t __attribute__((aligned(16)));

uint32_t readMB0(MB0_CHANNEL channel);
void writeMB0(fb_info_t * data, MB0_CHANNEL channel);

