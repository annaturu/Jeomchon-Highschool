#include <Adafruit_NeoPixel.h>
#define redPIN 5
#define bluePIN 6
#define NUM_LIGHTS  2

Adafruit_NeoPixel redstrip = Adafruit_NeoPixel(NUM_LIGHTS, redPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel bluestrip = Adafruit_NeoPixel(NUM_LIGHTS, bluePIN, NEO_GRB + NEO_KHZ800);

void setup() {
  redstrip.begin();
  bluestrip.begin();
  redstrip.show(); // Initialize all pixels to 'off'
  bluestrip.show(); // Initialize all pixels to 'off'
}

void loop() {
    uint32_t low = redstrip.Color(0, 0, 0); 
    uint32_t red = redstrip.Color(255, 0, 0);
    uint32_t blue = bluestrip.Color(0, 0, 255);  
    
    // Turn them off
    for( int i = 0; i<NUM_LIGHTS; i++){
        redstrip.setPixelColor(i, red);
        redstrip.show();
        bluestrip.setPixelColor(i, low);
        bluestrip.show();      
    }   
    delay(5000);
    
    for( int i = 0; i<NUM_LIGHTS; i++){
        redstrip.setPixelColor(i, low);
        redstrip.show();
        bluestrip.setPixelColor(i, blue);
        bluestrip.show();      
    }   
     delay(5000);
}
