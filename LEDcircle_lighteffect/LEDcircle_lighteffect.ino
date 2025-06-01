#include <Adafruit_NeoPixel.h>

#define LED_PIN    D2
#define LED_COUNT  59

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// 光效控制参数
float baseHue = 160.0;        // 青蓝色基调
float hueRange = 20.0;        // 色相循环范围
float huePhase = 0.0;         // 控制整体色相变化
float hueSpeed = 0.01;        // 整体颜色变慢些

int flowOffset = 0;           // 流水灯偏移量
int flowSpeed = 1;            // 控制流水移动速度

float breathPhase = 0.0;      // 呼吸节奏
float breathSpeed = 0.03;     // 呼吸速度（越小越慢）

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(255);  // 我们手动控制亮度，不靠这个参数
}

void loop() {
  // 1. 呼吸亮度 (0~255)
  uint8_t brightness = (sin(breathPhase) + 1.0) * 0.5 * 255;

  // 2. 动态 hue（整条灯带的主色在循环）
  float dynamicHue = baseHue + sin(huePhase) * hueRange;

  // 3. 每颗灯做出“流动”的 hue 偏移
  for (int i = 0; i < LED_COUNT; i++) {
    float pixelHue = dynamicHue + (i + flowOffset) * 2;  // 2 控制颜色过渡紧密程度
    uint16_t hue16 = (uint16_t)((int(pixelHue) % 360) * 182);  // 转成 0-65535

    uint32_t color = strip.gamma32(strip.ColorHSV(hue16, 255, brightness));
    strip.setPixelColor(i, color);
  }

  strip.show();

  // 更新 phase 和偏移
  huePhase += hueSpeed;
  if (huePhase > TWO_PI) huePhase -= TWO_PI;

  breathPhase += breathSpeed;
  if (breathPhase > TWO_PI) breathPhase -= TWO_PI;

  flowOffset += flowSpeed;

  delay(40);  // 控制整体速度
}

