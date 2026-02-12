# Copyright (c) DarkEmu
# Linux container build for DarkEmu servers.

FROM ubuntu:22.04 AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        cmake \
        g++ \
        make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel

FROM ubuntu:22.04 AS runtime

RUN useradd -m darkemu

WORKDIR /app
COPY --from=build /app/build /app/build
COPY --from=build /app/server/Connect/Data /app/server/Connect/Data

USER darkemu

EXPOSE 44405 55901

CMD ["/app/build/server/Connect/DarkheimCS"]

