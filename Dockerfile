FROM ubcuas/rustuuas:latest AS builder

RUN mkdir -p /uas/smurfette
WORKDIR /uas/smurfette

COPY Cargo.toml Cargo.lock ./
COPY src/ ./src/

RUN cargo build --release


FROM ubcuas/cppuuas:latest AS runner

RUN mkdir -p /uas/smurfette
WORKDIR /uas/smurfette

RUN apt-get update -y && apt-get install -y openssl

COPY --from=builder /uas/smurfette/target/release/smurfette /uas/smurfette/

ENTRYPOINT ["/uas/smurfette/smurfette"]
