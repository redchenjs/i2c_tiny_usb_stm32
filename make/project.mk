#
# Common make definition
#

# Assembly files can be name with upper case .S, convert it to .s
SRC_S := $(SRC_S:.S=.s)

# Due to GCC LTO bug https://bugs.launchpad.net/gcc-arm-embedded/+bug/1747966
# assembly file should be placed first in linking order
OBJ += $(addprefix $(BUILD)/obj/, $(SRC_S:.s=.o))
OBJ += $(addprefix $(BUILD)/obj/, $(SRC_C:.c=.o))

# Verbose mode
ifeq ("$(V)", "1")
$(info CFLAGS  $(CFLAGS))  $(info )
$(info LDFLAGS $(LDFLAGS)) $(info )
$(info ASFLAGS $(ASFLAGS)) $(info )
endif

# Set all as default goal
.DEFAULT_GOAL := all
all: $(BUILD)/$(PROJECT_NAME).bin $(BUILD)/$(PROJECT_NAME).hex size

OBJ_DIRS = $(sort $(dir $(OBJ)))
$(OBJ): | $(OBJ_DIRS)
$(OBJ_DIRS):
	@$(MKDIR) -p $@

$(BUILD)/$(PROJECT_NAME).elf: $(OBJ)
	@echo LINK $@
	@$(CC) -o $@ $(LDFLAGS) $^ -Wl,--start-group $(LIBS) -Wl,--end-group

$(BUILD)/$(PROJECT_NAME).bin: $(BUILD)/$(PROJECT_NAME).elf
	@echo CREATE $@
	@$(OBJCOPY) -O binary $^ $@

$(BUILD)/$(PROJECT_NAME).hex: $(BUILD)/$(PROJECT_NAME).elf
	@echo CREATE $@
	@$(OBJCOPY) -O ihex $^ $@

# We set vpath to point to the top of the tree so that the source files
# can be located. By following this scheme, it allows a single build rule
# to be used to compile all .c files.
vpath %.c . $(TOP)
$(BUILD)/obj/%.o: %.c
	@echo CC $(notdir $@)
	@$(CC) $(CFLAGS) -c -MD -o $@ $<
	@# The following fixes the dependency file.
	@# See http://make.paulandlesley.org/autodep.html for details.
	@# Regex adjusted from the above to play better with Windows paths, etc.
	@$(CP) $(@:.o=.d) $(@:.o=.P); \
	  $(SED) -e 's/#.*//' -e 's/^.*:  *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.d) >> $(@:.o=.P); \
	  $(RM) $(@:.o=.d)

# ASM sources lower case .s
vpath %.s . $(TOP)
$(BUILD)/obj/%.o: %.s
	@echo AS $(notdir $@)
	@$(CC) -x assembler-with-cpp $(ASFLAGS) -c -o $@ $<

# ASM sources upper case .S
vpath %.S . $(TOP)
$(BUILD)/obj/%.o: %.S
	@echo AS $(notdir $@)
	@$(CC) -x assembler-with-cpp $(ASFLAGS) -c -o $@ $<

size: $(BUILD)/$(PROJECT_NAME).elf
	-@echo ''
	@$(SIZE) $<
	-@echo ''

clean:
	rm -rf $(BUILD)

OPENOCD_IF ?= jlink
OPENOCD_TP ?= swd

flash: $(BUILD)/$(PROJECT_NAME).hex
	@echo adapter driver $(OPENOCD_IF) > $(BUILD)/openocd.cfg
	@echo transport select $(OPENOCD_TP) >> $(BUILD)/openocd.cfg
	@echo source [find target/stm32f1x.cfg] >> $(BUILD)/openocd.cfg
	@echo init >> $(BUILD)/openocd.cfg
	@echo reset halt >> $(BUILD)/openocd.cfg
	@echo flash write_image erase $(BUILD)/$(PROJECT_NAME).hex >> $(BUILD)/openocd.cfg
	@echo verify_image $(BUILD)/$(PROJECT_NAME).hex >> $(BUILD)/openocd.cfg
	@echo reset run >> $(BUILD)/openocd.cfg
	@echo shutdown >> $(BUILD)/openocd.cfg
	openocd -f $(BUILD)/openocd.cfg
