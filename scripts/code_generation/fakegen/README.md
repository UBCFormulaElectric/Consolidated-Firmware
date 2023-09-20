# fakegen

`fakegen` is a tool to generate fake function definitions based on a header file. 
"Fakes" are definitions of existing functions with no actual behavior, but allow us to manually set their 
return value, or check the number of times they've been called, usually so we can run tests on higher-level code.
This is especially useful for embedded targets, since we have to remove the hardware-dependent drivers to write
unit tests.

## Overview

For a provided header file, `generate_fake.py` will generate a header and source pair which consist of 
fakes for each function defined in the provided header, as well as functions to control the fakes.

The generated files are: 
- Fake `.hpp`: This includes functions declarations to interact with the fakes (set return value, get call count, reset the fake, etc.).
- Faked `.cpp`: Definitions for functions to interact with the fakes, as well as faked implementation of the functions from the provided header.

### Invocation

Invoke `generate_fake.py` and pass the `--header` (header to fake), `--output-header` (faked header output), and `--output-source` (faked source output) argments.  

For example:

```sh
python3 generate_fake.py --header example.h --output_header fake_example.hpp --output-source fake_example.cpp
```

## Usage

### Setting Return Values

If the function being faked returns a value, helpers will be generated to set the fake's return value.
If the function doesn't take any arguments, you can set the single return value for the function.
```c
/**
 * Set the value to be returned when <faked function> is invoked.
 */
void fake_<faked function>_returns(<faked function return value>);
```

If the function being faked takes arguments, then you must set the value to be returned when the faked function is invoked with certain arguments.
```c
/**
 * Set the value to be returned by <faked function> when invoked with a specific set of arguments.
 */
void fake_<faked function>_returnsForArgs(<faked function arguments>, <faked function return value>);
```

If the return value has not been set, the default value is binary 0, i.e. `0.0` for `float`s, `0` for `int`s, `NULL` for pointers, etc.

### Checking Call Counts

You can also check the number of times a fake has been invoked.

```c
 * Return the total number of times voidFuncParams has been invoked.
 */
fake_<faked function>_callCount(void);
```
If the function being faked takes arguments, then you can also check the number of times the fake has been invoked with specific arguments.

```c
/**
 * Return the number of times <faked function> has been invoked with a specific set of arguments.
 */
uint32_t fake_<faked function>_callCountForArgs(<faked function arguments>);
```

### Resetting Fakes

Each function being faked will generate a reset function. This must be called between tests.

```c
/**
 * Reset the fake for <faked function>.
 */
fake_<faked function>_reset(void);
```

## Implementation

`fakegen` is written in Python under the `src` directory. 
[pyclibrary](https://pyclibrary.readthedocs.io/en/latest/get_started/basic_usage.html#parsing-headers) is used to parse the provided headers, and
[jinja2](https://pypi.org/project/Jinja2/) templates are used to generate the corresponding faked `.hpp` and `.cpp`. Value-returning functions with no parameters, value-returning functions with parameters, void functions with no parameters, and void functions with parameters each have a seperate `jinja2` template under 
`src/templates`. This means we have some repeated code, but considerably simplifies the templates themselves (although they're still pretty gross).

For functions with parameters, the generated `.cpp` file generates classes with member variables representing the set of parameters.
A hashmap (called an `unordered_map` in the C++ standard library) is used to map instances of these parameters to values, allowing us 
to look up return values or call counts by a set of arguments.

