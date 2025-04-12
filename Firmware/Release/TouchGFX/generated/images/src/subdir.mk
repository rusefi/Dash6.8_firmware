################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX/generated/images/src/BitmapDatabase.cpp \
../TouchGFX/generated/images/src/SVGDatabase.cpp \
../TouchGFX/generated/images/src/image_ST800_8.cpp \
../TouchGFX/generated/images/src/image_indBett_red.cpp \
../TouchGFX/generated/images/src/image_indECT_Red.cpp \
../TouchGFX/generated/images/src/image_indECT_blue.cpp \
../TouchGFX/generated/images/src/image_indFuel_Orang.cpp \
../TouchGFX/generated/images/src/image_indOil_red.cpp \
../TouchGFX/generated/images/src/image_needle_1.cpp 

OBJS += \
./TouchGFX/generated/images/src/BitmapDatabase.o \
./TouchGFX/generated/images/src/SVGDatabase.o \
./TouchGFX/generated/images/src/image_ST800_8.o \
./TouchGFX/generated/images/src/image_indBett_red.o \
./TouchGFX/generated/images/src/image_indECT_Red.o \
./TouchGFX/generated/images/src/image_indECT_blue.o \
./TouchGFX/generated/images/src/image_indFuel_Orang.o \
./TouchGFX/generated/images/src/image_indOil_red.o \
./TouchGFX/generated/images/src/image_needle_1.o 

CPP_DEPS += \
./TouchGFX/generated/images/src/BitmapDatabase.d \
./TouchGFX/generated/images/src/SVGDatabase.d \
./TouchGFX/generated/images/src/image_ST800_8.d \
./TouchGFX/generated/images/src/image_indBett_red.d \
./TouchGFX/generated/images/src/image_indECT_Red.d \
./TouchGFX/generated/images/src/image_indECT_blue.d \
./TouchGFX/generated/images/src/image_indFuel_Orang.d \
./TouchGFX/generated/images/src/image_indOil_red.d \
./TouchGFX/generated/images/src/image_needle_1.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX/generated/images/src/%.o TouchGFX/generated/images/src/%.su TouchGFX/generated/images/src/%.cyclo: ../TouchGFX/generated/images/src/%.cpp TouchGFX/generated/images/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX-2f-generated-2f-images-2f-src

clean-TouchGFX-2f-generated-2f-images-2f-src:
	-$(RM) ./TouchGFX/generated/images/src/BitmapDatabase.cyclo ./TouchGFX/generated/images/src/BitmapDatabase.d ./TouchGFX/generated/images/src/BitmapDatabase.o ./TouchGFX/generated/images/src/BitmapDatabase.su ./TouchGFX/generated/images/src/SVGDatabase.cyclo ./TouchGFX/generated/images/src/SVGDatabase.d ./TouchGFX/generated/images/src/SVGDatabase.o ./TouchGFX/generated/images/src/SVGDatabase.su ./TouchGFX/generated/images/src/image_ST800_8.cyclo ./TouchGFX/generated/images/src/image_ST800_8.d ./TouchGFX/generated/images/src/image_ST800_8.o ./TouchGFX/generated/images/src/image_ST800_8.su ./TouchGFX/generated/images/src/image_indBett_red.cyclo ./TouchGFX/generated/images/src/image_indBett_red.d ./TouchGFX/generated/images/src/image_indBett_red.o ./TouchGFX/generated/images/src/image_indBett_red.su ./TouchGFX/generated/images/src/image_indECT_Red.cyclo ./TouchGFX/generated/images/src/image_indECT_Red.d ./TouchGFX/generated/images/src/image_indECT_Red.o ./TouchGFX/generated/images/src/image_indECT_Red.su ./TouchGFX/generated/images/src/image_indECT_blue.cyclo ./TouchGFX/generated/images/src/image_indECT_blue.d ./TouchGFX/generated/images/src/image_indECT_blue.o ./TouchGFX/generated/images/src/image_indECT_blue.su ./TouchGFX/generated/images/src/image_indFuel_Orang.cyclo ./TouchGFX/generated/images/src/image_indFuel_Orang.d ./TouchGFX/generated/images/src/image_indFuel_Orang.o ./TouchGFX/generated/images/src/image_indFuel_Orang.su ./TouchGFX/generated/images/src/image_indOil_red.cyclo ./TouchGFX/generated/images/src/image_indOil_red.d ./TouchGFX/generated/images/src/image_indOil_red.o ./TouchGFX/generated/images/src/image_indOil_red.su ./TouchGFX/generated/images/src/image_needle_1.cyclo ./TouchGFX/generated/images/src/image_needle_1.d ./TouchGFX/generated/images/src/image_needle_1.o ./TouchGFX/generated/images/src/image_needle_1.su

.PHONY: clean-TouchGFX-2f-generated-2f-images-2f-src

