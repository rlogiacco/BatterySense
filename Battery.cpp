/*
 Battery.cpp - Battery library
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

#include "Battery.h"
#include <Arduino.h>

Battery::Battery(uint16_t minVoltage, uint16_t maxVoltage, uint8_t sensePin, uint8_t activationPin) {
	this->sensePin = sensePin;
	this->activationPin = activationPin;
	this->minVoltage = minVoltage;
	this->maxVoltage = maxVoltage;
}

void Battery::begin(uint16_t refVoltage, float dividerRatio) {
	this->refVoltage = refVoltage;
	this->dividerRatio = dividerRatio;
	pinMode(this->sensePin, INPUT);
	if (this->activationPin < 0xFF) {
		pinMode(this->activationPin, OUTPUT);
	}
}

uint8_t Battery::level() {
	int16_t sense = this->voltage();
	if (sense <= minVoltage) {
		return 0;
	} else if (sense >= maxVoltage) {
		return 100;
	} else {
		return (unsigned long)(sense - minVoltage) * 100 / (maxVoltage - minVoltage);
	}
}

uint16_t Battery::voltage() {
	if (activationPin != 0xFF) {
		digitalWrite(activationPin, HIGH);
		delayMicroseconds(10); // copes with slow switching activation circuits
	}
	analogRead(sensePin);
	delay(2); // allow the ADC to stabilize
	uint16_t reading = analogRead(sensePin) * dividerRatio * refVoltage / 1024;
	if (activationPin != 0xFF) {
		digitalWrite(activationPin, LOW);
	}
	return reading;
}
