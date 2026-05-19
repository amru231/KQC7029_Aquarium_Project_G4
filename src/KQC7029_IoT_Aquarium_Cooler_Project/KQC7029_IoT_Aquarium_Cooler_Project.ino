/************************************************************
   AQUARIUM COOLING SYSTEM WITH BLYNK
   ESP32 DEVKIT V1
************************************************************/

// =========================
// BLYNK SETTINGS
// =========================
#define BLYNK_TEMPLATE_ID "INSERT_BLYNK_ID"
#define BLYNK_TEMPLATE_NAME "INSERT_BLYNK_NAME"
#define BLYNK_AUTH_TOKEN "INSERT_BLYNK_AUTH_TOKEN"

// =========================
// WIFI + BLYNK LIBRARIES
// =========================
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// =========================
// SENSOR + LCD LIBRARIES
// =========================
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =========================
// WIFI CREDENTIALS
// =========================
char ssid[] = "INSERT_WIFI_SSID";
char pass[] = "INSERT_WIFI_PASS";

// =========================
// PIN DEFINITIONS
// =========================
#define ONE_WIRE_BUS 32

#define RELAY_PIN 27
#define BUZZER_PIN 14

#define GREEN_LED 26
#define YELLOW_LED 25
#define RED_LED 33

// =========================
// TEMPERATURE SETTINGS
// =========================
float FAN_ON_TEMP = 27.0;
float FAN_OFF_TEMP = 24.0;

// =========================
// DS18B20 SETUP
// =========================
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// =========================
// LCD SETUP
// =========================
LiquidCrystal_I2C lcd(0x27, 20, 4);

// =========================
// VARIABLES
// =========================
bool fanState = false;

bool manualMode = false;
bool manualFan = false;

float tempC = 0;

// =========================
// TIMER
// =========================
BlynkTimer timer;

// =====================================================
// BLYNK FUNCTIONS
// =====================================================

// V2 = MANUAL FAN CONTROL
BLYNK_WRITE(V2)
{
  manualFan = param.asInt();
}

// V3 = AUTO / MANUAL MODE
BLYNK_WRITE(V3)
{
  manualMode = param.asInt();
}

BLYNK_WRITE(V4)
{
  // V4 is ONLY display, so we don't need to read it
}

// =====================================================
// READ SENSOR + UPDATE SYSTEM
// =====================================================
void updateSystem()
{
  // =========================
  // READ TEMPERATURE
  // =========================
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.println(tempC);

  // =========================
  // SEND DATA TO BLYNK
  // =========================
  Blynk.virtualWrite(V0, tempC);
  Blynk.virtualWrite(V1, fanState);
  if (fanState)
      Blynk.virtualWrite(V4, "Fan is ON");
  else
      Blynk.virtualWrite(V4, "Fan is OFF");

  // =========================
  // LCD DISPLAY
  // =========================
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Aquarium Cooler");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print((char)223);
  lcd.print("C");

  // =========================
  // AUTO MODE
  // =========================
  if (manualMode == 0)
  {
    lcd.setCursor(0, 3);
    lcd.print("Mode: AUTO");

    // TURN FAN ON
    if (tempC >= FAN_ON_TEMP && fanState == false)
    {
      digitalWrite(GREEN_LED, LOW);

      // WARNING BUZZER + RED LED
      for (int i = 0; i < 5; i++)
      {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);

        delay(300);

        digitalWrite(RED_LED, LOW);
        digitalWrite(BUZZER_PIN, LOW);

        delay(300);
      }

      // FAN ON
      digitalWrite(RELAY_PIN, LOW);

      fanState = true;

      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(RED_LED, LOW);
    }

    // TURN FAN OFF
    if (tempC <= FAN_OFF_TEMP && fanState == true)
    {
      digitalWrite(RELAY_PIN, HIGH);

      fanState = false;

      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, LOW);
    }
  }

  // =========================
  // MANUAL MODE
  // =========================
  else
  {
    lcd.setCursor(0, 3);
    lcd.print("Mode: MANUAL");

    if (manualFan == 1)
    {
      digitalWrite(RELAY_PIN, LOW);

      fanState = true;

      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(RED_LED, LOW);
    }
    else
    {
      digitalWrite(RELAY_PIN, HIGH);

      fanState = false;

      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, LOW);
    }
  }

  // =========================
  // FAN STATUS LCD
  // =========================
  lcd.setCursor(0, 2);

  if (fanState)
  {
    lcd.print("Fan : ON ");
  }
  else
  {
    lcd.print("Fan : OFF");
  }
}

// =====================================================
// SETUP
// =====================================================
void setup()
{
  Serial.begin(115200);

  // =========================
  // SENSOR START
  // =========================
  sensors.begin();

  // =========================
  // LCD START
  // =========================
  Wire.begin(21, 22);

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  // =========================
  // PIN MODES
  // =========================
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // =========================
  // INITIAL STATES
  // =========================
  digitalWrite(RELAY_PIN, HIGH);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // =========================
  // CONNECT BLYNK
  // =========================
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // =========================
  // TIMER
  // =========================
  timer.setInterval(1000L, updateSystem);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");

  delay(1000);
}

// =====================================================
// MAIN LOOP
// =====================================================
void loop()
{
  Blynk.run();
  timer.run();
}
