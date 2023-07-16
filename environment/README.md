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
docker build . -t ubcformulaelectric/ci
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
docker push ubcformulaelectric/ci
```

To pull the container, run: 

```bash
docker pull ubcformulaelectric/ci
```

## Run Container Locally

Running a container locally is extremely useful for testing stuff out and debugging problems. 
The `run_container.sh` script pulls the latest CI image from the registry (if it doesn't exist locally),
then starts the container and mounts the local repo into the container.

This means that the repo files are shared between your local machine and the container: If you edit a file in the container, 
it will be modified locally, and vice-versa.

To run the container, `cd` into this directory and run:

```bash
./run_container.sh
```

This should start the container and open up a terminal. 

**Note:** VSCode supports containers via the "Dev Containers" extension, I'd recommend it if you're planning to do any development in the container.
