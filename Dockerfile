FROM centos
RUN cd ~ \
    && apk add zlib-dev \
    && apk add openssl-dev \
    #tool
    && apk add git \
    && apk add cmake \
    && apk add make \