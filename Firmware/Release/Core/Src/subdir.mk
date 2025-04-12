################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/FT5XX6.c \
../Core/Src/TargetTouch.c \
../Core/Src/freertos.c \
../Core/Src/main.c \
../Core/Src/mcu_flash.c \
../Core/Src/nema_hal.c \
../Core/Src/nmea.c \
../Core/Src/sdram.c \
../Core/Src/spi_flash.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/system_stm32f4xx.c 

C_DEPS += \
./Core/Src/FT5XX6.d \
./Core/Src/TargetTouch.d \
./Core/Src/freertos.d \
./Core/Src/main.d \
./Core/Src/mcu_flash.d \
./Core/Src/nema_hal.d \
./Core/Src/nmea.d \
./Core/Src/sdram.d \
./Core/Src/spi_flash.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/system_stm32f4xx.d 

OBJS += \
./Core/Src/FT5XX6.o \
./Core/Src/TargetTouch.o \
./Core/Src/freertos.o \
./Core/Src/main.o \
./Core/Src/mcu_flash.o \
./Core/Src/nema_hal.o \
./Core/Src/nmea.o \
./Core/Src/sdram.o \
./Core/Src/spi_flash.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/system_stm32f4xx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/FT5XX6.cyclo ./Core/Src/FT5XX6.d ./Core/Src/FT5XX6.o ./Core/Src/FT5XX6.su ./Core/Src/TargetTouch.cyclo ./Core/Src/TargetTouch.d ./Core/Src/TargetTouch.o ./Core/Src/TargetTouch.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mcu_flash.cyclo ./Core/Src/mcu_flash.d ./Core/Src/mcu_flash.o ./Core/Src/mcu_flash.su ./Core/Src/nema_hal.cyclo ./Core/Src/nema_hal.d ./Core/Src/nema_hal.o ./Core/Src/nema_hal.su ./Core/Src/nmea.cyclo ./Core/Src/nmea.d ./Core/Src/nmea.o ./Core/Src/nmea.su ./Core/Src/sdram.cyclo ./Core/Src/sdram.d ./Core/Src/sdram.o ./Core/Src/sdram.su ./Core/Src/spi_flash.cyclo ./Core/Src/spi_flash.d ./Core/Src/spi_flash.o ./Core/Src/spi_flash.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

