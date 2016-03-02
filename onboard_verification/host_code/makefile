################################################################################
# Automatically-generated file. Do not edit!
################################################################################


APP := ldecod

RM := rm -rf

LIBS := -lm

# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
ldecod.o \
nalu.o \
xdecode_main.o \
xdecode_main_linux.o \
xdecode_main_sinit.o \
fpga_comm.o

# Each subdirectory must supply rules for building sources it contributes
%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

# All Target
all: $(APP).elf

# Tool invocations
$(APP).elf: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM gcc linker'
	gcc -o "$(APP).elf" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(S_UPPER_DEPS) $(APP).elf *.d
	-@echo ' '

.PHONY: all clean dependents
