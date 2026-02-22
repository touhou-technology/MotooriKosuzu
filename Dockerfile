FROM alpine AS builder

WORKDIR /app

COPY ./app

RUN apk add curl-dev g++ cmake make git&& \
	mkdir build; cd build; cmake -G Ninja ..;
