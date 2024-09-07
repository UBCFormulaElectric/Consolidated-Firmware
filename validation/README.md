# Validation
This directory contains all software for electrical validation.

## Contributing
Install the python vscode extension.

Install Poetry (https://python-poetry.org/docs/#installing-with-the-official-installer),
```sh
curl -sSL https://install.python-poetry.org | python3 -
```

Add this to your bashrc/zshrc/zprofile
```sh
poetry config virtualenvs.in-project true
```

- Go to preferences - open user settings on VSCode
- Search for python
- Go to Extensions, Python on the left hand side
- Disable Python > Terminal: Activate Enviornment
- Restart vscode

Install dependencies
```sh
poetry install --with dev --no-root
```

Set the virtual enviornment python interpreter to the one vscode uses for indexing (https://stackoverflow.com/questions/75567769/vscode-does-not-recognize-my-flask-dependency-installed-with-poetry).