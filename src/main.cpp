#include <Arduino.h>
#include <FastLED.h>

#include <string>

// Variables
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LED_COUNT 5
#define STAIRS_COUNT 13
#define BRIGHTNESS 25   // 255 Max
#define COLOR "FAEBD7"  // HEX Code
#define DELAY_LEDS 10   // in MS
#define DELAY_STAIRS 75 // in MS
#define STAY_ON 5       // in S

// PINS
#define PIR_PIN_UP_STAIRS D6
#define PIR_PIN_DOWN_STAIRS D5
#define LED_PIN_1 4
#define LED_PIN_2 3

CRGB leds[LED_COUNT * STAIRS_COUNT];
int R;
int G;
int B;
boolean inAnimation = false;

int convert(const char num[])
{
  int len = strlen(num);
  int base = 1;
  int temp = 0;
  for (int i = len - 1; i >= 0; i--)
  {
    if (num[i] >= '0' && num[i] <= '9')
    {
      temp += (num[i] - 48) * base;
      base = base * 16;
    }
    else if (num[i] >= 'A' && num[i] <= 'F')
    {
      temp += (num[i] - 55) * base;
      base = base * 16;
    }
  }
  return temp;
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIR_PIN_UP_STAIRS, INPUT);
  pinMode(PIR_PIN_DOWN_STAIRS, INPUT);

  LEDS.addLeds<LED_TYPE, LED_PIN_1, COLOR_ORDER>(leds, LED_COUNT * STAIRS_COUNT);
  LEDS.addLeds<LED_TYPE, LED_PIN_2, COLOR_ORDER>(leds, LED_COUNT * STAIRS_COUNT);
  LEDS.setBrightness(BRIGHTNESS);

  std::string color = COLOR;
  std::string r = color.substr(0, 2);
  R = convert(r.c_str());
  std::string g = color.substr(2, 2);
  G = convert(g.c_str());
  std::string b = color.substr(4, 2);
  B = convert(b.c_str());

  Serial.println((String) "R:" + R);
  Serial.println((String) "G:" + G);
  Serial.println((String) "B:" + B);
}

void goingUp(int r, int g, int b)
{
  for (int i = 0; i < STAIRS_COUNT; i++)
  {
    for (int j = 0; j < LED_COUNT; j++)
    {
      int currentLedPos = i * LED_COUNT + j;
      leds[currentLedPos] = CRGB(r, g, b);
      LEDS.show();
      FastLED.delay(DELAY_LEDS);
    }
    FastLED.delay(DELAY_STAIRS);
  }
}

void goingDown(int r, int g, int b)
{
  for (int i = STAIRS_COUNT - 1; i >= 0; i--)
  {
    for (int j = LED_COUNT - 1; j >= 0; j--)
    {
      int currentLedPos = i * LED_COUNT + j;
      leds[currentLedPos] = CRGB(r, g, b);
      LEDS.show();
      FastLED.delay(DELAY_LEDS);
    }
    FastLED.delay(DELAY_STAIRS);
  }
}

void ledAnimationGoingUp()
{
  goingUp(R, G, B);
  FastLED.delay(STAY_ON * 1000);
  goingUp(0, 0, 0);
}

void ledAnimationGoingDown()
{
  goingDown(R, G, B);
  FastLED.delay(STAY_ON * 1000);
  goingDown(0, 0, 0);
}

void loop()
{
  // Upstairs Motion
  if (digitalRead(PIR_PIN_UP_STAIRS) == HIGH && inAnimation == false)
  {
    inAnimation = true;
    Serial.println("Upstairs Motion Detected");
    ledAnimationGoingDown();
    inAnimation = false;
  }

  // Downstairs Motion
  if (digitalRead(PIR_PIN_DOWN_STAIRS) == HIGH && inAnimation == false)
  {
    inAnimation = true;
    Serial.println("Downstairs Motion Detected");
    ledAnimationGoingUp();
    inAnimation = false;
  }
}
