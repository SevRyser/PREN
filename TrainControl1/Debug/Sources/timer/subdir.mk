################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/timer/ftm0.c \
../Sources/timer/ftm1quad.c \
../Sources/timer/ftm2.c \
../Sources/timer/ftm3.c 

OBJS += \
./Sources/timer/ftm0.o \
./Sources/timer/ftm1quad.o \
./Sources/timer/ftm2.o \
./Sources/timer/ftm3.o 

C_DEPS += \
./Sources/timer/ftm0.d \
./Sources/timer/ftm1quad.d \
./Sources/timer/ftm2.d \
./Sources/timer/ftm3.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/timer/%.o: ../Sources/timer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\cube" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\timer" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\sensors" -I"../Sources/adc" -I"../Sources/com" -I"../Sources/i2c" -I"../Sources/utils" -I"../Includes" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\drive" -std=c99 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


