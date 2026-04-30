<!-- markdownlint-disable -->

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2#L0"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

# <kbd>module</kbd> `chimera_v2`
Debug UBC Formula Electric boards with Python. 

Provides tooling to debug devices over USB. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\log_usb_devices#L33"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>function</kbd> `log_usb_devices`

```python
log_usb_devices()
```

Debug utility for printing all available usb devices. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\I2cDevice#L335"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `I2cDevice`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L336"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__(owner: '_Board', net_name: 'str')
```

Create an abstraction around an I2c device. 

This constructor should NEVER be called on its own, instead create I2c devices via a board's ``i2c_device`` method. 



**Args:**
 
 - <b>`owner`</b>:  Owner board. 
 - <b>`net_name`</b>:  Identifier of the I2C device. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\is_ready#L351"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `is_ready`

```python
is_ready() → bool
```

Check if an i2c device is ready. 



**Returns:**
  True if the device is ready, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\memory_read#L414"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `memory_read`

```python
memory_read(memory_address: 'int', length: 'int') → bytes
```

Read bytes from I2C device memory. 



**Args:**
 
 - <b>`memory_address`</b>:  Address of memory to be read. 
 - <b>`length`</b>:  Number of bytes to read. 



**Returns:**
 Bytes read. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\memory_write#L441"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `memory_write`

```python
memory_write(memory_address: 'int', data: 'bytes')
```

Read bytes into I2C device memory. 



**Args:**
 
 - <b>`memory_address`</b>:  Address of memory to be write to. 
 - <b>`data`</b>:  Bytes to write. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\receive#L369"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `receive`

```python
receive(length: 'int') → bytes
```

Receive bytes from the I2C device. 



**Args:**
 
 - <b>`length`</b>:  number of bytes to receive. 



**Returns:**
 Bytes received. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\transmit#L392"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `transmit`

```python
transmit(data: 'bytes')
```

Transmit bytes to the I2C device. 



**Args:**
 
 - <b>`data`</b>:  Bytes to transmit. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\SpiDevice#L466"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `SpiDevice`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L467"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__(owner: '_Board', net_name: 'str')
```

Create an abstraction around an SPI device. 

This constructor should NEVER be called on its own, instead create SPI devices via a board's ``spi_device`` method. 



**Args:**
 
 - <b>`owner`</b>:  Owner board. 
 - <b>`net_name`</b>:  Identifier of the SPI device. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\receive#L482"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `receive`

```python
receive(length: 'int') → bytes
```

Receive bytes from the SPI device. 



**Args:**
 
 - <b>`length`</b>:  Number of bytes to receive. 



**Returns:**
 Bytes received. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\transact#L527"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `transact`

```python
transact(request_data: 'bytes', response_length: 'int') → bytes
```

Run a full transaction (tx/rx) to the SPI device. 



**Args:**
 
 - <b>`request_data`</b>:  Bytes to transmit. 
 - <b>`response_length`</b>:  Number of bytes to read. 



**Returns:**
 Bytes received. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\transmit#L505"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `transmit`

```python
transmit(data: 'bytes')
```

Transmit bytes to the SPI device. 



**Args:**
 
 - <b>`data`</b>:  Bytes to transmit. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\DAM#L555"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `DAM`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L556"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an interface to an DAM board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\adc_read#L222"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: 'str') → float
```

Read the voltage at an ADC pin specified by the net name. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the ADC pin. 



**Returns:**
 Voltage value read over pin. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_read#L173"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: 'str') → bool
```

Read the value of a GPIO pin given the net name of the pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 



**Returns:**
 True if the pin is set high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_write#L198"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool')
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\i2c_device#L247"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `i2c_device`

```python
i2c_device(net_name: 'str') → I2cDevice
```

Create an abstraction around an I2c device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the I2C device. 



**Returns:**
 An I2C device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\pwm_set#L313"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `pwm_set`

```python
pwm_set(net_name: 'str', duty_cycle: 'float')
```

:param net_name: :param duty_cycle: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\spi_device#L259"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `spi_device`

```python
spi_device(net_name: 'str') → SpiDevice
```

Create an abstraction around a SPI device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the SPI device. 



**Returns:**
 An SPI device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_recieve#L292"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_recieve`

```python
uart_recieve(net_name: 'str', length: 'int') → bytes
```

:param net_name: :param length: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_transmit#L271"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_transmit`

```python
uart_transmit(net_name: 'str', data: 'bytes') → None
```

:param net_name: :param data: :return: 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\RSM#L566"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `RSM`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L567"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an interface to an RSM board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\adc_read#L222"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: 'str') → float
```

Read the voltage at an ADC pin specified by the net name. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the ADC pin. 



**Returns:**
 Voltage value read over pin. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_read#L173"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: 'str') → bool
```

Read the value of a GPIO pin given the net name of the pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 



**Returns:**
 True if the pin is set high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_write#L198"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool')
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\i2c_device#L247"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `i2c_device`

```python
i2c_device(net_name: 'str') → I2cDevice
```

Create an abstraction around an I2c device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the I2C device. 



**Returns:**
 An I2C device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\pwm_set#L313"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `pwm_set`

```python
pwm_set(net_name: 'str', duty_cycle: 'float')
```

:param net_name: :param duty_cycle: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\spi_device#L259"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `spi_device`

```python
spi_device(net_name: 'str') → SpiDevice
```

Create an abstraction around a SPI device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the SPI device. 



**Returns:**
 An SPI device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_recieve#L292"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_recieve`

```python
uart_recieve(net_name: 'str', length: 'int') → bytes
```

:param net_name: :param length: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_transmit#L271"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_transmit`

```python
uart_transmit(net_name: 'str', data: 'bytes') → None
```

:param net_name: :param data: :return: 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\VC#L577"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `VC`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L578"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an interface to an DAM board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\adc_read#L222"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: 'str') → float
```

Read the voltage at an ADC pin specified by the net name. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the ADC pin. 



**Returns:**
 Voltage value read over pin. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_read#L173"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: 'str') → bool
```

Read the value of a GPIO pin given the net name of the pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 



**Returns:**
 True if the pin is set high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_write#L198"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool')
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\i2c_device#L247"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `i2c_device`

```python
i2c_device(net_name: 'str') → I2cDevice
```

Create an abstraction around an I2c device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the I2C device. 



**Returns:**
 An I2C device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\pwm_set#L313"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `pwm_set`

```python
pwm_set(net_name: 'str', duty_cycle: 'float')
```

:param net_name: :param duty_cycle: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\spi_device#L259"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `spi_device`

```python
spi_device(net_name: 'str') → SpiDevice
```

Create an abstraction around a SPI device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the SPI device. 



**Returns:**
 An SPI device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_recieve#L292"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_recieve`

```python
uart_recieve(net_name: 'str', length: 'int') → bytes
```

:param net_name: :param length: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_transmit#L271"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_transmit`

```python
uart_transmit(net_name: 'str', data: 'bytes') → None
```

:param net_name: :param data: :return: 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\BMS#L588"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `BMS`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L589"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an interface to an DAM board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\adc_read#L222"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: 'str') → float
```

Read the voltage at an ADC pin specified by the net name. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the ADC pin. 



**Returns:**
 Voltage value read over pin. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_read#L173"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: 'str') → bool
```

Read the value of a GPIO pin given the net name of the pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 



**Returns:**
 True if the pin is set high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_write#L198"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool')
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\i2c_device#L247"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `i2c_device`

```python
i2c_device(net_name: 'str') → I2cDevice
```

Create an abstraction around an I2c device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the I2C device. 



**Returns:**
 An I2C device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\pwm_set#L313"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `pwm_set`

```python
pwm_set(net_name: 'str', duty_cycle: 'float')
```

:param net_name: :param duty_cycle: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\spi_device#L259"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `spi_device`

```python
spi_device(net_name: 'str') → SpiDevice
```

Create an abstraction around a SPI device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the SPI device. 



**Returns:**
 An SPI device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_recieve#L292"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_recieve`

```python
uart_recieve(net_name: 'str', length: 'int') → bytes
```

:param net_name: :param length: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_transmit#L271"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_transmit`

```python
uart_transmit(net_name: 'str', data: 'bytes') → None
```

:param net_name: :param data: :return: 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\CRIT#L599"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `CRIT`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L600"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an interface to an CRIT board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\adc_read#L222"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: 'str') → float
```

Read the voltage at an ADC pin specified by the net name. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the ADC pin. 



**Returns:**
 Voltage value read over pin. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_read#L173"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: 'str') → bool
```

Read the value of a GPIO pin given the net name of the pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 



**Returns:**
 True if the pin is set high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_write#L198"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool')
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\i2c_device#L247"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `i2c_device`

```python
i2c_device(net_name: 'str') → I2cDevice
```

Create an abstraction around an I2c device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the I2C device. 



**Returns:**
 An I2C device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\pwm_set#L313"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `pwm_set`

```python
pwm_set(net_name: 'str', duty_cycle: 'float')
```

:param net_name: :param duty_cycle: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\spi_device#L259"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `spi_device`

```python
spi_device(net_name: 'str') → SpiDevice
```

Create an abstraction around a SPI device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the SPI device. 



**Returns:**
 An SPI device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_recieve#L292"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_recieve`

```python
uart_recieve(net_name: 'str', length: 'int') → bytes
```

:param net_name: :param length: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_transmit#L271"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_transmit`

```python
uart_transmit(net_name: 'str', data: 'bytes') → None
```

:param net_name: :param data: :return: 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\FSM#L609"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `FSM`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\__init__#L610"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an interface to an CRIT board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\adc_read#L222"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: 'str') → float
```

Read the voltage at an ADC pin specified by the net name. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the ADC pin. 



**Returns:**
 Voltage value read over pin. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_read#L173"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: 'str') → bool
```

Read the value of a GPIO pin given the net name of the pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 



**Returns:**
 True if the pin is set high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\gpio_write#L198"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool')
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\i2c_device#L247"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `i2c_device`

```python
i2c_device(net_name: 'str') → I2cDevice
```

Create an abstraction around an I2c device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the I2C device. 



**Returns:**
 An I2C device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\pwm_set#L313"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `pwm_set`

```python
pwm_set(net_name: 'str', duty_cycle: 'float')
```

:param net_name: :param duty_cycle: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\spi_device#L259"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `spi_device`

```python
spi_device(net_name: 'str') → SpiDevice
```

Create an abstraction around a SPI device. 



**Args:**
 
 - <b>`net_name`</b>:  Identifier of the SPI device. 



**Returns:**
 An SPI device abstraction. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_recieve#L292"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_recieve`

```python
uart_recieve(net_name: 'str', length: 'int') → bytes
```

:param net_name: :param length: :return: 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master\firmware\chimera_v2\chimera_v2\uart_transmit#L271"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `uart_transmit`

```python
uart_transmit(net_name: 'str', data: 'bytes') → None
```

:param net_name: :param data: :return: 




---

_This file was automatically generated via [lazydocs](https://github.com/ml-tooling/lazydocs)._
