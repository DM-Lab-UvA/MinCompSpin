FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gcc \
    g++ \
    git \
    python3 \
    python3-pip \
    python3-venv \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/venv

RUN /opt/venv/bin/pip install --no-cache-dir notebook ipykernel jupyterlab jupyterlab-lsp python-lsp-server numpy scipy matplotlib seaborn pandas

ENV PATH="/opt/venv/bin:$PATH"

WORKDIR /MinCompSpin
COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

RUN cd build/mcmpy && \
    /opt/venv/bin/pip install .
