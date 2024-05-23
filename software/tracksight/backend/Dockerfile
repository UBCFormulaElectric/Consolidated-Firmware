FROM python:3.11

RUN apt update && apt install -y curl && apt clean

WORKDIR /backend
COPY software/tracksight/backend/requirements.txt .
RUN pip3 install -r requirements.txt

COPY software/tracksight/backend/ .

EXPOSE 5000
CMD ["python", "app/telemetry.py"]