FROM rust:1.92.0 AS builder

# Backend binary set up
WORKDIR /backend

RUN apt-get update && apt-get install -y \
    pkg-config \
    libudev-dev \
    clang

COPY software/tracksight/rust_backend/Cargo.toml .
COPY software/tracksight/rust_backend/src src

# Cache dependencies
RUN echo 'fn main() {}' > dummy.rs
RUN sed -i 's#src/main.rs#dummy.rs#' Cargo.toml
RUN cargo build --release
RUN sed -i 's#dummy.rs#src/main.rs#' Cargo.toml

# Verbosity option
ARG DOCKER_VERBOSITY

# Build actual backend based on verbosity
RUN if [ -n "$DOCKER_VERBOSITY" ]; then \
        if [ "$DOCKER_VERBOSITY" = "VERBOSE" ]; then \
            RUN cargo build --release --features verbose; \
        elif [ "$DOCKER_VERBOSITY" = "DEBUG" ]; then \
            RUN cargo build --release --features debug; \
        else \
            RUN cargo build --release; \
        fi; \
    else \
        RUN cargo build --release \
    fi


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
