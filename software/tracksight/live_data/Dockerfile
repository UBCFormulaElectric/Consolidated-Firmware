FROM python:3.11

# RUN apt update && apt install -y curl && apt clean

WORKDIR /live_data
COPY requirements.txt .
RUN pip3 install -r requirements.txt

EXPOSE 5000

COPY ./app ./app
COPY ./live_data.env .