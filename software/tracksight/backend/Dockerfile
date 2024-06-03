FROM python:3.11

RUN apt update && apt install -y curl && apt clean

WORKDIR /backend
COPY environment/requirements.txt .
RUN pip3 install -r requirements.txt

EXPOSE 5000

COPY software/tracksight/backend/app ./app
