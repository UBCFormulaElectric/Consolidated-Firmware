FROM python:3.11

WORKDIR /live_data
COPY software/tracksight/live_data/requirements.txt .
RUN pip3 install -r requirements.txt
RUN rm requirements.txt

EXPOSE 5000
ENV IN_DOCKER_CONTAINER=true