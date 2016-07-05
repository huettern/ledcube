ARCHFLAGS=-mthumb -mcpu=cortex-m0plus
CFLAGS=
LDFLAGS=--specs=nano.specs -Wl,--gc-sections,-Map,$(TARGET).map,-Tlink.ld

BUILDPATH=/usr/local/gcc-arm-none-eabi-5_4-2016q2/bin/

CC=$(BUILDPATH)arm-none-eabi-gcc
LD=$(BUILDPATH)arm-none-eabi-gcc
OBJCOPY=$(BUILDPATH)arm-none-eabi-objcopy
SIZE=$(BUILDPATH)arm-none-eabi-size
RM=rm -f

TARGET=main
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

all: build size
build: elf srec
elf: $(TARGET).elf
srec: $(TARGET).srec

clean:
	$(RM) $(TARGET).srec $(TARGET).elf $(TARGET).map $(OBJ)

%.o: %.c
	$(CC) -c $(ARCHFLAGS) $(CFLAGS) -o $@ $<

$(TARGET).elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

%.srec: %.elf
	$(OBJCOPY) -O srec $< $@

size:
	$(SIZE) $(TARGET).elf
