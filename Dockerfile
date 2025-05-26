FROM alpine AS builder

WORKDIR /app

COPY . /app

RUN apk curl-dev g++ cmake make git&& \
	