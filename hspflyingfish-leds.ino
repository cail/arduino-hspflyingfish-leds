// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

/////////////////////////
// Arduino NANO
/////////////////////////

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN  5

#define PINBUTTON  8
#define PINSPEED  2

#define ANALOG_PIN A5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELSF      6

#define NUMPIXELS_B      4

#define STATES 10
int g_state = 0;

Adafruit_NeoPixel pixels_f = Adafruit_NeoPixel(NUMPIXELSF, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 10;
int tick = 0;

void setup() {
  Serial.begin(9600);
  pixels_f.begin();

  pinMode(PINBUTTON, INPUT);  
  pinMode(PINSPEED, INPUT);
}

int strobe_500ms = 1;

int turns = 0;

int police_lights = 1;

int button_clicked = 0;

int s_speed = 0;

void hsp_fish_loop()
{
  
  int back_activity = 0;
  int front_activity = 100;
  int scaled_back = 0;


  Serial.print("s_speed "); Serial.println(s_speed);
  scaled_back = -s_speed*(255-back_activity)/100;
  if (s_speed < 7 && s_speed > -7) {
    back_activity = 255;
  } else {
    front_activity += s_speed * (255-front_activity) / 100;
  }
  
  for(int i=0; i<NUMPIXELS_B; i++)
  {
    pixels_f.setPixelColor(i, pixels_f.Color(back_activity, 0, 0));
  }

  if (s_speed < 0) {
    pixels_f.setPixelColor(0, pixels_f.Color(scaled_back, scaled_back, scaled_back));
    pixels_f.setPixelColor(1, pixels_f.Color(0, 0, 0));
    pixels_f.setPixelColor(2, pixels_f.Color(0, 0, 0));
    pixels_f.setPixelColor(3, pixels_f.Color(scaled_back, scaled_back, scaled_back));    
  }

  pixels_f.setPixelColor(4, pixels_f.Color(front_activity,front_activity,front_activity));
  pixels_f.setPixelColor(5, pixels_f.Color(front_activity,front_activity,front_activity));

  pixels_f.show();
}

void allwhite_loop()
{
  
}

int svoltage = 0;

void update_button()
{
  static int analogReads = 0;
  static uint32_t analog_accum = 0;
  static int button_latch = 0;

  int aval = analogRead(ANALOG_PIN);
  analog_accum = analog_accum + aval;
  analogReads = analogReads+1;
  if (analogReads > 10) {
    uint32_t button_voltage = (analog_accum / analogReads) * 5; 
    Serial.print("Voltage: ");
    Serial.println(aval);
    //Serial.println(button_voltage);
    svoltage = aval * 5000 / 1023;
    analogReads = 0;
    analog_accum = 0;

    if (button_voltage > 4000 && !button_latch) {
      button_clicked = 1;
      button_latch = 1;
      Serial.println("Button clicked");
    }
    if (button_voltage < 4000 && button_latch) {
      button_latch = 0;
      button_clicked = 0;
    }

    int vv = button_voltage / 100;
    
    for(int i=0; i<NUMPIXELSF; i++)
    {
      if (i < vv)
        pixels_f.setPixelColor(i, pixels_f.Color(255,255,255));
      else
        pixels_f.setPixelColor(i, pixels_f.Color(0,0,0));
    }
    //pixels_f.show();
  }  

  int buttonState = digitalRead(PINBUTTON);
  /*
  if (buttonState == HIGH) {
      Serial.println("Button2 clicked");    
  }else
      Serial.println("Button2 clicked LOW");    
  }*/
}


void update_speed()
{
  int ch1 = pulseIn(PINSPEED, HIGH, 25000);
  if (ch1 != 0)
  {  
    ch1 -= 1506;
    ch1 = ch1*100/600;
    s_speed = ch1;
    Serial.print("Channel 1:");
    Serial.println(ch1);
  }
}

void loop() {

  if (g_state == 0) {
    hsp_fish_loop();
  } else if (g_state == 1) {
    allwhite_loop();
  } else {
    
  }

  if ((0 == tick % (500/delayval))) {
     strobe_500ms = !strobe_500ms;
  }

  //update_button();

  update_speed();

  //int aval = analogRead(ANALOG_PIN);
  //Serial.print("Analog read: ");
  //Serial.println(aval);

//  if (button_clicked)
//    g_state = (g_state+1) % STATES;

  delay(delayval);
  tick++;
}
