TOP ?= .

all: totu.bin

#####
#
# The TI PRU assembler looks like it has macros and includes,
# but it really doesn't.  So instead we use cpp to pre-process the
# file and then strip out all of the directives that it adds.
# PASM also doesn't handle multiple statements per line, so we
# insert hard newline characters for every ; in the file.
#
PASM_DIR ?= $(TOP)/am335x/pasm
PASM := $(PASM_DIR)/pasm

%.bin: %.p $(PASM)
	$(CPP) - < $< | perl -p -e 's/^#.*//; s/;/\n/g; s/BYTE\((\d+)\)/t\1/g' > /tmp/$(basename $<).i
	$(PASM) -V3 -b /tmp/$(basename $<).i $(basename $@)
	$(RM) /tmp/$(basename $<).i

$(PASM):
	$(MAKE) -C $(PASM_DIR)

.PHONY: clean

clean:
	rm -rf $(TOP)/*.bin

