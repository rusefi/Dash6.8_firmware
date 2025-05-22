################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/\ json_dispatcher.c \
../Core/Src/CAN.c \
../Core/Src/ErrorCode.c \
../Core/Src/TargetTouch.c \
../Core/Src/cJSON.c \
../Core/Src/eeprom24lc512.c \
../Core/Src/freertos.c \
../Core/Src/ili211x.c \
../Core/Src/json_builder.c \
../Core/Src/main.c \
../Core/Src/nmea.c \
../Core/Src/sdram.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/usart3_json_dma.c \
../Core/Src/z_flash_W25QXXX.c 

C_DEPS += \
./Core/Src/\ json_dispatcher.d \
./Core/Src/CAN.d \
./Core/Src/ErrorCode.d \
./Core/Src/TargetTouch.d \
./Core/Src/cJSON.d \
./Core/Src/eeprom24lc512.d \
./Core/Src/freertos.d \
./Core/Src/ili211x.d \
./Core/Src/json_builder.d \
./Core/Src/main.d \
./Core/Src/nmea.d \
./Core/Src/sdram.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/usart3_json_dma.d \
./Core/Src/z_flash_W25QXXX.d 

OBJS += \
./Core/Src/\ json_dispatcher.o \
./Core/Src/CAN.o \
./Core/Src/ErrorCode.o \
./Core/Src/TargetTouch.o \
./Core/Src/cJSON.o \
./Core/Src/eeprom24lc512.o \
./Core/Src/freertos.o \
./Core/Src/ili211x.o \
./Core/Src/json_builder.o \
./Core/Src/main.o \
./Core/Src/nmea.o \
./Core/Src/sdram.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/usart3_json_dma.o \
./Core/Src/z_flash_W25QXXX.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/\ json_dispatcher.o: ../Core/Src/\ json_dispatcher.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/ json_dispatcher.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/\ json_dispatcher.cyclo ./Core/Src/\ json_dispatcher.d ./Core/Src/\ json_dispatcher.o ./Core/Src/\ json_dispatcher.su ./Core/Src/CAN.cyclo ./Core/Src/CAN.d ./Core/Src/CAN.o ./Core/Src/CAN.su ./Core/Src/ErrorCode.cyclo ./Core/Src/ErrorCode.d ./Core/Src/ErrorCode.o ./Core/Src/ErrorCode.su ./Core/Src/TargetTouch.cyclo ./Core/Src/TargetTouch.d ./Core/Src/TargetTouch.o ./Core/Src/TargetTouch.su ./Core/Src/cJSON.cyclo ./Core/Src/cJSON.d ./Core/Src/cJSON.o ./Core/Src/cJSON.su ./Core/Src/eeprom24lc512.cyclo ./Core/Src/eeprom24lc512.d ./Core/Src/eeprom24lc512.o ./Core/Src/eeprom24lc512.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/ili211x.cyclo ./Core/Src/ili211x.d ./Core/Src/ili211x.o ./Core/Src/ili211x.su ./Core/Src/json_builder.cyclo ./Core/Src/json_builder.d ./Core/Src/json_builder.o ./Core/Src/json_builder.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/nmea.cyclo ./Core/Src/nmea.d ./Core/Src/nmea.o ./Core/Src/nmea.su ./Core/Src/sdram.cyclo ./Core/Src/sdram.d ./Core/Src/sdram.o ./Core/Src/sdram.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/usart3_json_dma.cyclo ./Core/Src/usart3_json_dma.d ./Core/Src/usart3_json_dma.o ./Core/Src/usart3_json_dma.su ./Core/Src/z_flash_W25QXXX.cyclo ./Core/Src/z_flash_W25QXXX.d ./Core/Src/z_flash_W25QXXX.o ./Core/Src/z_flash_W25QXXX.su

.PHONY: clean-Core-2f-Src

