# git_hash_gen
Generates a header file that exposes the current git hash (and if the hash is clean). 

## Invocation
Invoke `src/generate_commit_info.py`, and use the `--output_header` option to set the output path.

For example,
```sh
python src/generate_commit_info.py --output_header test.h 
```

This will take a couple seconds... The whole repo needs to be scanned to check if the commit is dirty or clean.

## Generated File
A sample generated header file:

```h
#pragma once

#define GIT_COMMIT_HASH "f3d0d76"
#define GIT_COMMIT_CLEAN 0
```

## Implementation
- `jinja2` for templating.
- `GitPython` for interfacing with git through Python.