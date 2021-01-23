### &#x26A0; **IMPORTANT**
 
> Please, before submitting a support request read carefully this README and check if an answer already exists among [previously answered questions](https://github.com/rlogiacco/BatterySense/discussions): do not abuse of the Github issue tracker.

<!-- omit in toc -->
Battery Sense [![Build Status][travis-status]][travis]
=============
[travis]: https://travis-ci.org/rlogiacco/BatterySense
[travis-status]: https://travis-ci.org/rlogiacco/BatterySense.svg?branch=master

This is a simple Arduino library to monitor battery consumption of your battery powered projects, being LiPo, LiIon, NiCd or any other battery type, single or multiple cells: if it can power your Arduino you can monitor it!

The principle is simple: we are going to measure our battery capacity by measuring the voltage across the battery terminals.

The big assumption here is that battery capacity is linearly correlated to its voltage: the assumption itself is wrong, but in most cases it's *close enough* to reality, especially when it comes to the battery higher capacity side.

In reality, the relation between battery capacity and its voltage is better represented by a curve and there are many factors affecting it: current drawn, temperature, age, etc...

![Battery discharge curves at 750 mA](http://www.philohome.com/batteries/discharge-750.gif)

<!-- toc -->
- [How to](#how-to)
  - [Lesser than 5V, with voltage booster](#lesser-than-5v-with-voltage-booster)
  - [Higher than 5V, with internal voltage regulator](#higher-than-5v-with-internal-voltage-regulator)
  - [Higher than 5V, with external voltage regulator](#higher-than-5v-with-external-voltage-regulator)
  - [Higher than 5V, activated on demand](#higher-than-5v-activated-on-demand)
- [Voltage divider ratio](#voltage-divider-ratio)
- [Remaining capacity approximation](#remaining-capacity-approximation)
  - [Improvable](#improvable)
  - [Good enough](#good-enough)
- [Examples](#examples)
  - [Single-cell Li-Ion on 3.3V MCU](#single-cell-li-ion-on-33v-mcu)
  - [Double cell Li-Ion (2S) on 5V MCU](#double-cell-li-ion-2s-on-5v-mcu)
  - [9V Alkaline on 5V MCU](#9v-alkaline-on-5v-mcu)
<!-- tocstop -->

## How to
The library requires at least 1 analog pin (we will call this the `sense pin`) and no less than 2 pieces of info on your battery: the voltage you will consider the minimum acceptable level, below which your project/product becomes unreliable and should be shut down, and the maximum voltage you can expect when the battery is fully charged.

Additionally, you can provide a second pin (either analog or digital) to activate the battery measurement circuit (we call it the `activation pin`), useful in all those situations where you can sacrifice a pin to further increase your battery duration.

If you want your readings to be more accurate we *strongly suggest* to calibrate the library by providing your board reference voltage: most of the times you assume your board has exactly 5V between `Vcc` and `GND`, but this is rarely the case. To improve this we suggest using the [VoltageReference](https://github.com/rlogiacco/VoltageReference) library to obtain a better calibration value for all analog readings.

The `sense pin` wiring can vary depending on your battery configuration, but here are a few examples based on the assumption you are using a 5V board: in case of a 3.3V board you should be performing the necessary adjustments.

### Lesser than 5V, with voltage booster
Voltage sources made of single cell LiPo or LiIon, along with some single or multi-cell NiCd configurations (like up to 3 AA or AAA), are not able to provide the *suggested* `5.0 volts input` to your board and a voltage booster can solve your problem.
What does that mean when it comes to measuring your battery level? We need to measure the battery voltage *before* it gets boosted, which means your `sense pin` must be connected between the battery positive terminal and the booster positive input and we don't need any additional components as the voltage is already in the acceptable range:

```
                                               +---------+
          +----------------------------- SENSE |         |
          |        +---------+                 |         |
          |        |         |                 |         |
  BAT+ ---+--- IN+ |   5V    | OUT+ ------- 5V | Arduino |
                   | BOOSTER |                 |         |
  BAT- ------- IN- |         | OUT- ------ GND |         |
                   +---------+                 |         |
                                               +---------+
```

### Higher than 5V, with internal voltage regulator
Voltage sources made of multiple cells LiPo or LiIon, along with some single or multi-cell NiCd configurations (like up the classic 9V battery or 4+ AA or AAA), provide voltages above the `5.0 volts input`: most of the Arduino boards are equipped with voltage regulators able to dissipate into heat all the excess.
To measure such batteries we need to hook our `sense pin` *before* it gets regulated, between the battery positive terminal and the Arduino unregulated input `VIN` or `RAW`, but we require two resistors to reduce the voltage to acceptable values:

```
                         +---------+
  BAT+ ---+--------- VIN |         |
          |              |         |
         R1              |         |
          |              |         |
          +------- SENSE | Arduino |
          |              |         |
         R2              |         |
          |              |         |
  BAT- ------------- GND |         |
                         +---------+
```

The values of R1 and R2 determine the `voltage ratio` parameter for this library: for information about this value refer to the section below.

Because the resistors in this configuration will constantly draw power out of your battery, you shouldn't pick values under `1k Ohm`, or you'll deplete your batteries much faster than normal. On the other end, going too high on the resistor values will impede the library from getting accurate readings.

### Higher than 5V, with external voltage regulator
Whenever your battery maximum voltage exceeds the onboard regulator (if there is any) an external voltage regulator is required.
Once again, to measure such batteries we need to hook our `sense pin` *before* it gets regulated, between the battery positive terminal and the voltage regulator positive input `VIN` or `RAW` and, as before, we require two resistors to reduce the voltage to acceptable values:

```
+---------------------------------+
|                  +---------+    |         +---------+
| BAT+ ---+--- IN+ |         |    +-- SENSE |         |
|         |        |         |              |         |
|        R1        |         |              |         |
|         |        |         |              |         |
+---------+        |   REG   | OUT+ ---- 5V | Arduino |
          |        |         |              |         |
         R2        |         |              |         |
          |        |         |              |         |
  BAT- ---+--- IN- |         | OUT- --- GND |         |
                   +---------+              +---------+
```

The values of R1 and R2 determine the `voltage ratio` parameter for this library: for information about this value refer to the section below.

### Higher than 5V, activated on demand
Batteries are a precious resource and you want to prolong their life as much as you can so, deplete your battery to determine its capacity is not desirable.

As a consequence of connecting the battery terminals through two resistors we are drawing some energy out of the battery: for a 9V battery and 1k Ohm for R1 and R2, you will be adding a constant 4.5mA current consumption to your circuit. Not a huge amount, but definitely not desirable.

If you have an unused pin on your Arduino it will be easy to limit this additional current consumption to be drawn only when needed: during battery measurement. We will be turning the `activation pin` HIGH during battery measurement so that the voltage divider will be disconnected most of the time:

```
                         +---------+
  BAT+ ---+--------- VIN |         |
          |              |         |
         SW--------- ACT |         |
          |              |         |
         R1              |         |
          |              | Arduino |
          +------- SENSE |         |
          |              |         |
         R2              |         |
          |              |         |
  BAT- ------------- GND |         |
                         +---------+
```

In the above schematics **SW** is a circuit which can connect or disconnect the sensing circuit depending on the voltage on `ACT`: the most common and cheap circuit is made of a NPN transistor *Q1*, a p-channel MOSFET *Q2*, a 1k-4.7k Ohm resistor *R3* and a 5k-20k Ohm resistor *R4*:

```
                       BAT+
                        |
                  +-----+
                  |     |
                  R4    |
                  |\    |
   ACT --- R3 ---Q1 \-- Q2
                  |     |
                  |     |
                 GND   VDIV
                       to R1/R2/SENSE

```

Feel free to refer to this [circuit simulation](http://tinyurl.com/ydbjfk67) to better understand how the circuit works and how much current draws when in operation.

![Simulated Circuit](https://image.ibb.co/iob7AV/sense.gif)

## Voltage divider ratio
Whenever your battery voltage is above your board voltage you need a voltage divider to constraint your readings within the 0-5V range allowed by your Arduino and you will have to provide this library with its *ratio*.

```

  BAT+ ---+
          |
         R1
          |
          +------- SENSE
          |
         R2
          |
  BAT- ----
```

The `voltage divider ratio` is determined by the formula `(R1 + R2) / R2`: if you use two resistors of the same value the ratio will be **2**, which can be interpreted as *whatever value we read it will be **half** of the actual value*. This allows us to sense batteries up to 10V. 
If you use a 22k Ohm resistor for R1 and a 10k Ohm for R2 than your `voltage ratio` will be **3.2** and you will be able to safely monitor a 12-15V battery.

You **must** select the resistors in order to get a ratio which will produce values between the 0-5V range (or 0-3.3V for 3.3V devices) **at all the times** and to obtain that the process is quite simple: divide your battery maximum voltage by 5V and you'll get the *absolute minimum value* for the `voltage ratio`, then pick any two resistors values whose combination produce a ratio *equal or higher* than the absolute minimum. For a 12V battery the *absolute minimum voltage ratio* is **12/5=2.4**, meaning you can't use a *split supply divider* made of two equal resistors: you need R1 to be a higher value than R2! Get this wrong and you will probably burn your `sense pin`.

You can use [this nice website](http://www.ohmslawcalculator.com/voltage-divider-calculator) to find some appropriate values for the resistors setting your battery maximum voltage as _Voltage source_ and aiming at obtaining a _Output voltage_ value lesser than your board voltage (`5V` or `3.3V`) but as close as possible.

The *voltage divider total resistance*, made of `R1 + R2`, will determine the current drawn from your battery by the sensing circuit: lower is the total resistance and more accurate are your readings, higher the resistance and less current is drawn from your battery ([Ohm's law](http://www.ohmslawcalculator.com/ohms-law-calculator) rulez!). My suggestion is to keep this value within 20k-22k Ohm when using an *always-connected* circuit and under 10k Ohm if you use an *on-demand* configuration.

When determining the *ratio* don't stick with the resistors nominal values, instead, if possible, use a multimeter to actually measure their resistance so to improve your results: a `4.7kΩ` resistor could easily be a `4.75kΩ` in reality!

## Remaining capacity approximation
The `level` available functions aim at providing an approximation of the remaining battery capacity in percentage. This is not an easy task when you want to achieve reliable values and it is something the industry of mobile devices invests a decent amount of resources.
When an accurate estimate is desireable the battery voltage is not the sole parameter you want to take into consideration:
 * cell **chemistry** has a very high influence, obviously
 * cells based on the same chemistry might produce pretty different results depending on the **production process**
 * each chemistry has a different ideal operating **cell temperature**
 * the rate you **draw current** from the battery influences the remaining capacity
 * batteries are not everlasting: as the cell **age**s, the battery capacity gets reduced
 * _and more_ 

The library itself doesn't aim at providing accurate estimates, but what I consider _an improvable but good enough_ estimate.

### Improvable
The library can be configured to use a mapping function of your choice, given the function complies with the `mapFn_t` interface:

```cpp
uint8_t mapFunction(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage)
```

To configure your personalized function you only have to provide a pointer to it during initialization:

```cpp
Battery batt = Battery(3000, 4200, SENSE_PIN);

uint8_t myMapFunction(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage) {
  // your code here
}


void setup() {
  batt.begin(3300, 1.47, &myMapFunction);
}
```

> You are not limited in considering only the parameters listed in the function interface, meaning you can take into consideration the cell(s) temperature, current consumption or age: that's open to your requirements and circuitry.

### Good enough
After collecting a few data points on battery voltage vs. battery capacity, I've used the https://mycurvefit.com/ and https://www.desmos.com online tools to calculate the math functions best representing the data I've collected.

![Mapping functions](https://github.com/rlogiacco/BatterySense/blob/master/map-fn.png?raw=true)

> In the above plot I represent the battery percentage (Y axis) as a function of the difference between the current battery voltage and the minimum value (X axis): the graph represents a battery with a voltage swing of 1200mV from full to empty, but the functions scale accordingly to the `minVoltage` and `maxVoltage` parameters.

The library ships with three different implementations of mapping function: 

 * _linear_ is the default one (dashed red), probably the least accurate but the easiest to understand. It's main drawback is, for most chemistries, it will very quickly go from 25-20% to 0%, meaning you have to select the `minVoltage` parameter for your battery accordingly. As an example, a typical Li-Ion battery having a 3V to 4.2V range, you want to specify a 3.3V configuration value as _minimum voltage_.
 * _sigmoidal_ (in blue) is a good compromise between computational effort and approximation, modeled after the tipical discharge curve of Li-Ion and Li-Poly chemistries. It's more representative of the remaining charge on the lower end of the spectrum, meaning you can set the minimum voltage accordingly to the battery safe discharge limit (typically 3V for a Li-Ion or Li-Poly).
 * _asymmetric sigmoidal_ (in green) is probably the best approximation when you only look at battery voltage, but it's more computational expensive compared to _sigmoidal_ function and, in most cases, it doesn't provide a great advantage over it's simmetric counterpart.

I strongly encourage you to determine the function that best matches your particular battery chemistry/producer when you want to use this library in your product.

## Examples
Here follow a few real case scenarios which can guide you in using this library.

### Single-cell Li-Ion on 3.3V MCU
As an example, for a single cell Li-Ion battery (4.2V - 3.7V) powering a `3.3V MCU`, you'll need to use a voltage divider with a ratio no less than `1.3`. Considering only E6 resistors, you can use a `4.7kΩ` (R1) and a `10kΩ` (R2) to set a ratio of `1.47`: this allows to measure batteries with a maximum voltage of `4.85V`, well within the swing of a Li-Ion. It's a little too current hungry for my tastes in an *always-connected* configuration, but still ok. Considering the chemistry maps pretty well to our sigmoidal approximation function I'm going to set it accordingly along with the minimum voltage which lowest safe value clearly is 3.0V (if a Li-Ion is drained below `3.0V` the risk of permanent damage is high), so your code should look like:

```cpp
Battery batt = Battery(3000, 4200, SENSE_PIN);

void setup() {
  // specify an activationPin & activationMode for on-demand configurations
  //batt.onDemand(3, HIGH);
  batt.begin(3300, 1.47, &sigmoidal);
}
```

### Double cell Li-Ion (2S) on 5V MCU
For a double cell Li-Ion battery (8.4V - 7.4V) powering a `5V MCU`, you'll need to use a voltage divider with a ratio no less than `1.68`: you can use a `6.8kΩ` (R1) and a `10kΩ` (R2) to set the ratio *precisely* at `1.68`, perfect for our `8.4V` battery pack. The circuit will continuously draw 0.5mA in an *always-connected* configuration, if you can live with that. As we don't want to ruin our battery pack and we don't want to rush from 20% to empty in afew seconds, we'll have to set the minimum voltage to `6.8V` (with a _linear_ mapping) to avoid the risk of permanent damage, meaning your code should look like:

```cpp
Battery batt = Battery(6800, 8400, SENSE_PIN); 

void setup() {
  // specify an activationPin & activationMode for on-demand configurations
  //batt.onDemand(3, HIGH);
  batt.begin(5000, 1.68);
}
```

> **NOTE**: I could have used the _sigmoidal_ approximation, as the chemistry fits pretty well on the curve, in which case a `6V` minimum voltage would have been a better configuration value.

### 9V Alkaline on 5V MCU
Another classic example might be a single 9V Alkaline battery (9V - 6V) powering a `5V MCU`. In this case, you'll need to use a voltage divider with a ratio no less than `1.8` and, for sake of simplicity, we'll go for a nice round `2` ratio. Using a nice `10kΩ` both for R1 and R2 we'll be able to measure batteries with a maximum voltage of `10V` consuming only 0.45mA. The trick here is to determine when our battery should be considered empty: a 9V Alkaline, being a non-rechargeable one, can potentially go down to 0V, but it's hard our board can still be alive when this occurs. Assuming we are using a linear regulator to step down the battery voltage to power our board we'll have to account for the regulator voltage drop: assuming it's a `1.2V` drop, we might safely consider our battery empty when it reaches `6.2V` (5V + 1.2V), leading to the following code:

```cpp
Battery batt = Battery(6200, 9000, SENSE_PIN);

void setup() {
  // specify an activationPin & activationMode for on-demand configurations
  //batt.onDemand(3, HIGH);
  batt.begin(5000, 2.0);
}
```


> **NOTE**: Most `5V MCU` can actually continue to operate when receiving `4.8V` or even less: if you want to squeeze out as much energy as you can you can fine tune the low end, but also consider there is not much juice left when a battery voltage drops that much.
