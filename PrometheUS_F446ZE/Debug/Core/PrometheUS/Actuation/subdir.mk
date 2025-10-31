################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/PrometheUS/Actuation/Clutch.cpp \
../Core/PrometheUS/Actuation/Motor.cpp \
../Core/PrometheUS/Actuation/PWM.cpp \
../Core/PrometheUS/Actuation/PWMComplementary.cpp 

OBJS += \
./Core/PrometheUS/Actuation/Clutch.o \
./Core/PrometheUS/Actuation/Motor.o \
./Core/PrometheUS/Actuation/PWM.o \
./Core/PrometheUS/Actuation/PWMComplementary.o 

CPP_DEPS += \
./Core/PrometheUS/Actuation/Clutch.d \
./Core/PrometheUS/Actuation/Motor.d \
./Core/PrometheUS/Actuation/PWM.d \
./Core/PrometheUS/Actuation/PWMComplementary.d 


# Each subdirectory must supply rules for building sources it contributes
Core/PrometheUS/Actuation/%.o Core/PrometheUS/Actuation/%.su Core/PrometheUS/Actuation/%.cyclo: ../Core/PrometheUS/Actuation/%.cpp Core/PrometheUS/Actuation/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/PrometheUS -I../Core/PrometheUS/Tests -I../Core/PrometheUS/System -I../Core/PrometheUS/Control -I../Core/PrometheUS/Actuation -I../Core/PrometheUS/Sensing -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-PrometheUS-2f-Actuation

clean-Core-2f-PrometheUS-2f-Actuation:
	-$(RM) ./Core/PrometheUS/Actuation/Clutch.cyclo ./Core/PrometheUS/Actuation/Clutch.d ./Core/PrometheUS/Actuation/Clutch.o ./Core/PrometheUS/Actuation/Clutch.su ./Core/PrometheUS/Actuation/Motor.cyclo ./Core/PrometheUS/Actuation/Motor.d ./Core/PrometheUS/Actuation/Motor.o ./Core/PrometheUS/Actuation/Motor.su ./Core/PrometheUS/Actuation/PWM.cyclo ./Core/PrometheUS/Actuation/PWM.d ./Core/PrometheUS/Actuation/PWM.o ./Core/PrometheUS/Actuation/PWM.su ./Core/PrometheUS/Actuation/PWMComplementary.cyclo ./Core/PrometheUS/Actuation/PWMComplementary.d ./Core/PrometheUS/Actuation/PWMComplementary.o ./Core/PrometheUS/Actuation/PWMComplementary.su

.PHONY: clean-Core-2f-PrometheUS-2f-Actuation

