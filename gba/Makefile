DEVKITPRO ?= /opt/devkitpro
DEVKITARM ?= $(DEVKITPRO)/devkitARM
CXX := $(DEVKITARM)/bin/arm-none-eabi-g++
OBJCOPY := $(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX := $(DEVKITPRO)/tools/bin/gbafix
FLAGS := -mcpu=arm7tdmi -mthumb -mthumb-interwork -std=c++17 -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics
.PHONY: all clean
all: build/Sigilbound_GBA_Battle.gba
build:
	mkdir -p build
build/main.o: source/main.cpp include/battle.hpp | build
	$(CXX) $(FLAGS) -Iinclude -c $< -o $@
build/Sigilbound_GBA_Battle.elf: build/main.o
	$(CXX) -mcpu=arm7tdmi -mthumb -mthumb-interwork -specs=gba.specs $< -o $@
build/Sigilbound_GBA_Battle.gba: build/Sigilbound_GBA_Battle.elf
	$(OBJCOPY) -O binary $< $@
	$(GBAFIX) $@ -tSIGILBOUND -cSBND -mNO
clean:
	rm -rf build
