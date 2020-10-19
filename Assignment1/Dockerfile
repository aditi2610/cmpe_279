FROM ubuntu:latest
RUN apt-get update
RUN apt-get install -y --no-install-recommends apt-utils build-essential sudo git
RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
USER docker
WORKDIR /code
COPY . .
CMD ["sleep", "10000000"]