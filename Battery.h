/*
 Battery.h - Battery library
 Copyright (c) 2014 Roberto Lo Giacco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
		 * @param activationPin is the optional digital pin which will be turned LOW before starting the battery sensing,
		 *            useful when using a resistor divider to save on battery consumption
		 */
		Battery(uint16_t minVoltage, uint16_t maxVoltage, uint8_t sensePin, uint8_t activationPin = 0xFF);

		/**
		 * Initializes the library by optionally setting the board voltage level, expressed in millivolts, (defaults to 5000)
		 * and the resistor divider ratio (defaults to 2, an equal divider).
		 * To obtain the best results use a calibrated reference using the VoltageReference library or equivalent.
		 */
		void begin(uint16_t refVoltage = 5000, float dividerRatio = 2);

		/**
		 * Returns the current battery level as a number between 0 and 100, with 0 indicating an empty battery and 100 a
		 * completely full battery.
		 */
		uint8_t level();

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
};

#endif // _BATTERY_
