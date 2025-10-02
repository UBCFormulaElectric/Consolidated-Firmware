FROM python:3.11

COPY can_bus can_bus
WORKDIR /live_data
COPY software/tracksight/live_data/requirements.txt .
RUN pip3 install -r requirements.txt
RUN rm requirements.txt
# RUN rm .env

EXPOSE 5000
ENV DOCKERIZED=1