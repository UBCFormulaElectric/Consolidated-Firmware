<!-- markdownlint-disable -->

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope#L0"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

# <kbd>module</kbd> `scope`






---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/DS1054Z#L11"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

## <kbd>class</kbd> `DS1054Z`




<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/__init__#L20"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `__init__`

```python
__init__()
```

Create an abstraction around a Rigol DS1054Z oscilloscope. 




---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/channel_all_on#L80"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `channel_all_on`

```python
channel_all_on()
```

Turns on all channels. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/channel_off#L59"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `channel_off`

```python
channel_off(channel: Channel)
```

Turns off the given channel. 



**Args:**
 
 - <b>`channel`</b>:  Scope channel to turn off. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/channel_on#L49"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `channel_on`

```python
channel_on(channel: Channel)
```

Turns on the given channel. 



**Args:**
 
 - <b>`channel`</b>:  Scope channel to turn on. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/channel_only_on#L69"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `channel_only_on`

```python
channel_only_on(channel: Channel)
```

Turns on the given channel and turns off all of the other channels. 



**Args:**
 
 - <b>`channel`</b>:  Scope channel to keep on. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/channel_read_and_save_all_data#L313"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `channel_read_and_save_all_data`

```python
channel_read_and_save_all_data(file_path: str | None = None)
```

Save .csv file with time vs. voltage data for all channels. 



**Args:**
 
 - <b>`file_path`</b>:  CSV path to save data. Will default to timestamp based file name if None. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/channel_read_and_save_data#L289"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `channel_read_and_save_data`

```python
channel_read_and_save_data(channel: Channel, file_path: str | None = None)
```

Save .csv file with time vs. voltage data for a given channel. 



**Args:**
 
 - <b>`channel`</b>:  Target channel. 
 - <b>`file_path`</b>:  CSV path to save data. Will default to timestamp based file name if None. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/channel_read_data#L209"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `channel_read_data`

```python
channel_read_data(channel: Channel) → tuple[ndarray, ndarray]
```

Collect voltage vs. time data from a channel. 



**Args:**
 
 - <b>`channel`</b>:  Target channel. 



**Returns:**
 A tuple, whose first item is a Numpy array of times, and second item is the corresponding voltage values. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/get_data_premable#L185"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `get_data_premable`

```python
get_data_premable() → DataPreamble
```

Get information about oscilloscope axes. 



**Returns:**
  Info about oscilloscope axes. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/horizontal_scale#L148"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `horizontal_scale`

```python
horizontal_scale(scale: float)
```

Sets the horizontal scale. 



**Args:**
 
 - <b>`scale`</b>:  Seconds per division. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/offset#L85"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `offset`

```python
offset(channel: Channel, value: float)
```

Sets the DC offset of the given channel. 



**Args:**
 
 - <b>`channel`</b>:  Scope channel to target. 
 - <b>`value`</b>:  Offset to set, in volts. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/run#L41"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `run`

```python
run()
```

Run the scope. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/screenshot#L96"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `screenshot`

```python
screenshot(file_path: str | None = None)
```

Save a png screenshot. 



**Args:**
 
 - <b>`file_path`</b>:  Png path to save image. Will default to timestamp based file name if None. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/single#L45"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `single`

```python
single()
```

Set a single capture trigger 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/stop#L37"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `stop`

```python
stop()
```

Stop the scope. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/vertical_scale#L123"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `vertical_scale`

```python
vertical_scale(channel: Channel, scale: float)
```

Set the vertical scale of a channel. 



**Args:**
 
 - <b>`channel`</b>:  Channel to target. 
 - <b>`scale`</b>:  Scale in Volts, 10mV to 100V. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/vertical_scale_all#L136"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `vertical_scale_all`

```python
vertical_scale_all(scale: float)
```

Set the vertical scale of all channels. 



**Args:**
 
 - <b>`scale`</b>:  Scale in Volts, 10mV to 100V. 

---

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/tree/master/firmware/chimera_v2/scope/write#L159"><img align="right" style="float:right;" src="https://img.shields.io/badge/-source-cccccc?style=flat-square"></a>

### <kbd>method</kbd> `write`

```python
write(message: str)
```

Write a command directly to the scope. 



**Args:**
 
 - <b>`message`</b>:  Message to pass to the scope. 




---

_This file was automatically generated via [lazydocs](https://github.com/ml-tooling/lazydocs)._
