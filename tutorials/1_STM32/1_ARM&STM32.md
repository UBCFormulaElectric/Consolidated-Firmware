# ARM and STM32
Introduction to ARM architecture and STM32 microcontrollers.

## Table of Contents
1. [What is ARM and STM32?](#what-is-arm-and-stm32)
2. [Additional Resources](#additional-resources)

## What is ARM and STM32?

**ARM (Advanced RISC Machine)** is a processor **architecture based** on 32-bit and 64-bit reduced instruction set computer, designed by Arm Holdings.
* **RISC (reduced instruction set computer)** processors are designed to perform a smaller number of types of computer instructions so that they can operate at a higher speed
  * Extensively used in consumer electronic devices
* ARM does not manufacture actual chips, and sells license to the architecture for other companies to produce
* These are divided into series of Cortex:
  * **ARM Cortex-A**: Application processors provide a range of solutions for devices undertaking complex compute tasks
  * **ARM Cortex-M**: Smallest/lowest power processors, optimized for discrete processing and microcontrollers; the processors we use are based on this architecture
  * **ARM Cortex-R**: Processors optimized for high performance, hard real-time applications

![Figure 1][Figure 1]

* **STM32** is a family of 32 bit microprocessors manufactured by STMicroelectronics based on the ARM Cortex-M architecture
* These are divided into families:
  * **STM32F** is the foundation of the STM32 family that includes balanced devices targeting applications that do not require extreme low power. They combine high performance, peripherals, and low-power, low-voltage operation
    * This is the STM32 family that we normally use (ie. STM32F302R8, based on ARM Cortex-M4)
  * **STM32L** is based on ST's EnergyLite platform that targets ultra-low-power, ultra-low-voltage applications through the use of an optimized architecture and proprietary ultra-low leakage process, shared with the STM8L family
  * **STM32W** supports a configurable total link budget up to 109 dB with an ARM Cortex-M3 core to target the wireless sensor network market
  * **STM32H** is the highest performance MCU based on ARM Cortex-M7

![Figure 2][Figure 2]

## Additional Resources
1. [ST’s description of STM32 MCU][Resource 1]
2. [ARM processor description][Resource 2]
3. [ARM Cortex further description][Resource 3]

[Figure 1]: http://www.emcu.it/CortexFamily/ArmRoadMap.png "ARM Families"
[Figure 2]: https://i.imgur.com/zkB9vsZ.jpg "STM32 MCUs"

[Resource 1]: https://www.st.com/en/microcontrollers/stm32-32-bit-arm-cortex-mcus.html 
[Resource 2]: https://whatis.techtarget.com/definition/ARM-processor
[Resource 3]: https://electronics.stackexchange.com/questions/174044/difference-between-arm-a-and-m-series-processors 