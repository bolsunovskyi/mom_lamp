boolean brightDirection;

void buttonTick() {
  btn2.tick();
  touch.tick();
  
  /*if (touch.isSingle()) {
    Serial.println("single");
      if (ONflag) {
        ONflag = false;
        changePower();
      } else {
        ONflag = true;
        changePower();
      }
  }*/

  if (touch.isSingle()) {
    if (++currentMode >= MODE_AMOUNT) currentMode = 0;
    FastLED.setBrightness(modes[currentMode].brightness);
    loadingFlag = true;
    FastLED.clear();
    analogWrite(WHITE_PIN2, 0);
    delay(1);
  }

  if (touch.isDouble()) {
    currentMode = 18;
    FastLED.clear();
    delay(1);
  }
  
  if (ONflag && touch.isTriple()) {
    Serial.println("triple");
    
    if (--currentMode < 0) currentMode = MODE_AMOUNT - 1;
    FastLED.setBrightness(modes[currentMode].brightness);
    loadingFlag = true;
    FastLED.clear();
    delay(1);
  }

  
  if (btn2.isSingle()) {
    Serial.println("btn2 single");
    FastLED.clear();
    delay(500);
    digitalWrite(WHITE_PIN2, 1);
    delay(500);
    digitalWrite(WHITE_PIN2, 0);
    delay(500);
    digitalWrite(WHITE_PIN2, 1);
    delay(500);
    digitalWrite(WHITE_PIN2, 0);
    motionMode = !motionMode;
  }

  // вывод IP на лампу
  /*if (ONflag && touch.hasClicks()) {
    if (touch.getClicks() == 5) {
      resetString();
      while (!fillString(lampIP, CRGB::Green, true)) {
        delay(1);
        ESP.wdtFeed();   // пнуть собаку
        yield();  // ещё раз пнуть собаку
      }
    }
  }*/

//  if (ONflag && touch.isHolded()) {
//    brightDirection = !brightDirection;
//  }
//  if (ONflag && touch.isStep()) {
//    if (brightDirection) {
//      if (modes[currentMode].brightness < 10) modes[currentMode].brightness += 1;
//      else if (modes[currentMode].brightness < 250) modes[currentMode].brightness += 5;
//      else modes[currentMode].brightness = 255;
//    } else {
//      if (modes[currentMode].brightness > 15) modes[currentMode].brightness -= 5;
//      else if (modes[currentMode].brightness > 1) modes[currentMode].brightness -= 1;
//      else modes[currentMode].brightness = 1;
//    }
//    FastLED.setBrightness(modes[currentMode].brightness);
////    settChanged = true;
////    eepromTimer = millis();
////    sendSettings_flag = true;
//  }
}
