FROM ubuntu:16.04

# Install dependencies
RUN apt-get update
RUN apt-get install -y debhelper \
    dh-autoreconf \
    autotools-dev \
    autoconf-archive \
    doxygen \
    graphviz \
    libasound2-dev \
	libtool \
    libjpeg-dev \
    libqt4-dev \
    libqt4-opengl-dev \
    libudev-dev \
    libx11-dev \
	pkg-config \
    udev \
    make \
    gcc \
    git

WORKDIR /app

COPY . .

# Build
RUN ./bootstrap.sh
RUN ./configure
RUN make

CMD [ "bash" ]