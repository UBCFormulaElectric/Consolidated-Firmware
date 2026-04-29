FROM node:22

WORKDIR /frontend
COPY software/tracksight/frontend .
RUN corepack enable && yarn install --frozen-lockfile
RUN yarn build

EXPOSE $FRONTEND_PORT
