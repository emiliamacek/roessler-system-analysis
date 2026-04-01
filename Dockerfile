FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    cmake \
    g++ \
    make \
    pkg-config \
    libgmp-dev \
    libmpfr-dev \
    libboost-all-dev \
    python3 \
    python3-pip \
    python3-matplotlib \
    python3-numpy \
    python3-scipy \
    python3-pandas

RUN git clone --branch v6.0.0 --depth 1 https://github.com/CAPDGroup/CAPD.git /opt/CAPD
WORKDIR /opt/CAPD/build

# 'make install' will put capd-config in standard system paths
RUN cmake .. && make -j$(nproc) && make install

WORKDIR /app
COPY . /app

RUN make

CMD ["/bin/bash"]
