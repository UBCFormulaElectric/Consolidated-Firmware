<!-- markdownlint-disable -->

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L0"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

# <kbd>module</kbd> `chimera_v2`
Debug UBC Formula Electric boards with Python. 

Provides tooling to debug devices over USB, CAN, etc. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L23"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>function</kbd> `log_usb_devices`

```python
log_usb_devices()
```

Debug utility for printing all available usb devices. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L35"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `CanDevice`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L36"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__(dbc_path: str, bus: BusABC)
```

Create an abstraction around a CAN bus device. 



**Args:**
 
 - <b>`dbc_path`</b>:  Path to a dbc file. 
 - <b>`bus`</b>:  handle to a can bus. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L75"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `get`

```python
get(msg_name: str, signal_name: str) → Optional[<built-in function any>]
```

Get a given signal from the last received can message. 



**Args:**
 
 - <b>`msg_name`</b>:  Name of the message. 
 - <b>`signal_name`</b>:  Name of the signal. 



**Returns:**
 The value of the signal, or None of the message name has not been received. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L91"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `transmit`

```python
transmit(msg_name: str, data: Dict[str, <built-in function any>])
```

Transmit a message given it's data. 



**Args:**
 
 - <b>`msg_name`</b>:  Name of the message. 
 - <b>`data`</b>:  Dictonary containing the signals to send. 


---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L298"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `F4Dev`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L299"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__() → None
```

Create an interface to an F4Dev board. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L276"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: str) → float
```

Read the voltage at an ADC pin specified by the net name. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the ADC pin. 



**Returns:**
 Voltage value read over pin. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L233"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: str) → bool
```

Read the value of a GPIO pin given the net name of the pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 



**Returns:**
 True if the pin is set high, otherwise false. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/chimera_v2.py#L255"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: str, value: bool) → None
```

Write a value to a GPIO pin. 



**Args:**
 
 - <b>`net_name`</b>:  Name of the pin. 
 - <b>`value`</b>:  True if high, otherwise false. 




---

_This file was automatically generated via [lazydocs](https://github.com/ml-tooling/lazydocs)._
