#include <Battery.h>

Battery battery(6000, 8400, A0);
/**
 * 2 cells li-ion/li-poly battery wired to A0, on demand P-CH sensing on pin 3, sigmoidal mapping function
 * https://github.com/rlogiacco/BatterySense#higher-than-5v-activated-on-demand
 **/
void setup() {
	Serial.begin(9600);
	while (!Serial);
	battery.onDemand(3, HIGH);
	battery.begin(5000, 2.0, &sigmoidal);
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
