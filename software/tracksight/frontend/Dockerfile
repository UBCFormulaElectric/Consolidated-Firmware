FROM node:22

WORKDIR /frontend
COPY software/tracksight/frontend .
# This is a temporary fix, seems like we need to upgrade our dependencies
RUN npm install ----legacy-peer-deps
RUN npm run build

EXPOSE $FRONTEND_PORT
