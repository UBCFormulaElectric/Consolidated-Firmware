# commit_info_gen
Generates a header file and corresponding source file that exposes some info about the current commit:
- Short commit hash.
- If the commit is clean.

## Invocation
Invoke `src/generate_commit_info.py`, and use the `--output-directory`, `--output-header`, `--output-source` options to set the output paths.

For example,
```sh
python3 src/generate_commit_info.py --output-directory ./ --output-header example_header.h --output-source example_source.c 
```

## Generated File
The generated header file exposes:
- `GIT_COMMIT_HASH`, eg. `0xe6dd7a7`
- `GIT_COMMIT_CLEAN`, which is `0` for dirty, and `1` for clean

A sample generated header file:

```h
#pragma once

#define GIT_COMMIT_HASH 0xe6dd7a7
#define GIT_COMMIT_CLEAN 0
```

The generated source file looks like:

```c
#include "example_header.h"
/* 
  This file is intentionally empty,
  it only exists to tell cmake to scan example_header.h
*/
```

## Implementation
- `jinja2` for templating.
- `GitPython` for interfacing with git through Python.