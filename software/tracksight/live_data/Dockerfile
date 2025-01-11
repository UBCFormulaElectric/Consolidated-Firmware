FROM python:3.11

RUN apt update && apt install -y curl && apt clean

WORKDIR /live_data
COPY environment/requirements.txt .
RUN pip3 install -r requirements.txt

EXPOSE 5000

COPY software/tracksight/live_data/app ./app
