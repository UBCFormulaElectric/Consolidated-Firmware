<!-- markdownlint-disable -->

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply#L0"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

# <kbd>module</kbd> `power_supply`






---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/DP821A#L4"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `DP821A`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/__init__#L5"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an abstraction around a Rigol DP821A programmable DC power supply. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/disable_output#L87"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `disable_output`

```python
disable_output(channel: int = 1)
```

Disables the output for the specified channel. 



**Args:**
 
 - <b>`channel`</b>:  Power supply channel number (default is 1). 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/enable_output#L76"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `enable_output`

```python
enable_output(channel: int = 1)
```

Enables the output for the specified channel. 



**Args:**
 
 - <b>`channel`</b>:  Power supply channel number (default is 1). 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/get_id#L102"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `get_id`

```python
get_id() → str
```

Gets the device identification string. 



**returns:**
  Device identification. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/measure_current#L62"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `measure_current`

```python
measure_current(channel: int = 1) → float
```

Measures the output current. 



**Args:**
 
 - <b>`channel`</b>:  Power supply channel number (default is 1). 



**Returns:**
 Measured current in Amps. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/measure_voltage#L48"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `measure_voltage`

```python
measure_voltage(channel: int = 1) → float
```

Measures the output voltage. 



**Args:**
 
 - <b>`channel`</b>:  Power supply channel number (default is 1). 



**Returns:**
 Measured voltage in Volts. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/reset#L98"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `reset`

```python
reset()
```

Resets the power supply to its default settings. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/set_current#L35"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `set_current`

```python
set_current(current: float, channel: int = 1)
```

Sets the current for the power supply. 



**Args:**
 
 - <b>`current`</b>:  Desired current in Amps 
 - <b>`channel`</b>:  Power supply channel number (default is 1). 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/power_supply/set_voltage#L22"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `set_voltage`

```python
set_voltage(voltage: float, channel: int = 1)
```

Sets the voltage for the power supply. 



**Args:**
 
 - <b>`voltage`</b>:  Desired voltage in Volts. 
 - <b>`channel`</b>:  Power supply channel number (default is 1). 




---

_This file was automatically generated via [lazydocs](https://github.com/ml-tooling/lazydocs)._
