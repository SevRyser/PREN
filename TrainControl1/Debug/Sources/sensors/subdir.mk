################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/sensors/accel.c \
../Sources/sensors/beep.c \
../Sources/sensors/encoder.c \
../Sources/sensors/switch.c \
../Sources/sensors/tof.c 

OBJS += \
./Sources/sensors/accel.o \
./Sources/sensors/beep.o \
./Sources/sensors/encoder.o \
./Sources/sensors/switch.o \
./Sources/sensors/tof.o 

C_DEPS += \
./Sources/sensors/accel.d \
./Sources/sensors/beep.d \
./Sources/sensors/encoder.d \
./Sources/sensors/switch.d \
./Sources/sensors/tof.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/sensors/%.o: ../Sources/sensors/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\cube" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\timer" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\sensors" -I"../Sources/adc" -I"../Sources/com" -I"../Sources/i2c" -I"../Sources/utils" -I"../Includes" -I"C:\PRG_G\wsp_pren\TrainControl1\Sources\drive" -std=c99 -Wa,-adhlns="$@.lst" -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


