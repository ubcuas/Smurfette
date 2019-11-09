# UBC UAS Dockerfile - Smurfette


## Prebuild image
FROM alpine:latest AS build
RUN mkdir -p /uas/smurfette/build
WORKDIR /uas/smurfette/build

RUN apk add --no-cache build-base cmake libpq postgresql-dev boost-dev linux-headers

COPY src/ ./
RUN cmake . && make -j4


## Runtime image
FROM alpine:latest AS run
RUN mkdir -p /uas/smurfette
WORKDIR /uas/smurfette

RUN apk add --no-cache libstdc++ libpq boost

COPY --from=build /uas/smurfette/build/smurfette ./
