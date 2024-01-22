#include <Battery.h>

Battery battery(3400, 4200, A0, 12);
/**
 * 3.3V board with a 12 bits ADC using a 1 cell li-ion/li-poly battery, voltage divider using 470 and 1k resistors sensed by A0, on demand sensing on pin D3, asigmoidal mapping function
 * Circuit available at https://www.falstad.com/circuit/circuitjs.html?ctz=CQAgjCAMB0l3BWcMBMcUHYMGZIA4UA2ATmIxAUgpABZsKBTAWjDACgAnETFEbBXj24JCUZHDYA3cMV79eYWSDxVVtKjWjk1MBJ24YFhKkJQixNDJDYB3A3IF9so+VFszeK+8qrs7Qr2xnH253U3Mg0TMotjRyRU9fYxCaEAAhAEEAFQAdAAdYyHilVzBk11TMrKZC8kiQ5zwUkAA1AEsAO1qnF0dGvkdUgHEAOQARbvrovkIm6dSM6wAzHvAaVKEwdadwaCQYSBQwwwGFEsdrABlabCMNLzKdECWAQwAbAGcGPmh6a38Tl5NmBeNYuMC5MFWKDuGgJAAXGZNR6rFG+EBMTQkUikLAEGjwYi7SAIBA44gIMCQXE0Yh4ZjIuAgAAmDFeAFc3vC2ABzVEYFxQpSqXmrVz1foiuz1aGo7b-bwguYnWVgpHgZJ0O5iFBwtgAezEhFSGmpRP2sEghDMOrE2DY2CaVAAYhB0VSICwQAAlBgfNofeEvDoAYwYbCAA
 **/

#define R1 470
#define R2 1000
#define SENSE_PIN 3

void setup() {
	Serial.begin(9600);
	pinMode(SENSE_PIN, INPUT);
	analogReadResolution(12); // switches ADC resolution to 12 bits
	while (!Serial);
	battery.begin(3300, (R1 + R2) / R2, &asigmoidal);
	battery.onDemand(SENSE_PIN, HIGH);
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
	Serial.println("%)");
}
