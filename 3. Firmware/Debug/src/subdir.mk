################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mylibrary/src/DEV_Config.c \
../Mylibrary/src/EPD_2in9_V2.c \
../Mylibrary/src/EPD_2in9_V2_test.c \
../Mylibrary/src/GUI_Paint.c \
../Mylibrary/src/ImageData.c \
../Mylibrary/src/font12.c \
../Mylibrary/src/font12CN.c \
../Mylibrary/src/font16.c \
../Mylibrary/src/font20.c \
../Mylibrary/src/font24.c \
../Mylibrary/src/font24CN.c \
../Mylibrary/src/font8.c 

OBJS += \
./src/DEV_Config.o \
./src/EPD_2in9_V2.o \
./src/EPD_2in9_V2_test.o \
./src/GUI_Paint.o \
./src/ImageData.o \
./src/font12.o \
./src/font12CN.o \
./src/font16.o \
./src/font20.o \
./src/font24.o \
./src/font24CN.o \
./src/font8.o 

C_DEPS += \
./src/DEV_Config.d \
./src/EPD_2in9_V2.d \
./src/EPD_2in9_V2_test.d \
./src/GUI_Paint.d \
./src/ImageData.d \
./src/font12.d \
./src/font12CN.d \
./src/font16.d \
./src/font20.d \
./src/font24.d \
./src/font24CN.d \
./src/font8.d 


# Each subdirectory must supply rules for building sources it contributes
src/DEV_Config.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/DEV_Config.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/EPD_2in9_V2.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/EPD_2in9_V2.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/EPD_2in9_V2_test.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/EPD_2in9_V2_test.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/GUI_Paint.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/GUI_Paint.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/ImageData.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/ImageData.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/font12.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/font12.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/font12CN.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/font12CN.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/font16.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/font16.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/font20.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/font20.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/font24.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/font24.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/font24CN.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/font24CN.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/font8.o: C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3.\ Firmware/Mylibrary/src/font8.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Public/Documents/Altium/Projects/BTL_test_1/Project_EBS/Embedded-System-Project/3. Firmware/Mylibrary/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/DEV_Config.cyclo ./src/DEV_Config.d ./src/DEV_Config.o ./src/DEV_Config.su ./src/EPD_2in9_V2.cyclo ./src/EPD_2in9_V2.d ./src/EPD_2in9_V2.o ./src/EPD_2in9_V2.su ./src/EPD_2in9_V2_test.cyclo ./src/EPD_2in9_V2_test.d ./src/EPD_2in9_V2_test.o ./src/EPD_2in9_V2_test.su ./src/GUI_Paint.cyclo ./src/GUI_Paint.d ./src/GUI_Paint.o ./src/GUI_Paint.su ./src/ImageData.cyclo ./src/ImageData.d ./src/ImageData.o ./src/ImageData.su ./src/font12.cyclo ./src/font12.d ./src/font12.o ./src/font12.su ./src/font12CN.cyclo ./src/font12CN.d ./src/font12CN.o ./src/font12CN.su ./src/font16.cyclo ./src/font16.d ./src/font16.o ./src/font16.su ./src/font20.cyclo ./src/font20.d ./src/font20.o ./src/font20.su ./src/font24.cyclo ./src/font24.d ./src/font24.o ./src/font24.su ./src/font24CN.cyclo ./src/font24CN.d ./src/font24CN.o ./src/font24CN.su ./src/font8.cyclo ./src/font8.d ./src/font8.o ./src/font8.su

.PHONY: clean-src

