# Battery
This is a simple Arduino library to monitor battery consumption of your battery powered projects, being LiPo, LiIon, NiCd or any other battery type, single or multiple cells: if it can power your Arduino you can monitor it!

The principle is simple: we are going to measure our battery capacity by measuring the voltage across the battery terminals.

The big assumption here is that battery capacity is linearly correlated to its voltage: the assumption itself is wrong, but in most cases it's *close enough* to reality, especially when it comes to the battery higher capacity side.

In reality the relation between battery capacity and its voltage is better represented by a curve and there are many factors affecting it.

## How to
The library requires at least 1 analog pin (we will call this the `sense pin`) and no less than 2 bits of info on your battery: the voltage you will consider the minimum acceptable level, below which your project/product becomes unreliable and should be shut down, and the maximum voltage you can expect when the battery is fully charged.

Additionally you can provide a second pin (either analog or digital) to activate the battery measurement circuit (we call it the `activation pin`), useful in all those situation where you can sacrifice a pin to further increase your battery duration.

If you want your readings to be more accurate we *strongly suggest* to calibrate the library providing your board reference voltage: most of the times you assume your board has exactly 5V between `Vcc` and `GND`, but this is rarely the case. To improve this we suggest to use the [VoltageReference](https://github.com/rlogiacco/VoltageReference) library to obtain a better calibration value for all annalog readings.

The `sense pin` wiring can vary depending on your battery configuration, but here are a few examples based on the assumption you are using a 5V board: in case of a 3.3V board you should be performing the necessary adjustments.

### Lesser than 5V, with voltage booster
Voltage sources made of single cell LiPo or LiIon, along with some single or multi cell NiCd configurations (like up to 3 AA or AAA), are not able to provide the *suggested* `5.0 volts input` to your board and a voltage booster can solve your problem.
What that means when it comes to measuring your battery level? We need to measure the battery voltage *before* it gets boosted, which means your `sense pin` must be connected between the battery positive terminal and the booster positive input and we don't need any additional components as the voltage is already in the acceptable range:

```
                                               +---------+
          +----------------------------- SENSE |         |
          |        +---------+                 |         |
  BAT+ ---+--- IN+ |   5V    | OUT+ ------- 5V | Arduino |
                   | BOOSTER |                 |         |
  BAT- ------- IN- |         | OUT- ------ GND |         |
                   +---------+                 +---------+
```

### Higher than 5V, with internal voltage regulator
Voltage sources made of multiple cells LiPo or LiIon, along with some single or multi cell NiCd configurations (like up the classic 9V battery or 4+ AA or AAA), provide voltages above the `5.0 volts input`: most of the Arduino boards are equipped with voltage regulators able to dissipate into heat all the excess.
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

The values of R1 and R2 determine the `voltage ratio` parameter for this library which is determined by `(R1 + R2) / R2`: if you use two resistors of the same value the ratio will be **2**, which means whatever value we read it will be *half of the actual value*.

You **must** select the resistors in order to get a ratio which will produce values between the 0-5V range **at all the times** and to obtain that the process is quite simple: divide your battery maximum voltage by 5V and you'll get the *absolute minimum value* for the `voltage ratio`, then pick any two resistors values whose combination produce a ratio *equal or higher* than the absolute minimum.

Because the resistors in this configuration will constantly draw power out of your battery, you shouldn't pick values under the `1k Ohm`, or you'll deplenish your batteries much faster than normal. On the other end, going too high on the resistor values will impede the library to get accurate readings.

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
+---------+        |         | OUT+ ---- 5V | Arduino |
          |        |         |              |         |
         R2        |         |              |         |
          |        |         |              |         |
  BAT- ---+--- IN- |         | OUT- --- GND |         |
                   +---------+              +---------+
```

As stated before, the `voltage ratio` determined by **R1** and **R2** **must** produce values between the 0-5V range **at all the times**: use the same rules and guidelines stated above to determine the resistors value.