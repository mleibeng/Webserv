FROM debian:latest

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
    build-essential \
    libreadline-dev \
    g++ \
    git \
    make \
    valgrind \
    strace \
    cmake \
    php \
    php-cgi \
    python3 && \
    apt-get clean && \
    rm -fr /var/lib/apt/lists/*

WORKDIR /workspace
EXPOSE 8080 8081

CMD ["tail", "-f", "/dev/null"]