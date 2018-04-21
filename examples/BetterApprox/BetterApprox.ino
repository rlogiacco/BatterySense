#include <Battery.h>

Battery battery(3000, 4200, A0);
/**
 * 1 cell li-ion/li-poly battery wired to A0, continuous sensing, sigmoidal mapping function, cut off at 3000mV
 * https://github.com/rlogiacco/BatterySense#lesser-than-5v-with-voltage-booster
 **/
void setup() {
	Serial.begin(9600);
	while (!Serial);
	battery.begin(5000, 1.0, &sigmoidal);
}

void loop() {
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500);

	Serial.print("Battery voltage is ");
	Serial.print(battery.voltage());
	Serial.print("mV (");
	Serial.print(battery.level());
	Serial.println("%)");
}
