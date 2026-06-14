FROM ubuntu:24.04

# Avoid timezone prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install compilation tools and build dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    libssl-dev \
    nlohmann-json3-dev \
    ca-certificates \
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory to /app (mounted from host)
WORKDIR /app

# Default command builds the project
CMD ["bash", "-c", "mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build ."]
