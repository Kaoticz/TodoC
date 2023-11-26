FROM archlinux:latest
RUN pacman-key --init
RUN pacman -Syu --noconfirm base-devel gcc sqlite

WORKDIR /app

COPY . .

RUN make -f ./makefile

CMD ["./bin/main"]