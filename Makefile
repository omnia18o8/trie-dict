WARNS := -Wall -Wextra -Wfloat-equal -Wvla -std=c99 -Wpedantic
DEBUG := $(WARNS) -fsanitize=undefined -fsanitize=address -g3
OPTIM := $(WARNS) -O3
.PHONY: all run rund clean

all: t27 t27_d

t27: t27.c td27.h driver.c test.c
	gcc driver.c t27.c test.c $(OPTIM) -o t27

t27_d: t27.c td27.h driver.c test.c
	gcc driver.c t27.c test.c $(DEBUG) -o t27_d

run: t27
	./t27

rund: t27_d
	./t27_d

ext: ext.c driverext.c ext.h testext.c
	gcc driverext.c ext.c testext.c -I. $(OPTIM) -o ext

clean:
	rm -f t27 t27_d ext
