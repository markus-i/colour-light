#include <ClickEncoder.h>
#include <FastLED.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, encoderValue;
int16_t led, hue, saturation, value;
bool button_lock;
enum mode {
  mode_off,     // leds off
  mode_hue,     // turning the button changes hue
  mode_sat,     // turning the button changes saturation
  mode_val,     // turning the button changes value
  mode_disco    // turning the button changes colour change speed
};
mode currentMode;

// in the lithophane lamp, I use 4 strings of three LEDs each, easier wiring
#define NUM_LEDS 3
#define DATA_PIN_0 A4
#define DATA_PIN_1 A5
#define DATA_PIN_2 2
#define DATA_PIN_3 3
CRGB leds[NUM_LEDS];

void timerIsr() 
{
  encoder->service();
}


void setup() {
  // put your setup code here, to run once:
  // initialize serial port for console
  Serial.begin(9600);
  
  // initialize FastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN_0>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,500);
  FastLED.setBrightness(255);


  // initialize rotary encoder
  encoder = new ClickEncoder(A2, A1, A0);
  encoder->setAccelerationEnabled(true);

  // initialize timer for interrupt
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = -1;

  hue = 0;
  saturation = 0;
  value = 127;
  currentMode = mode_off;
  button_lock = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  ClickEncoder::Button b = encoder->getButton();
  switch (b)
  {
    case ClickEncoder::Open:      // do nothing
      break;
    case ClickEncoder::Held:      // reset to white
      if( !button_lock )
      {
        button_lock = true;
        Serial.print("Encoder held, ");
        saturation = 0;
        currentMode = mode_val;
        //if( currentMode == mode_sat )
        //{
        //  currentMode = mode_val;
        //  Serial.println("current mode = value.");
        //}
        //else
        //{
        //  currentMode = mode_sat;
        //  Serial.println("current mode = saturation.");
        //}
      }
      break;
    case ClickEncoder::Released:
      Serial.println("Encoder relesed.");
      button_lock = false;
      break;
    case ClickEncoder::Clicked:
      Serial.print("Encoder clicked, ");
      if( currentMode == mode_off )
      {
        currentMode = mode_val;
        Serial.println("current mode = value.");
      }
      else
      {
        currentMode = mode_off;
        Serial.println("current mode = off.");
      }
      break;
    case ClickEncoder::DoubleClicked:
      Serial.print("Encoder double clicked, ");
      if( currentMode == mode_hue )
      {
        currentMode = mode_val;
        Serial.println("current mode = value.");
      }
      else
      {
        currentMode = mode_hue;
        Serial.println("current mode = hue.");
      }
      break;

    default:
      break;
  }
  
  
  switch (currentMode)
  {
    case mode_off:
      set_leds(hue, saturation, 0);
      break;
    case mode_hue:
      hue += encoder->getValue();
      //Serial.print("Hue: ");
      //Serial.println(hue);
      if( hue > 255) hue = 0;
      if( hue < 0) hue = 255;
      if( saturation == 0) saturation = 255;
      set_leds(hue, saturation, value);
      break;
    case mode_sat:
      saturation += encoder->getValue();
      //Serial.print("Saturation: ");
      //Serial.println(saturation);
      if( saturation > 255) saturation = 255;
      if( saturation < 0) saturation = 0;
      set_leds(hue, saturation, value);
      break;
    case mode_val:
      value += encoder->getValue();
      //Serial.print("Value: ");
      //Serial.println(value);
      if( value > 255) value = 255;
      if( value < 0) value = 0;
      set_leds(hue, saturation, value);
      break;
    case mode_disco:
      break;
    default:
      Serial.println("This wasn't supposed to happen");
      currentMode = mode_off;
      break;
  }
 
  FastLED.show();
}

void set_leds( int16_t h, int16_t s, int16_t v )
{
  for( int i=0; i<NUM_LEDS; i++)
  {
    leds[i] = CHSV(h, s, v); 
  }
  
}
