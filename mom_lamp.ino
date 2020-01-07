#define BRIGHTNESS 100         // стандартная маскимальная яркость (0-255)
//#define CURRENT_LIMIT 2000    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define CURRENT_LIMIT 0    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 4              // ширина матрицы
#define HEIGHT 8             // высота матрицы

#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 0     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/

#define LED_PIN 4     
#define BTN_PIN 2
#define BTN2_PIN 3
#define POT_PIN A2
#define MODE_AMOUNT 19

#define NUM_LEDS WIDTH * HEIGHT
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
// ---------------- БИБЛИОТЕКИ -----------------
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0

#define WHITE_PIN1 5
#define WHITE_PIN2 6
#define MOT_PIN 7
#define MOT_BTN_PIN 3

//#include "timer2Minim.h"
#include <FastLED.h>
#include <GyverButton.h>

CRGB leds[NUM_LEDS];

boolean loadingFlag = true;
unsigned char matrixValue[8][16];
boolean ONflag = true;
int8_t currentMode = 0;
//boolean manualOff = false;
//boolean sendSettings_flag = false;
//boolean settChanged = false;
//uint32_t eepromTimer;
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);
GButton btn2(MOT_BTN_PIN, LOW_PULL, NORM_OPEN);

//static const byte maxDim = max(WIDTH, HEIGHT);
struct {
  byte brightness = 50;
  byte speed = 30;
  byte scale = 40;
} modes[MODE_AMOUNT];

int brightness = BRIGHTNESS;
boolean motionMode = false;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.show();
  touch.setStepTimeout(100);
  touch.setClickTimeout(500);

  btn2.setStepTimeout(100);
  btn2.setClickTimeout(500);
  pinMode(WHITE_PIN1, OUTPUT);
  pinMode(WHITE_PIN2, OUTPUT);
  pinMode(MOT_PIN, INPUT);

  digitalWrite(WHITE_PIN1, 0);
  digitalWrite(WHITE_PIN2, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  effectsTick();
  buttonTick();
  int newBrighness = map(analogRead(POT_PIN), 0, 1023, 0, 255);
  if (abs(brightness - newBrighness) > 3)  {
    brightness = newBrighness;
    FastLED.setBrightness(newBrighness);
  }

  if(motionMode) {
    int motion = digitalRead(MOT_PIN);
    Serial.print("motion: "); Serial.println(motion);
    
    if (ONflag && !motion) {
      ONflag = false;
      changePower();
    } else if(!ONflag && motion) {
      ONflag = true;
      changePower();
    }
  }

//  Serial.println(digitalRead(MOT_BTN_PIN));
//  Serial.println(digitalRead(BTN_PIN));
}
