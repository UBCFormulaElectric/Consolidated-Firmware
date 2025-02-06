<!-- markdownlint-disable -->

<a href="../../../firmware/chimera_v2/chimera_v2.py#L0"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

# <kbd>module</kbd> `chimera_v2`
Debug UBC Formula Electric boards with Python over USB. 


---

<a href="../../../firmware/chimera_v2/chimera_v2.py#L16"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>function</kbd> `log_usb_devices`

```python
log_usb_devices()
```

Debug utility for printing all available usb devices. 


---

<a href="../../../firmware/chimera_v2/chimera_v2.py#L168"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `F4Dev`
Chimera access point for the F4Dev. 

<a href="../../../firmware/chimera_v2/chimera_v2.py#L170"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__() → None
```








---

<a href="../../../firmware/chimera_v2/chimera_v2.py#L155"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `adc_read`

```python
adc_read(net_name: str) → float
```

Read the voltage at an adc pin specified by the net name. 

---

<a href="../../../firmware/chimera_v2/chimera_v2.py#L124"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_read`

```python
gpio_read(net_name: str) → bool
```

Read the value of a GPIO pin given the net name of the pin, returns true if high. 

---

<a href="../../../firmware/chimera_v2/chimera_v2.py#L139"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `gpio_write`

```python
gpio_write(net_name: str, value: bool) → None
```

Write a value to the gpio pin indicated by the provided net name, true for high. 




---

_This file was automatically generated via [lazydocs](https://github.com/ml-tooling/lazydocs)._
