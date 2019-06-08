################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/cube/cube.c 

OBJS += \
./Sources/cube/cube.o 

C_DEPS += \
./Sources/cube/cube.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/cube/%.o: ../Sources/cube/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\cube" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\timer" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\sensors" -I"../Sources/adc" -I"../Sources/com" -I"../Sources/i2c" -I"../Sources/utils" -I"../Includes" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\drive" -std=c99 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


