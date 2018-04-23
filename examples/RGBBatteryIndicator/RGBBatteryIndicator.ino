#include <Battery.h>

// 9V alkaline battery
Battery battery(5000, 9000, A0);

#define RED 5
#define GREEN 6
#define BLUE 7

void setup() {
	Serial.begin(9600);
	while (!Serial);
	battery.begin(5000, 2.0, &sigmoidal);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}
long ms = 0;
bool on = true;

void loop() {
    long v = battery.voltage();
    if (battery.level(v) >= 75) {
        // blue: everything is fine
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, HIGH);
    } else if (battery.level(v) >= 50) {
        // green: battery is ok, but not full
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, HIGH);
        digitalWrite(BLUE, LOW);
    } else if (battery.level(v) >= 20) {
        // yellow: battery is getting low
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, HIGH);
        digitalWrite(BLUE, LOW);
    } else if (battery.level(v) >= 5) {
        // red: time to change the battery
        digitalWrite(RED, HIGH);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, LOW);
    } else {
        // blinking red: level is critical
        digitalWrite(RED, on);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, LOW);
    }
    if (millis() - ms > 250) {
        Serial.print("Battery voltage is ");
        Serial.print(v);
        Serial.print(" (");
        Serial.print(battery.level(v));
        Serial.println("%)");
        ms = millis();
        on = !on;
    }
}
