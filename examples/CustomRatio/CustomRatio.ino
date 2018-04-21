#include <Battery.h>

Battery battery(7200, 8400, A0);
/**
 * 2 cells (2S1P) li-ion/li-poly battery wired to A0, continuous sensing
 * https://github.com/rlogiacco/BatterySense#higher-than-5v-with-internal-voltage-regulator
 *
 *   +--------+
 *   |        |
 *   |       +-+
 *   |       | | 33k
 *   |       | |
 * -----     +-+
 *  ---       |---------+ A0
 *   |       +-+
 *   |       | |
 *   |       | | 10k
 *   |       +-+
 *   |        |
 *   +--------+---------+ GND
 *
 **/

void setup() {
	Serial.begin(9600);
	while (!Serial);
	float ratio = (33 + 10) / 10;
	// 5V board, change to 3300 for a 3.3V board
	battery.begin(5000, ratio);
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
