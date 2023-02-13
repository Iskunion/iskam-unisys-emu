## specify this
ISKAM ?= /home/litrehinn/Documents/Programs/iskam-unisys-emu/iskam-unisys
ISKEMU ?= /home/litrehinn/Documents/Programs/iskam-unisys-emu/iskemu
ISKEMU_FLAG = -l $(DST_DIR)/iskemu-unisys.log
ISKEMU_INTERPRETER = $(ISKEMU)/build/iskemu-unisys

ifndef NOBAT
	ISKEMU_FLAG += -b
endif

include $(ISKAM)/Makefile

run: image
	@$(MAKE) -C $(ISKEMU)
	@$(ISKEMU_INTERPRETER) $(ISKEMU_FLAG) $(IMAGE).bin

count:
	@echo lines: `find . | grep -e \\\.[ch]| xargs cat | grep -v // | grep -v ^$$ | wc -l`