import chimera_v2
import time

# WIP
chimera_device = chimera_v2.CRIT()
led_device = chimera_device.spi_device("SPI_LED")

chimera_device.gpio_write("GPIO_LED_RCK", False)
time.sleep(1)
led_device.transmit(b"\xee")
time.sleep(1)
chimera_device.gpio_write("GPIO_LED_RCK", True)
time.sleep(1)
chimera_device.gpio_write("GPIO_LED_RCK", False)
