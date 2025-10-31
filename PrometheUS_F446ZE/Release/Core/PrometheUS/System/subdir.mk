################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/PrometheUS/System/PrometheUS.cpp \
../Core/PrometheUS/System/SystemFlags.cpp \
../Core/PrometheUS/System/TelemetrySender.cpp 

OBJS += \
./Core/PrometheUS/System/PrometheUS.o \
./Core/PrometheUS/System/SystemFlags.o \
./Core/PrometheUS/System/TelemetrySender.o 

CPP_DEPS += \
./Core/PrometheUS/System/PrometheUS.d \
./Core/PrometheUS/System/SystemFlags.d \
./Core/PrometheUS/System/TelemetrySender.d 


# Each subdirectory must supply rules for building sources it contributes
Core/PrometheUS/System/%.o Core/PrometheUS/System/%.su Core/PrometheUS/System/%.cyclo: ../Core/PrometheUS/System/%.cpp Core/PrometheUS/System/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/PrometheUS -I../Core/PrometheUS/Tests -I../Core/PrometheUS/System -I../Core/PrometheUS/Control -I../Core/PrometheUS/Actuation -I../Core/PrometheUS/Sensing -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-PrometheUS-2f-System

clean-Core-2f-PrometheUS-2f-System:
	-$(RM) ./Core/PrometheUS/System/PrometheUS.cyclo ./Core/PrometheUS/System/PrometheUS.d ./Core/PrometheUS/System/PrometheUS.o ./Core/PrometheUS/System/PrometheUS.su ./Core/PrometheUS/System/SystemFlags.cyclo ./Core/PrometheUS/System/SystemFlags.d ./Core/PrometheUS/System/SystemFlags.o ./Core/PrometheUS/System/SystemFlags.su ./Core/PrometheUS/System/TelemetrySender.cyclo ./Core/PrometheUS/System/TelemetrySender.d ./Core/PrometheUS/System/TelemetrySender.o ./Core/PrometheUS/System/TelemetrySender.su

.PHONY: clean-Core-2f-PrometheUS-2f-System

