# commit_info_gen
Generates a header file that exposes some info about the current commit:
- Short commit hash.
- If the commit is clean.

## Invocation
Invoke `src/generate_commit_info.py`, and use the `--output_header` option to set the output path.

For example,
```sh
python3 src/generate_commit_info.py --output-header example_header.h 
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

## Implementation
- `jinja2` for templating.
- `GitPython` for interfacing with git through Python.