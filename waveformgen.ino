#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
const int potPin = 34;
const int buttonPin = 27;


// Waveform state and debounce vars 
enum WaveformType {SINE, SQUARE, TRIANGLE, NUM_WAVEFORMS};
WaveformType waveformcurrent = SINE;

bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

//freq 
float frequency = 1000;
const int waveformResolution = 80; //chnaged from 100 to 80 

/*

uint8_t sineTable[waveformResolution];

// Plot buffer for waveform display
#define PLOT_HEIGHT 30
#define PLOT_OFFSET_Y 33
uint8_t plotBuffer[SCREEN_WIDTH];

void generateSineTable() {
  for (int i = 0; i < waveformResolution; i++) {
    float angle = 2.0 * PI * i / waveformResolution;
    sineTable[i] = (uint8_t)((sin(angle) * 0.5 + 0.5) * 255);
  }
}*/


void drawWaveform(int cycles =1){ //cycles is used to control waves, cycle 10 is stretched out 
  display.clearDisplay();

  //show waveform deets
  display.setCursor(0, 0);
  display.print("Type: ");
  switch (waveformcurrent) {
    case SINE:     display.print("SINE "); break;
    case SQUARE:   display.print("SQUARE "); break;
    case TRIANGLE: display.print("TRIANGLE "); break;
    
  }
  display.setCursor(0, 10);
  display.print("Freq: ");
  display.println(cycles);
  display.print(" Hz");

  //waveform drawing
  for (int x=0; x<SCREEN_WIDTH; x++){
    float phase = (float)x / SCREEN_WIDTH *cycles * 2 * PI;
    int y;
    switch (waveformcurrent) {
      case SINE:
        y = 50 + (int)(15 * sin(phase));
        break;

      case SQUARE:
        // Calculate all points first, then draw lines
        for (int x = 0; x < SCREEN_WIDTH - 1; x++) {
            float phase1 = (float)x / SCREEN_WIDTH * cycles * 2 * PI;
            float phase2 = (float)(x + 1) / SCREEN_WIDTH * cycles * 2 * PI;
            
            int y1 = (fmod(phase1, 2 * PI) < PI) ? 35 : 50;  // Adjusted heights
            int y2 = (fmod(phase2, 2 * PI) < PI) ? 35 : 50;
            
            display.drawLine(x, y1, x + 1, y2, SSD1306_WHITE);
          }
          break;

      case TRIANGLE:
        y = 50 + (int)(15 * (2.0 / PI) * asin(sin(phase))); // triangle via asin(sin())
        break;
    }
    display.drawPixel(x,y,SSD1306_WHITE);
  }
  display.display();
}


void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  analogReadResolution(12);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  //drawWaveform();
  //generateSineTable(); //commented thi sout to see what happens 
}

void loop() {
  // Handle button with debounce
   static unsigned long lastPrint = 0;
  
  // Simple button check without debouncing first
  int reading = digitalRead(buttonPin);
  if (reading == LOW) {  // Button pressed
    waveformcurrent = (WaveformType)((waveformcurrent + 1) % 3);
    Serial.print("Button pressed! New waveform: ");
    Serial.println(waveformcurrent);
    delay(300); // Simple delay instead of debouncing
  }
  
 

  // Read frequency from potentiometer
  int potValue = analogRead(potPin);
  frequency = map(potValue, 0, 4095, 100, 1000); // 10Hz to 1kHz

  drawWaveform(frequency);
  delay(100);
}


/*
  // DAC output
  static int index = 0;
  static unsigned long lastUpdate = 0;
  unsigned long interval = 1000000UL / (waveformResolution * frequency); // μs

  if (micros() - lastUpdate >= interval) {
    lastUpdate = micros();
    uint8_t value = 0;
    switch (waveform) {
      case SINE:
        value = sineTable[index];
        break;
      case SQUARE:
        value = (index < waveformResolution / 2) ? 255 : 0;
        break;
      case TRIANGLE:
        value = (index < waveformResolution / 2) ?
                map(index, 0, waveformResolution / 2, 0, 255) :
                map(index, waveformResolution / 2, waveformResolution, 255, 0);
        break;
    }

    dacWrite(dacPin, value);
    updatePlot(value);
    drawUI();
    index = (index + 1) % waveformResolution;
  }
}

void updatePlot(uint8_t val) {
  // Shift old values left
  for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
    plotBuffer[i] = plotBuffer[i + 1];
  }

  // Scale DAC value (0–255) to OLED range (PLOT_HEIGHT pixels)
  plotBuffer[SCREEN_WIDTH - 1] = map(val, 0, 255, 0, PLOT_HEIGHT - 1);
}

void drawUI() {
  display.clearDisplay();

  // Draw text
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Type: ");
  display.print(getWaveformName());

  display.setCursor(0, 10);
  display.print("Freq: ");
  display.print((int)frequency);
  display.print(" Hz");

  // Draw waveform graph
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    int y = PLOT_OFFSET_Y + (PLOT_HEIGHT - 1 - plotBuffer[x]);
    display.drawPixel(x, y, SSD1306_WHITE);
  }

  display.display();
}

const char* getWaveformName() {
  switch (waveform) {
    case SINE: return "SINE";
    case SQUARE: return "SQUARE";
    case TRIANGLE: return "TRIANGLE";
    default: return "UNKNOWN";
  }
}
*/
