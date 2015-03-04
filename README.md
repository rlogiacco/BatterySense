# Battery
This is a simple Arduino library to monitor battery consumption of your battery powered projects, being LiPo, LiIon, NiCd or any other battery type, single or multiple cells: if it can power your Arduino you can monitor it!

The principle is simple: we are going to measure our battery capacity by measuring the voltage across the battery terminals.

The big assumption here is that battery capacity is linearly correlated to its voltage: the assumption itself is wrong, but in most cases it's *close enough* to reality, especially when it comes to the battery higher capacity side.

In reality the relation between battery capacity and its voltage is better represented by a curve and there are many factors affecting it.

## How to
The library requires at least 1 analog pin (we will call this the `sense pin`) and no less than 2 bits of info on your battery: the voltage you will consider the minimum acceptable level, below which your project/product becomes unreliable and should be shut down, and the maximum voltage you can expect when the battery is fully charged.

Additionally you can provide a second pin (either analog or digital) to activate the battery measurement circuit (we call it the `activation pin`), useful in all those situation where you can sacrifice a pin to further increase your battery duration.

If you want your readings to be more accurate we *strongly suggest* to calibrate the library providing your board reference voltage: most of the times you assume your board has exactly 5V between `Vcc` and `GND`, but this is rarely the case. To improve this we suggest to use the [https://github.com/rlogiacco/VoltageReference VoltageReference] library to obtain a better calibration value for all annalog readings.

The `sense pin` wiring can vary depending on your battery configuration, but here are a few examples based on the assumption you are using a 5V board: in case of a 3.3V board you should be performing the necessary adjustments.

### Lesser than 5V, with voltage booster
Voltage sources made of single cell LiPo and LiIon, along with some single or multi cell NiCd configurations (like up to 3 AA or AAA), are not able to provide the *suggested* `5.0 volts input` to your board and a voltage booster can solve your problem.
What that means when it comes to measuring your battery level? We need to measure the battery voltage *before* it gets boosted, which means your `sense pin` must be connected between the battery positive terminal and the booster positive input:

```
                                              +---------+
         +-------------------------------- A0 |         |
         |        +---------+                 |         |
  BAT+ --*--- IN+ |         | OUT+ ------ Vcc | Arduino |
                  | BOOSTER |                 |         |
  BAT- ------ IN- |         | OUT- ------ GND |         |
                  +---------+                 +---------+
```