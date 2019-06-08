################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Project_Settings/Startup_Code/system_MK22F51212.c 

S_UPPER_SRCS += \
../Project_Settings/Startup_Code/startup_MK22F51212.S 

OBJS += \
./Project_Settings/Startup_Code/startup_MK22F51212.o \
./Project_Settings/Startup_Code/system_MK22F51212.o 

S_UPPER_DEPS += \
./Project_Settings/Startup_Code/startup_MK22F51212.d 

C_DEPS += \
./Project_Settings/Startup_Code/system_MK22F51212.d 


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/%.o: ../Project_Settings/Startup_Code/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/%.o: ../Project_Settings/Startup_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\cube" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\timer" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\sensors" -I"../Sources/adc" -I"../Sources/com" -I"../Sources/i2c" -I"../Sources/utils" -I"../Includes" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\drive" -std=c99 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


