# Validation
This directory contains all python-facing software for electrical validation. These tools include
- [`chimera`](./tools/chimera): TODO DESCRIPTION HERE
- [`load_bank`](./tools/load_bank): TODO DESCRIPTION HERE
- [`power_supply`](./tools/power_supply): TODO DESCRIPTION HERE

## Getting Started
The validation team uses Poetry as their package manager on the Python end.

Start by [installing Poetry](ttps://python-poetry.org/docs/#installing-with-the-official-installer),
```sh
curl -sSL https://install.python-poetry.org | python3 -
```

Run 
```sh
poetry --version
```
to confirm installation

We want to keep our Python virtual enviornment in the project directory. To do this, add 
```sh
poetry config virtualenvs.in-project true
```
to the end of your `~/.zprofile`/`~/.zshrc` (Zsh), or `~/.bashrc` (Bash).

Now close your current terminal, and open a new one.

You can now navigate to this directory [`Consolidated-Firmware/validation`](.), and install dependencies,
```sh
poetry install --with dev --no-root
```

## VSCode Quality of Life
Install the [Python VSCode extension](https://marketplace.visualstudio.com/items?itemName=ms-python.python) if you haven't already.

The [Even Better TOML extension](https://marketplace.visualstudio.com/items?itemName=tamasfe.even-better-toml) is also a nice to have ofr managing `pyproject.toml`.

- Hit `ctrl` + `shift` + `p`
- Search for `Preferences: Open User Settings` and press `enter`
- Search for "python" in the top, `search settings` bar
- Open `Extensions`, `Python` on the left hand side
- Disable `Python > Terminal: Activate Enviornment`
- Restart VSCode

To set up package indexing, run
```sh
poetry env info
```

It will look something like:
```sh
Virtualenv
Python:         3.10.12
Implementation: CPython
Path:           /home/liamilan/projects/Consolidated-Firmware/validation/.venv
Executable:     /home/liamilan/projects/Consolidated-Firmware/validation/.venv/bin/python
Valid:          True

Base
Platform:   linux
OS:         posix
Python:     3.10.12
Path:       /usr
Executable: /usr/bin/python3.10
```

Copy the path under `VirtualEnv`, `Executable:` (eg. `/home/liamilan/projects/Consolidated-Firmware/validation/.venv/bin/python`).

Open a python file, and on the bottom right, to the left of the bell in VSCode, click where the Python version number is specified (eg. `3.10.12 64-bit`). Click `enter interpreter path`, and paste the path you copied before.

More info can be found here: https://stackoverflow.com/questions/75567769/vscode-does-not-recognize-my-flask-dependency-installed-with-poetry