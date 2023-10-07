# git_hash_gen
Generates a header file that exposes the current git hash (and if the hash is clean).

## WIP
1) generate a header file with: ✅
  - the short commit hash (`git rev-parse --short HEAD`) exposed under `GIT_COMMIT_HASH`
  - whether or not the commit is clean (see `git status`) exposed under `GIT_COMMIT_CLEAN`
2) invoke `generate_commit_info.py` on compile, and add header file to dependencies
  - currently the generated file is generated in the same directory as this readme.

## Invocation
To generate the header file, run
```bash
python src/generate_commit_info.py
```

This will take a couple seconds... The whole repo needs to be scanned to check if we are dirty or clean.

## Generated File
A sample generated header file:

```h
#define GIT_COMMIT_HASH "f3d0d76"
#define GIT_COMMIT_CLEAN 0
```