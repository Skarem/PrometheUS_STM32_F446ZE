################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/PrometheUS/System/Sensing/ClutchCurrentSampler.cpp \
../Core/PrometheUS/System/Sensing/ClutchesTemperatureSampler.cpp \
../Core/PrometheUS/System/Sensing/EncoderSampler.cpp \
../Core/PrometheUS/System/Sensing/GenericAdcBurstSampler.cpp \
../Core/PrometheUS/System/Sensing/GenericAdcChannelSampler.cpp \
../Core/PrometheUS/System/Sensing/MotorVelocitySampler.cpp \
../Core/PrometheUS/System/Sensing/PotentiometersSampler.cpp 

OBJS += \
./Core/PrometheUS/System/Sensing/ClutchCurrentSampler.o \
./Core/PrometheUS/System/Sensing/ClutchesTemperatureSampler.o \
./Core/PrometheUS/System/Sensing/EncoderSampler.o \
./Core/PrometheUS/System/Sensing/GenericAdcBurstSampler.o \
./Core/PrometheUS/System/Sensing/GenericAdcChannelSampler.o \
./Core/PrometheUS/System/Sensing/MotorVelocitySampler.o \
./Core/PrometheUS/System/Sensing/PotentiometersSampler.o 

CPP_DEPS += \
./Core/PrometheUS/System/Sensing/ClutchCurrentSampler.d \
./Core/PrometheUS/System/Sensing/ClutchesTemperatureSampler.d \
./Core/PrometheUS/System/Sensing/EncoderSampler.d \
./Core/PrometheUS/System/Sensing/GenericAdcBurstSampler.d \
./Core/PrometheUS/System/Sensing/GenericAdcChannelSampler.d \
./Core/PrometheUS/System/Sensing/MotorVelocitySampler.d \
./Core/PrometheUS/System/Sensing/PotentiometersSampler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/PrometheUS/System/Sensing/%.o Core/PrometheUS/System/Sensing/%.su Core/PrometheUS/System/Sensing/%.cyclo: ../Core/PrometheUS/System/Sensing/%.cpp Core/PrometheUS/System/Sensing/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/PrometheUS -I../Core/PrometheUS/Tests -I../Core/PrometheUS/System -I../Core/PrometheUS/Control -I../Core/PrometheUS/Actuation -I../Core/PrometheUS/Sensing -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-PrometheUS-2f-System-2f-Sensing

clean-Core-2f-PrometheUS-2f-System-2f-Sensing:
	-$(RM) ./Core/PrometheUS/System/Sensing/ClutchCurrentSampler.cyclo ./Core/PrometheUS/System/Sensing/ClutchCurrentSampler.d ./Core/PrometheUS/System/Sensing/ClutchCurrentSampler.o ./Core/PrometheUS/System/Sensing/ClutchCurrentSampler.su ./Core/PrometheUS/System/Sensing/ClutchesTemperatureSampler.cyclo ./Core/PrometheUS/System/Sensing/ClutchesTemperatureSampler.d ./Core/PrometheUS/System/Sensing/ClutchesTemperatureSampler.o ./Core/PrometheUS/System/Sensing/ClutchesTemperatureSampler.su ./Core/PrometheUS/System/Sensing/EncoderSampler.cyclo ./Core/PrometheUS/System/Sensing/EncoderSampler.d ./Core/PrometheUS/System/Sensing/EncoderSampler.o ./Core/PrometheUS/System/Sensing/EncoderSampler.su ./Core/PrometheUS/System/Sensing/GenericAdcBurstSampler.cyclo ./Core/PrometheUS/System/Sensing/GenericAdcBurstSampler.d ./Core/PrometheUS/System/Sensing/GenericAdcBurstSampler.o ./Core/PrometheUS/System/Sensing/GenericAdcBurstSampler.su ./Core/PrometheUS/System/Sensing/GenericAdcChannelSampler.cyclo ./Core/PrometheUS/System/Sensing/GenericAdcChannelSampler.d ./Core/PrometheUS/System/Sensing/GenericAdcChannelSampler.o ./Core/PrometheUS/System/Sensing/GenericAdcChannelSampler.su ./Core/PrometheUS/System/Sensing/MotorVelocitySampler.cyclo ./Core/PrometheUS/System/Sensing/MotorVelocitySampler.d ./Core/PrometheUS/System/Sensing/MotorVelocitySampler.o ./Core/PrometheUS/System/Sensing/MotorVelocitySampler.su ./Core/PrometheUS/System/Sensing/PotentiometersSampler.cyclo ./Core/PrometheUS/System/Sensing/PotentiometersSampler.d ./Core/PrometheUS/System/Sensing/PotentiometersSampler.o ./Core/PrometheUS/System/Sensing/PotentiometersSampler.su

.PHONY: clean-Core-2f-PrometheUS-2f-System-2f-Sensing

