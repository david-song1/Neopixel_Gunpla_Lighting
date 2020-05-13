//importing required libraries
#include <Adafruit_NeoPixel.h>
//#include <SoftwareSerial.h> for bluetooth, if needed later

//
#define BUTTON_PIN 2
//This is where you would define what pin each LED strip is connected to
const int LED_PIN_TORSO_HEAD = 6;
const int LED_PIN_L_ARM = 7;
const int LED_PIN_R_ARM = 8;
const int LED_PIN_L_LEG = 9;
const int LED_PIN_R_LEG = 10;

//This is where you would define how many LEDs are connected to the strip
const int LED_COUNT_TORSO_HEAD = 14;
const int LED_COUNT_L_ARM = 5;
const int LED_COUNT_R_ARM = 5;
const int LED_COUNT_L_LEG = 3;
const int LED_COUNT_R_LEG = 3;

//This allows easier manipulation of the LEDs in each individual limb
const int NUM_STRIPS = 5;
const int LED_INDEX[3][14] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, {0, 1, 2, 3, 4}, {0, 1, 2}};
//[0][0-3] = engine?? lights
//[0][4-8] = chest lights
//[0][9-13] = head lights

//This is where you would instantiate eache strip as an object, I put everything in an array as it is
//much easier to access sequentially.
//INDEX 0 = Torso and Head LED strip
//INDEX 1 = Left arm LED strip
//INDEX 2 = Right arm LED strip
//INDEX 3 = Left leg LED strip
//INDEX 4 = Right leg LED strip
Adafruit_NeoPixel ledStrip[] = {(LED_COUNT_TORSO_HEAD, LED_PIN_TORSO_HEAD, NEO_GRB + NEO_KHZ800),
                                (LED_COUNT_L_ARM, LED_PIN_L_ARM, NEO_GRB + NEO_KHZ800),
                                (LED_COUNT_R_ARM, LED_PIN_R_ARM, NEO_GRB + NEO_KHZ800),
                                (LED_COUNT_L_LEG, LED_PIN_L_LEG, NEO_GRB + NEO_KHZ800),
                                (LED_COUNT_R_LEG, LED_PIN_R_LEG, NEO_GRB + NEO_KHZ800)
                               };

bool buttonPress = false;

int currentMode = -1;

void setup() {
  //start serial communication
  Serial.begin(9600); 
  
  // intialize all the LED strips
  for (int i = 0; i < NUM_STRIPS; i++) {
    ledStrip[i].begin();
    ledStrip[i].setBrightness(35);
  }
  showAll();
  Serial.println("Gundam Exia System Ready..."); //output welcome message
}

//this will run as long as the board is on
void loop() {
  if (currentMode < 0) {
    GN_Start(5, 10);
    currentMode = 1;
  }
  else if (currentMode = 1) {
    GN_running(false, 15);
    currentMode = 1;
  }
  else if (currentMode = 2) {
    GN_running(true, 15);
    currentMode = 2;
  }
}

void showAll() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    ledStrip[i].show();
  }
}

void clearAll() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    ledStrip[i].fill();
  }
  showAll();
}


// GN drive startup aninmation
void GN_Start(int fadeSpeed, int intervalTime) {
  uint32_t rgb = ledStrip[0].ColorHSV(65536 / 3, 255, 0); //initialize HSV color value (green)
  //Loop thru Saturation to create a slow spin up effect
  for (int i = 0; i < 255; i++) {
    rgb = ledStrip[0].ColorHSV(65536 / 3, 255, i);
    for (int j = 0; j < 8; j++) {
      ledStrip[0].setPixelColor(LED_INDEX[0][j], rgb);
    }
    showAll();
    delay(fadeSpeed);
  }

  //fade in the eye color after a short delay
  delay(intervalTime);

  for (int i = 0; i < 255; i++) {
    rgb = ledStrip[0].ColorHSV(65536 / 5, 255, i);
    ledStrip[0].setPixelColor(LED_INDEX[0][8], rgb);
    ledStrip[0].setPixelColor(LED_INDEX[0][9], rgb);
    showAll();
    delay(fadeSpeed);
  }

  //fade in the side and sensor led colors
  delay(intervalTime);

  for (int i = 0; i < 255; i++) {
    rgb = ledStrip[0].ColorHSV(65536 / 3, 255, i);
    ledStrip[0].setPixelColor(LED_INDEX[0][10], rgb);
    ledStrip[0].setPixelColor(LED_INDEX[0][11], rgb);
    ledStrip[0].setPixelColor(LED_INDEX[0][12], rgb);
    showAll();
    delay(fadeSpeed);
  }
}

//running mode of the suit. has a slow breathing animation and able to switch to trans am mode
void GN_running(bool trans_am, int fadeSpeed) {
  uint32_t rgb;
  int suitColor;

  //activates TransAm mode if argument passed in is true
  if (trans_am == true) {
    suitColor = 65536 / 20;
    rgb = headTorso.ColorHSV(65536 / 6, 255, 150);
    for (int j = 0; j < 8; j++) {
      headTorso.setPixelColor(LED_INDEX[0][j], rgb);

    }
    showAll();
  } else {
    suitColor = 65536 * 2 / 3;
  }

  //fade in arm and legs
  for (int i = 25; i < 255 / 2; i++) {
    rgb = rArm.ColorHSV(suitColor, 255, i);
    for (int j = 0; j < LED_COUNT_R_ARM; j++) {
      rArm.setPixelColor(LED_INDEX[1][j], rgb);
      lArm.setPixelColor(LED_INDEX[1][j], rgb);
      rLeg.setPixelColor(LED_INDEX[2][j], rgb);
      lLeg.setPixelColor(LED_INDEX[2][j], rgb);
    }
    showAll();
    delay(fadeSpeed);
  }
  //pulse arm and leg LED strips to create a breathing effect
  for (int i = 255 / 2; i > 25; i--) {
    rgb = rArm.ColorHSV(suitColor, 255, i);
    for (int j = 0; j < LED_COUNT_R_ARM; j++) {
      rArm.setPixelColor(LED_INDEX[1][j], rgb);
      lArm.setPixelColor(LED_INDEX[1][j], rgb);
      rLeg.setPixelColor(LED_INDEX[2][j], rgb);
      lLeg.setPixelColor(LED_INDEX[2][j], rgb);
    }
    showAll();
    delay(fadeSpeed);
  }
}

////Suit power down animation
//void powerDown() {
//  //fade to an orange glow, before shutting off
//
//}
//
//void rainbow1(int wait) {
//
//  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
//    for (int i = 0; i < rArm.numPixels(); i++) { // For each pixel in strip...
//      int pixelHue = firstPixelHue + (i * 65536L / rArm.numPixels());
//      strip.setPixelColor(i, rArm.gamma32(strip.ColorHSV(pixelHue)));
//    }
//    for (int i = 0; i < rArm.numPixels(); i++) { // For each pixel in strip...
//      int pixelHue = firstPixelHue + (i * 65536L / rArm.numPixels());
//      strip.setPixelColor(i, rArm.gamma32(strip.ColorHSV(pixelHue)));
//    }
//    for (int i = 0; i < rArm.numPixels(); i++) { // For each pixel in strip...
//      int pixelHue = firstPixelHue + (i * 65536L / rArm.numPixels());
//      strip.setPixelColor(i, rArm.gamma32(strip.ColorHSV(pixelHue)));
//    }
//    for (int i = 0; i < rArm.numPixels(); i++) { // For each pixel in strip...
//      int pixelHue = firstPixelHue + (i * 65536L / rArm.numPixels());
//      strip.setPixelColor(i, rArm.gamma32(strip.ColorHSV(pixelHue)));
//    }
//    for (int i = 0; i < rArm.numPixels(); i++) { // For each pixel in strip...
//      int pixelHue = firstPixelHue + (i * 65536L / rArm.numPixels());
//      strip.setPixelColor(i, rArm.gamma32(strip.ColorHSV(pixelHue)));
//    }
//    showAll(); // Update strip with new contents
//    delay(wait);  // Pause for a moment
//  }
//}
