BENCHMARK = aes
#BENCHMARK = rsa
#BENCHMARK = sha-saddi
#BENCHMARK = sha-gladman

GCC_DIR =  ~/ti/msp430-gcc/bin
SUPPORT_FILE_DIRECTORY = ~/ti/msp430-gcc/include
CC      = $(GCC_DIR)/msp430-elf-gcc

OBJDIR  = bin
RM      = rm -rf

ifeq ($(BENCHMARK), aes)
SRCDIR  = src/aes_ipe
OBJECTS = $(OBJDIR)/aes-test.out
else ifeq ($(BENCHMARK), rsa)
SRCDIR  = src/rsa_ipe
OBJECTS = $(OBJDIR)/rsa-test.out
else ifeq ($(BENCHMARK), sha-saddi)
SRCDIR  = src/sha256_ipe
OBJECTS = $(OBJDIR)/sha-saddi-test.out
else ifeq ($(BENCHMARK), sha-gladman)
SRCDIR  = src/sha256_gladman_ipe
OBJECTS = $(OBJDIR)/sha-gladman-test.out
endif

OPTFLAG = -O0
#OPTFLAG = -O1
#OPTFLAG = -O2
#OPTFLAG = -O3
#OPTFLAG = -Os
#OPTFLAG = -Ofast
#OPTFLAG = -Og
CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=msp430fr5994 -D_GNU_ASSEMBLER -Wall -g -fdata-sections -ffunction-sections -T $(SRCDIR)/msp430fr5994.ld -ffreestanding -mlarge -fverbose-asm -w
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY)

all: $(OBJECTS)

$(OBJDIR)/aes-test.out: $(SRCDIR)/cs.c $(SRCDIR)/eusci_a_uart.c $(SRCDIR)/gpio.c $(SRCDIR)/pmm.c $(SRCDIR)/sfr.c $(SRCDIR)/timer_b.c $(SRCDIR)/wdt_a.c $(SRCDIR)/aes.c $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(OPTFLAG) $(LFLAGS) $^ -o $@
	
$(OBJDIR)/rsa-test.out: $(SRCDIR)/cs.c $(SRCDIR)/eusci_a_uart.c $(SRCDIR)/gpio.c $(SRCDIR)/pmm.c $(SRCDIR)/sfr.c $(SRCDIR)/timer_b.c $(SRCDIR)/wdt_a.c $(SRCDIR)/codebase/rsa.c $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(OPTFLAG) $(LFLAGS) $^ -o $@
	
$(OBJDIR)/sha-saddi-test.out: $(SRCDIR)/cs.c $(SRCDIR)/eusci_a_uart.c $(SRCDIR)/gpio.c $(SRCDIR)/pmm.c $(SRCDIR)/sfr.c $(SRCDIR)/timer_b.c $(SRCDIR)/wdt_a.c $(SRCDIR)/saddi/sha256.c $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(OPTFLAG) $(LFLAGS) $^ -o $@
	
$(OBJDIR)/sha-gladman-test.out: $(SRCDIR)/cs.c $(SRCDIR)/eusci_a_uart.c $(SRCDIR)/gpio.c $(SRCDIR)/pmm.c $(SRCDIR)/sfr.c $(SRCDIR)/timer_b.c $(SRCDIR)/wdt_a.c $(SRCDIR)/gladman/sha2.c $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(OPTFLAG) $(LFLAGS) $^ -o $@

clean:
	$(RM) bin/*.out
