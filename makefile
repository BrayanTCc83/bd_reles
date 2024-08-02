SRC=./src/
MODELS=$(SRC)models/
UTILS=$(SRC)utils/
STRUCT=$(SRC)struct/
TYPES=$(SRC)types/
CRYPTO=$(SRC)crypto/
PATTERN=$(SRC)pattern/
BUILD=./build/

.PHONY: run

run: $(BUILD)minidb.exe
	./build/minidb.exe

$(BUILD)minidb.exe: $(SRC)main.c $(MODELS)*.c $(UTILS)*.c $(TYPES)*.c $(STRUCT)*.c $(CRYPTO)*.c $(UTILS)utils.H $(PATTERN)pattern.h
	gcc -o $(BUILD)minidb.exe $(SRC)main.c $(MODELS)*.c $(UTILS)*.c $(TYPES)*.c $(STRUCT)*.c $(CRYPTO)*.c $(PATTERN)*.c -L$(MODELS)models.h -L$(UTILS)utils.h -L$(TYPES)types.h -L$(STRUCT)structs.h -L$(CRYPTO)crypto.h -L$(PATTERN)pattern.h -std=c2x
