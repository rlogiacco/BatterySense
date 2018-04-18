/*
 Battery.h - Battery library
 Copyright (c) 2014 Roberto Lo Giacco.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as 
 published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BATTERY_
#define _BATTERY_

#include <Arduino.h>
class Battery {
	public:
		/**
		 * Creates an instance to monitor battery voltage and level.
		 * Initialization parameters depend on battery type and configuration.
		 *
		 * @param minVoltage is the voltage, expressed in millivolts, corresponding to an empty battery
		 * @param maxVoltage is the voltage, expressed in millivolts, corresponding to a full battery
		 * @param sensePin is the analog pin used for sensing the battery voltage
		 * @param activationPin is the optional digital pin which will be turned HIGH or LOW before starting the battery sensing
		 * @param activationMode is the optional value to set on the activationPin to enable battery sensing, defaults to LOW
		 *            useful when using a resistor divider to save on battery consumption, but it can be changed to HIGH in case
		 *            you are using a P-CH MOSFET or a PNP BJT
		 */
		Battery(uint16_t minVoltage, uint16_t maxVoltage, uint8_t sensePin, uint8_t activationPin = 0xFF, uint8_t activationMode = LOW);

		/**
		 * Initializes the library by optionally setting additional parameters.
		 * To obtain the best results use a calibrated reference using the VoltageReference library or equivalent.
		 * 
		 * @param refVoltage is the board reference voltage, expressed in millivolts (defaults to 5000)
		 * @param dividerRatio is the multiplier used to obtain the real battery voltage (defaults to 2.0)
		 * @param mapFunction is a pointer to the function used to map the battery voltage to the remaining capacity percentage (defaults to linear mapping)
		 */
		void begin(uint16_t refVoltage = 5000, float dividerRatio = 2, uint8_t (*mapFunction)(uint16_t, uint16_t, uint16_t) = &linear);

		/**
		 * Returns the current battery level as a number between 0 and 100, with 0 indicating an empty battery and 100 a
		 * full battery.
		 */
		uint8_t level();
		uint8_t level(uint16_t voltage);

		/**
		 * Returns the current battery voltage in millivolts.
		 */
		uint16_t voltage();

	private:
		uint16_t refVoltage;
		uint16_t minVoltage;
		uint16_t maxVoltage;
		float dividerRatio;
		uint8_t sensePin;
		uint8_t activationPin;
		uint8_t activationMode;
		uint8_t (*mapFunction)(uint16_t, uint16_t, uint16_t);
		uint8_t linear(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage);
		uint8_t sigmoidal(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage);
};
#endif // _BATTERY_
