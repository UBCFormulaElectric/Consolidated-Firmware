# ENV FLASK_APP=app.py
# ENV FLASK_RUN_HOST=0.0.0.0
FROM python:3.11-slim

# for healthcheck
RUN apt update && apt install -y curl && apt clean

USER root
WORKDIR /backend
RUN pip install pipenv

WORKDIR /backend
COPY Pipfile .
COPY Pipfile.lock .
RUN pipenv install --system --deploy

WORKDIR /backend
COPY ./software/tracksight/backend/ .

USER root
WORKDIR /backend
EXPOSE 5000
CMD ["python", "app/telemetry.py"]