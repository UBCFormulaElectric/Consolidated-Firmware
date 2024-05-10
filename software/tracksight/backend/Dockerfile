FROM continuumio/miniconda3
ARG CONDA_COMMAND="conda run --no-capture-output -p /opt/conda/envs/tracksight-backend"
# ENV FLASK_APP=app.py
# ENV FLASK_RUN_HOST=0.0.0.0
# RUN apk add --no-cache gcc musl-dev linux-headers

USER root
WORKDIR /backend
COPY ./environment.yml ./environment.yml
RUN conda env create -f ./environment.yml
RUN echo "source activate telemetry-database" > ~/.bashrc
ENV PATH /opt/conda/envs/telemetry-database/bin:$PATH

USER root
WORKDIR /backend
COPY . .
EXPOSE 5000
CMD ["conda", "run", "--no-capture-output", "-p", "/opt/conda/envs/tracksight-backend", "python", "app/telemetry.py"]