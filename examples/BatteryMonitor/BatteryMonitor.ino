#include <Battery.h>

Battery battery(3400, 4600, A0, 3);

void setup() {
	Serial.begin(9600);
	while (!Serial);
	battery.begin();
}

void loop() {
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500);

	Serial.print("Battery voltage is ");
	Serial.print(battery.voltage());
	Serial.print(" (");
	Serial.print(battery.level());
	Serial.println(")");
}
