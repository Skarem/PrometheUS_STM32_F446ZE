################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/PrometheUS/Control/Finger.cpp \
../Core/PrometheUS/Control/PIDController.cpp 

OBJS += \
./Core/PrometheUS/Control/Finger.o \
./Core/PrometheUS/Control/PIDController.o 

CPP_DEPS += \
./Core/PrometheUS/Control/Finger.d \
./Core/PrometheUS/Control/PIDController.d 


# Each subdirectory must supply rules for building sources it contributes
Core/PrometheUS/Control/%.o Core/PrometheUS/Control/%.su Core/PrometheUS/Control/%.cyclo: ../Core/PrometheUS/Control/%.cpp Core/PrometheUS/Control/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/PrometheUS -I../Core/PrometheUS/System -I../Core/PrometheUS/Control -I../Core/PrometheUS/Actuation -I../Core/PrometheUS/Sensing -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-PrometheUS-2f-Control

clean-Core-2f-PrometheUS-2f-Control:
	-$(RM) ./Core/PrometheUS/Control/Finger.cyclo ./Core/PrometheUS/Control/Finger.d ./Core/PrometheUS/Control/Finger.o ./Core/PrometheUS/Control/Finger.su ./Core/PrometheUS/Control/PIDController.cyclo ./Core/PrometheUS/Control/PIDController.d ./Core/PrometheUS/Control/PIDController.o ./Core/PrometheUS/Control/PIDController.su

.PHONY: clean-Core-2f-PrometheUS-2f-Control

