# Philosphy

Docker containers here are used as a runtime environment for the code. This means that it carries the
- networking configurations
- healthchecks
- environment variables
- dependencies
- runtime configurations
for the code.

Note these yield a few important benefits
1. **More realistic development environment**: the development environment can run inside the containers as the production environment. With only a few changes in runtime configurations, the development environment can be made to mimic the production environment.
2. **Easier deployment**: With one command, all containers can be started. This means that the deployment is more reliable. Furthermore, healthchecks are conducted periodically to ensure that the containers are running as expected.
3. **Easier testing**: We can use the containers to run tests. This means that the tests are run in the same environment as the production environment. This means that the tests are more reliable.
4. **Rapid Prototyping**: You can change code on your computer, and restart the container to immediately.
5. **File Actions Transparency**: You can see all the files that are being used by the container. This way, we can more quickly debug issues that arise from the code in relation to management of files.

There are a few important design decisions made because of this philosophy:

1. **No code in the container**: The code is mounted as a volume in the container. This means that the code is not part of the container image. This is important because the code is the most frequently changing part of the system. This also means that the container can be used for multiple codebases.
2. **No environment variables in the code**: The environment variables are passed to the container at runtime. This means that the code is not dependent on the environment variables. This is important because the environment variables are the most frequently changing part of the system. This also means that the container can be used for multiple environments.
3. **No dependencies in the code**: The dependencies are installed in the container. This means that the code is not dependent on the dependencies. This is important because the dependencies are the most frequently changing part of the system. This also means that the container can be used for multiple dependencies.