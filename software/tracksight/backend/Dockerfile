# ENV FLASK_APP=app.py
# ENV FLASK_RUN_HOST=0.0.0.0
FROM python:3.11

USER root
WORKDIR /backend
RUN pip install pipenv

WORKDIR /backend
COPY Pipfile .
COPY Pipfile.lock .
RUN pipenv install --system --deploy

USER root
WORKDIR /backend
COPY ./software/tracksight/backend/ .
EXPOSE 5000
CMD ["python", "app/telemetry.py"]