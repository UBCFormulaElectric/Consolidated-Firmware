<!-- markdownlint-disable -->

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2#L0"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

# <kbd>module</kbd> `chimera_v2`
Debug UBC Formula Electric boards with Python. 

Provides tooling to debug devices over USB, CAN, etc. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/log_usb_devices#L38"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>function</kbd> `log_usb_devices`

```python
log_usb_devices()
```

Debug utility for printing all available usb devices. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/CanDevice#L50"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `CanDevice`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/__init__#L51"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__(dbc_path: 'str', bus: 'BusABC')
```

Create an abstraction around a CAN bus device. 



**Args:**
 
 - <b>`dbc_path`</b>:  Path to a dbc file. 
 - <b>`bus`</b>:  handle to a can bus. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/get#L92"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `get`

```python
get(msg_name: 'str', signal_name: 'str') → Optional[Any]
```

Get a given signal from the last received can message. 



**Args:**
 
 - <b>`msg_name`</b>:  Name of the message. 
 - <b>`signal_name`</b>:  Name of the signal. 



**Returns:**
 The value of the signal, or None of the message name has not been received. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/transmit#L108"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `transmit`

```python
transmit(msg_name: 'str', data: 'Dict[str, Any]')
```

Transmit a message given it's data. 



**Args:**
 
 - <b>`msg_name`</b>:  Name of the message. 
 - <b>`data`</b>:  Dictonary containing the signals to send. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/I2cDevice#L339"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `I2cDevice`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/__init__#L340"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/is_ready#L355"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `is_ready`

```python
is_ready() → bool
```

Check if an i2c device is ready. 



**Returns:**
  True if the device is ready, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/memory_read#L416"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/memory_write#L443"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `memory_write`

```python
memory_write(memory_address: 'int', data: 'bytes')
```

Read bytes into I2C device memory. 



**Args:**
 
 - <b>`memory_address`</b>:  Address of memory to be write to. 
 - <b>`data`</b>:  Bytes to write. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/receive#L373"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/transmit#L396"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `transmit`

```python
transmit(data: 'bytes')
```

Transmit bytes to the I2C device. 



**Args:**
 
 - <b>`data`</b>:  Bytes to transmit. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/F4Dev#L468"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `F4Dev`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/__init__#L469"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__() → None
```

Create an interface to an F4Dev board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/adc_read#L301"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/gpio_read#L252"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/gpio_write#L277"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool') → None
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/i2c_device#L326"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/SSM#L481"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `SSM`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/__init__#L482"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__() → None
```

Create an interface to an SSM board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/adc_read#L301"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/gpio_read#L252"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/gpio_write#L277"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool') → None
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/i2c_device#L326"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/CRIT#L494"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `CRIT`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/__init__#L495"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__() → None
```

Create an interface to a CRIT/cDIM board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/adc_read#L301"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/gpio_read#L252"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/gpio_write#L277"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: 'str', value: 'bool') → None
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2/i2c_device#L326"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

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

_This file was automatically generated via [lazydocs](https://github.com/ml-tooling/lazydocs)._
