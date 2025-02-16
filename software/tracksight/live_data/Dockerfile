FROM python:3.11

# RUN apt update && apt install -y curl && apt clean

WORKDIR /live_data
COPY software/tracksight/live_data/requirements.txt .
RUN pip3 install -r requirements.txt

EXPOSE 5000

ENV IN_DOCKER_CONTAINER=true
COPY software/tracksight/live_data/app app
COPY ../../../can_bus/quadruna quadruna