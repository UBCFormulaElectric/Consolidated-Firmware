FROM node:22

WORKDIR /frontend
COPY software/tracksight/frontend .
RUN npm ci
RUN npm run build

EXPOSE $FRONTEND_PORT
