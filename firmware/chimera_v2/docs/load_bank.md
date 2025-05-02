<!-- markdownlint-disable -->

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank#L0"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

# <kbd>module</kbd> `load_bank`






---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/DL3021#L4"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `DL3021`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/__init__#L5"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an abstraction around a Rigol DL3021 programmable DC electronic load. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/disable_load#L101"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `disable_load`

```python
disable_load()
```

Disables the load input. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/enable_load#L97"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `enable_load`

```python
enable_load()
```

Enables the load input. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/get_id#L109"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `get_id`

```python
get_id() → str
```

Gets the device identification string. 



**returns:**
  Device identification. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/measure_current#L77"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `measure_current`

```python
measure_current() → float
```

Measures the input current. 



**Returns:**
  Measured current in Amps. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/measure_power#L87"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `measure_power`

```python
measure_power() → float
```

Measures the input power. 



**Returns:**
  Measured power in Watts. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/measure_voltage#L66"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `measure_voltage`

```python
measure_voltage() → float
```

Measures the input voltage. 



**Returns:**
  Measured voltage in Volts. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/reset#L105"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `reset`

```python
reset()
```

Resets the load to its default settings. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/set_current#L22"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `set_current`

```python
set_current(current: float)
```

Sets the current for the load in constant current (CC) mode. 



**Args:**
 
 - <b>`current`</b>:  Desired current in Amps. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/set_power#L55"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `set_power`

```python
set_power(power: float)
```

Sets the power for the load in constant power (CP) mode. 



**Args:**
 
 - <b>`power`</b>:  Ddesired power in Watts. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/set_resistance#L44"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `set_resistance`

```python
set_resistance(resistance: float)
```

Sets the resistance for the load in constant resistance (CR) mode. 



**Args:**
 
 - <b>`resistance`</b>:  Desired resistance in Ohms. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/load_bank/set_voltage#L33"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `set_voltage`

```python
set_voltage(voltage: float)
```

Sets the voltage for the load in constant voltage (CV) mode. 



**Args:**
 
 - <b>`voltage`</b>:  Desired voltage in Volts. 




---

_This file was automatically generated via [lazydocs](https://github.com/ml-tooling/lazydocs)._
