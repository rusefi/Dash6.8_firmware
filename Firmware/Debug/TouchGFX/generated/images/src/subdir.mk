################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TouchGFX/generated/images/src/BitmapDatabase.cpp \
../TouchGFX/generated/images/src/SVGDatabase.cpp \
../TouchGFX/generated/images/src/image_Frame_66.cpp \
../TouchGFX/generated/images/src/image_Frame_69.cpp \
../TouchGFX/generated/images/src/image_Fuel.cpp \
../TouchGFX/generated/images/src/image_Group_38.cpp \
../TouchGFX/generated/images/src/image_Group_39.cpp \
../TouchGFX/generated/images/src/image_Group_42.cpp \
../TouchGFX/generated/images/src/image_Needl_Oil.cpp \
../TouchGFX/generated/images/src/image_Serv.cpp \
../TouchGFX/generated/images/src/image_Temp.cpp \
../TouchGFX/generated/images/src/image_V.cpp \
../TouchGFX/generated/images/src/image_Vector.cpp \
../TouchGFX/generated/images/src/image_Vector_8.cpp \
../TouchGFX/generated/images/src/image_Water_temp.cpp \
../TouchGFX/generated/images/src/image_Water_temp_Blue.cpp \
../TouchGFX/generated/images/src/image_Water_temp_red.cpp \
../TouchGFX/generated/images/src/image_batteryblock.cpp \
../TouchGFX/generated/images/src/image_chek.cpp \
../TouchGFX/generated/images/src/image_fuil.cpp \
../TouchGFX/generated/images/src/image_fuil_llov.cpp \
../TouchGFX/generated/images/src/image_h.cpp \
../TouchGFX/generated/images/src/image_lh.cpp \
../TouchGFX/generated/images/src/image_oil.cpp \
../TouchGFX/generated/images/src/image_record.circle.cpp \
../TouchGFX/generated/images/src/image_reshot-icon-light-max-SKL25EYVD9_1.cpp 

OBJS += \
./TouchGFX/generated/images/src/BitmapDatabase.o \
./TouchGFX/generated/images/src/SVGDatabase.o \
./TouchGFX/generated/images/src/image_Frame_66.o \
./TouchGFX/generated/images/src/image_Frame_69.o \
./TouchGFX/generated/images/src/image_Fuel.o \
./TouchGFX/generated/images/src/image_Group_38.o \
./TouchGFX/generated/images/src/image_Group_39.o \
./TouchGFX/generated/images/src/image_Group_42.o \
./TouchGFX/generated/images/src/image_Needl_Oil.o \
./TouchGFX/generated/images/src/image_Serv.o \
./TouchGFX/generated/images/src/image_Temp.o \
./TouchGFX/generated/images/src/image_V.o \
./TouchGFX/generated/images/src/image_Vector.o \
./TouchGFX/generated/images/src/image_Vector_8.o \
./TouchGFX/generated/images/src/image_Water_temp.o \
./TouchGFX/generated/images/src/image_Water_temp_Blue.o \
./TouchGFX/generated/images/src/image_Water_temp_red.o \
./TouchGFX/generated/images/src/image_batteryblock.o \
./TouchGFX/generated/images/src/image_chek.o \
./TouchGFX/generated/images/src/image_fuil.o \
./TouchGFX/generated/images/src/image_fuil_llov.o \
./TouchGFX/generated/images/src/image_h.o \
./TouchGFX/generated/images/src/image_lh.o \
./TouchGFX/generated/images/src/image_oil.o \
./TouchGFX/generated/images/src/image_record.circle.o \
./TouchGFX/generated/images/src/image_reshot-icon-light-max-SKL25EYVD9_1.o 

CPP_DEPS += \
./TouchGFX/generated/images/src/BitmapDatabase.d \
./TouchGFX/generated/images/src/SVGDatabase.d \
./TouchGFX/generated/images/src/image_Frame_66.d \
./TouchGFX/generated/images/src/image_Frame_69.d \
./TouchGFX/generated/images/src/image_Fuel.d \
./TouchGFX/generated/images/src/image_Group_38.d \
./TouchGFX/generated/images/src/image_Group_39.d \
./TouchGFX/generated/images/src/image_Group_42.d \
./TouchGFX/generated/images/src/image_Needl_Oil.d \
./TouchGFX/generated/images/src/image_Serv.d \
./TouchGFX/generated/images/src/image_Temp.d \
./TouchGFX/generated/images/src/image_V.d \
./TouchGFX/generated/images/src/image_Vector.d \
./TouchGFX/generated/images/src/image_Vector_8.d \
./TouchGFX/generated/images/src/image_Water_temp.d \
./TouchGFX/generated/images/src/image_Water_temp_Blue.d \
./TouchGFX/generated/images/src/image_Water_temp_red.d \
./TouchGFX/generated/images/src/image_batteryblock.d \
./TouchGFX/generated/images/src/image_chek.d \
./TouchGFX/generated/images/src/image_fuil.d \
./TouchGFX/generated/images/src/image_fuil_llov.d \
./TouchGFX/generated/images/src/image_h.d \
./TouchGFX/generated/images/src/image_lh.d \
./TouchGFX/generated/images/src/image_oil.d \
./TouchGFX/generated/images/src/image_record.circle.d \
./TouchGFX/generated/images/src/image_reshot-icon-light-max-SKL25EYVD9_1.d 


# Each subdirectory must supply rules for building sources it contributes
TouchGFX/generated/images/src/%.o TouchGFX/generated/images/src/%.su TouchGFX/generated/images/src/%.cyclo: ../TouchGFX/generated/images/src/%.cpp TouchGFX/generated/images/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../TouchGFX/App -I../TouchGFX/target/generated -I../TouchGFX/target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../TouchGFX/generated/fonts/include -I../TouchGFX/generated/gui_generated/include -I../TouchGFX/generated/images/include -I../TouchGFX/generated/texts/include -I../TouchGFX/generated/videos/include -I../TouchGFX/gui/include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TouchGFX-2f-generated-2f-images-2f-src

clean-TouchGFX-2f-generated-2f-images-2f-src:
	-$(RM) ./TouchGFX/generated/images/src/BitmapDatabase.cyclo ./TouchGFX/generated/images/src/BitmapDatabase.d ./TouchGFX/generated/images/src/BitmapDatabase.o ./TouchGFX/generated/images/src/BitmapDatabase.su ./TouchGFX/generated/images/src/SVGDatabase.cyclo ./TouchGFX/generated/images/src/SVGDatabase.d ./TouchGFX/generated/images/src/SVGDatabase.o ./TouchGFX/generated/images/src/SVGDatabase.su ./TouchGFX/generated/images/src/image_Frame_66.cyclo ./TouchGFX/generated/images/src/image_Frame_66.d ./TouchGFX/generated/images/src/image_Frame_66.o ./TouchGFX/generated/images/src/image_Frame_66.su ./TouchGFX/generated/images/src/image_Frame_69.cyclo ./TouchGFX/generated/images/src/image_Frame_69.d ./TouchGFX/generated/images/src/image_Frame_69.o ./TouchGFX/generated/images/src/image_Frame_69.su ./TouchGFX/generated/images/src/image_Fuel.cyclo ./TouchGFX/generated/images/src/image_Fuel.d ./TouchGFX/generated/images/src/image_Fuel.o ./TouchGFX/generated/images/src/image_Fuel.su ./TouchGFX/generated/images/src/image_Group_38.cyclo ./TouchGFX/generated/images/src/image_Group_38.d ./TouchGFX/generated/images/src/image_Group_38.o ./TouchGFX/generated/images/src/image_Group_38.su ./TouchGFX/generated/images/src/image_Group_39.cyclo ./TouchGFX/generated/images/src/image_Group_39.d ./TouchGFX/generated/images/src/image_Group_39.o ./TouchGFX/generated/images/src/image_Group_39.su ./TouchGFX/generated/images/src/image_Group_42.cyclo ./TouchGFX/generated/images/src/image_Group_42.d ./TouchGFX/generated/images/src/image_Group_42.o ./TouchGFX/generated/images/src/image_Group_42.su ./TouchGFX/generated/images/src/image_Needl_Oil.cyclo ./TouchGFX/generated/images/src/image_Needl_Oil.d ./TouchGFX/generated/images/src/image_Needl_Oil.o ./TouchGFX/generated/images/src/image_Needl_Oil.su ./TouchGFX/generated/images/src/image_Serv.cyclo ./TouchGFX/generated/images/src/image_Serv.d ./TouchGFX/generated/images/src/image_Serv.o ./TouchGFX/generated/images/src/image_Serv.su ./TouchGFX/generated/images/src/image_Temp.cyclo ./TouchGFX/generated/images/src/image_Temp.d ./TouchGFX/generated/images/src/image_Temp.o ./TouchGFX/generated/images/src/image_Temp.su ./TouchGFX/generated/images/src/image_V.cyclo ./TouchGFX/generated/images/src/image_V.d ./TouchGFX/generated/images/src/image_V.o ./TouchGFX/generated/images/src/image_V.su ./TouchGFX/generated/images/src/image_Vector.cyclo ./TouchGFX/generated/images/src/image_Vector.d ./TouchGFX/generated/images/src/image_Vector.o ./TouchGFX/generated/images/src/image_Vector.su ./TouchGFX/generated/images/src/image_Vector_8.cyclo ./TouchGFX/generated/images/src/image_Vector_8.d ./TouchGFX/generated/images/src/image_Vector_8.o ./TouchGFX/generated/images/src/image_Vector_8.su ./TouchGFX/generated/images/src/image_Water_temp.cyclo ./TouchGFX/generated/images/src/image_Water_temp.d ./TouchGFX/generated/images/src/image_Water_temp.o ./TouchGFX/generated/images/src/image_Water_temp.su ./TouchGFX/generated/images/src/image_Water_temp_Blue.cyclo ./TouchGFX/generated/images/src/image_Water_temp_Blue.d ./TouchGFX/generated/images/src/image_Water_temp_Blue.o ./TouchGFX/generated/images/src/image_Water_temp_Blue.su ./TouchGFX/generated/images/src/image_Water_temp_red.cyclo ./TouchGFX/generated/images/src/image_Water_temp_red.d ./TouchGFX/generated/images/src/image_Water_temp_red.o ./TouchGFX/generated/images/src/image_Water_temp_red.su ./TouchGFX/generated/images/src/image_batteryblock.cyclo ./TouchGFX/generated/images/src/image_batteryblock.d ./TouchGFX/generated/images/src/image_batteryblock.o ./TouchGFX/generated/images/src/image_batteryblock.su ./TouchGFX/generated/images/src/image_chek.cyclo ./TouchGFX/generated/images/src/image_chek.d ./TouchGFX/generated/images/src/image_chek.o ./TouchGFX/generated/images/src/image_chek.su ./TouchGFX/generated/images/src/image_fuil.cyclo ./TouchGFX/generated/images/src/image_fuil.d ./TouchGFX/generated/images/src/image_fuil.o ./TouchGFX/generated/images/src/image_fuil.su ./TouchGFX/generated/images/src/image_fuil_llov.cyclo ./TouchGFX/generated/images/src/image_fuil_llov.d ./TouchGFX/generated/images/src/image_fuil_llov.o ./TouchGFX/generated/images/src/image_fuil_llov.su ./TouchGFX/generated/images/src/image_h.cyclo ./TouchGFX/generated/images/src/image_h.d ./TouchGFX/generated/images/src/image_h.o ./TouchGFX/generated/images/src/image_h.su ./TouchGFX/generated/images/src/image_lh.cyclo ./TouchGFX/generated/images/src/image_lh.d ./TouchGFX/generated/images/src/image_lh.o ./TouchGFX/generated/images/src/image_lh.su ./TouchGFX/generated/images/src/image_oil.cyclo ./TouchGFX/generated/images/src/image_oil.d ./TouchGFX/generated/images/src/image_oil.o ./TouchGFX/generated/images/src/image_oil.su ./TouchGFX/generated/images/src/image_record.circle.cyclo ./TouchGFX/generated/images/src/image_record.circle.d ./TouchGFX/generated/images/src/image_record.circle.o ./TouchGFX/generated/images/src/image_record.circle.su ./TouchGFX/generated/images/src/image_reshot-icon-light-max-SKL25EYVD9_1.cyclo ./TouchGFX/generated/images/src/image_reshot-icon-light-max-SKL25EYVD9_1.d ./TouchGFX/generated/images/src/image_reshot-icon-light-max-SKL25EYVD9_1.o ./TouchGFX/generated/images/src/image_reshot-icon-light-max-SKL25EYVD9_1.su

.PHONY: clean-TouchGFX-2f-generated-2f-images-2f-src

