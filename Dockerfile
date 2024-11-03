FROM alpine:latest
RUN apk update && \
    apk add --no-cache \
    build-base \
    gcc \
    g++ \
    make \
    valgrind
WORKDIR /workspace
# docker build -t cpp-dev-env .
# docker run -it --rm -v /Users/{$USER}/code/Webserv:/workspace cpp-dev-env
# docker run -it --rm -v /Users/{$USER}/code/Webserv:/workspace -p 8080:8080 cpp-dev-env
# valgrind --leak-check=full ./<program> <parameters>
# valgrind --tool=helgrind ./<program> <parameters>
