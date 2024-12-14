FROM centos
RUN cd ~ \
    && apk add zlib-dev \
    && apk add openssl-dev \
    && apk add curl-dev \
    #tool
    && apk add git \
    && apk add cmake \
    && apk add make \