FROM node:22

WORKDIR /frontend
COPY software/tracksight/frontend .
COPY software/tracksight/docker/envs/frontend.env .env.local
# This is a temporary fix, seems like we need to upgrade our dependencies
RUN npm install ----legacy-peer-deps
RUN npm run build

ENV DOCKER=true

EXPOSE $FRONTEND_PORT
