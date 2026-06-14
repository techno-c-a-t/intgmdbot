FROM ubuntu:24.04

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install minimal runtime dependencies (SSL certificates and OpenSSL runtime)
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libssl3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the built binary and configurations from the host's directory
COPY build/intgmdbot ./
COPY config.json ./
COPY secrets.json ./

# Run the bot
CMD ["./intgmdbot"]
