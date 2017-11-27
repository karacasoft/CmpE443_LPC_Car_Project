################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/dev/adc.c \
../src/dev/car_leds.c \
../src/dev/device.c \
../src/dev/iocon.c \
../src/dev/joystick.c \
../src/dev/l298n.c \
../src/dev/ldr.c \
../src/dev/pwm0.c \
../src/dev/rgb_led.c \
../src/dev/timer.c \
../src/dev/trimpot.c 

OBJS += \
./src/dev/adc.o \
./src/dev/car_leds.o \
./src/dev/device.o \
./src/dev/iocon.o \
./src/dev/joystick.o \
./src/dev/l298n.o \
./src/dev/ldr.o \
./src/dev/pwm0.o \
./src/dev/rgb_led.o \
./src/dev/timer.o \
./src/dev/trimpot.o 

C_DEPS += \
./src/dev/adc.d \
./src/dev/car_leds.d \
./src/dev/device.d \
./src/dev/iocon.d \
./src/dev/joystick.d \
./src/dev/l298n.d \
./src/dev/ldr.d \
./src/dev/pwm0.d \
./src/dev/rgb_led.d \
./src/dev/timer.d \
./src/dev/trimpot.d 


# Each subdirectory must supply rules for building sources it contributes
src/dev/%.o: ../src/dev/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -DNO_BOARD_LIB -D__LPC407X_8X__ -D__REDLIB__ -I"/Users/bahadir/Documents/MCUXpressoIDE_10.1.0/workspace/lpc_chip_40xx/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


