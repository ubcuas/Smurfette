FROM ubcuas/rustuuas:latest as build

RUN mkdir -p /uas/smurfette
WORKDIR /uas/smurfette

COPY Cargo.toml Cargo.lock ./
COPY src/ ./src/

RUN cargo build --release


FROM ubcuas/rustuuas:latest as runner

RUN mkdir -p /uas/smurfette
WORKDIR /uas/smurfette

COPY --from=build /uas/smurfette/target/release/smurfette /uas/smurfette/

ENTRYPOINT ["/uas/smurfette/smurfette"]
