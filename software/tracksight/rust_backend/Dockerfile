FROM rust:1.92.0 AS builder

# Backend binary set up
WORKDIR /backend

RUN apt-get update && apt-get install -y \
    pkg-config \
    libudev-dev

COPY software/tracksight/rust_backend/Cargo.toml software/tracksight/rust_backend/Cargo.lock .
COPY software/tracksight/rust_backend/src src

RUN cargo build --release

# Runtime set up
FROM debian:bookworm

RUN apt-get update && apt-get install -y \
    libssl3 \
    libudev1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /backend

COPY --from=builder /backend/target/release/backend /usr/local/bin/backend
COPY can_bus /backend/can_bus

ENV DOCKER=true

CMD ["backend"]