# ==============================================================================
# Multi-Stage Dockerfile for C++ Google Ping-Pong Game
# ==============================================================================
# TUTORIAL CONCEPTS:
# Multi-stage Docker builds separate build dependencies (compilers, headers)
# from the runtime image, drastically reducing output image size and improving security.
# ==============================================================================

# STAGE 1: Build Environment
FROM ubuntu:24.04 AS builder

# Install C++ compiler toolchain & CMake
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source repository
COPY . .

# Configure and build release binary
RUN cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release \
    && ctest --test-dir build --output-on-failure

# STAGE 2: Lightweight Minimal Runtime Environment
FROM ubuntu:24.04 AS runtime

# Install runtime terminal libraries
RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled binary from builder stage
COPY --from=builder /app/build/pingpong /app/pingpong

# Allocate interactive pseudo-TTY for ANSI terminal rendering
ENV TERM=xterm-256color

ENTRYPOINT ["/app/pingpong"]
