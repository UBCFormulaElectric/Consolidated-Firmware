# CI Docker Container

We run our CI (Continuous Integration) pipeline via Github Actions inside a Docker container, defined via the `Dockerfile` in this directory. The `Dockerfile` installs all dependencies required to generate code with STM32CubeMX, build ARM binaries, and build and run tests. This allows for:

1. Fast CI builds: The CI machines don't need to do any setup work since everything is already pre-installed in the container, and Docker containers are extremely lightweight.
2. Reproducibility: Using a container ensures that builds are reliable and fully independent of each other.
3. Easier debugging: Debugging problems in a container is easier compared to keeping all setup in the Github Actions workflow `.yml` file, since you can run containers locally.

## Prerequisites

All following steps require Docker Desktop. Install from https://www.docker.com/products/docker-desktop/. Also, make sure Docker Desktop is running.

All following steps should be independent of OS.

## Build Container

If you've modified the container, `cd` into this directory and build via:

```bash
docker build . -t ubcformulaelectric/environment
```

## Push/Pull Container

The Github Actions CI pulls the latest container image automatically from our Docker Hub registry. 

### Registry Info
Username: ubcformulaelectric \
Email: electrical@ubcformulaelectric.com \
Password: Ask a lead.

### Commands

If you've rebuilt the container and want to push a new version, run:

```bash
# Follow prompts to login
docker login 
docker push ubcformulaelectric/environment
```

To pull the container, run: 

```bash
docker pull ubcformulaelectric/environment
```
