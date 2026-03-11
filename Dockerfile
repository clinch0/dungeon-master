FROM alpine:3.19 AS builder
RUN apk add --no-cache build-base
WORKDIR /build
COPY dungeon.c .
RUN gcc -Os -s -static dungeon.c -o dungeon && strip -s dungeon

FROM scratch
COPY --from=builder /build/dungeon /dungeon
CMD ["/dungeon"]
