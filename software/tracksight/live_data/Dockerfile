FROM python:3.11

# can_bus used to configure json can
COPY can_bus can_bus

WORKDIR /live_data
COPY software/tracksight/live_data/requirements.txt .
RUN pip3 install -r requirements.txt
RUN rm requirements.txt

EXPOSE $BACKEND_PORT
ENV DOCKERIZED=1