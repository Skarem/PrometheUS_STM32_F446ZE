################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/PrometheUS/Sensing/ClutchCurrentSampler.cpp \
../Core/PrometheUS/Sensing/ClutchesTemperatureSampler.cpp \
../Core/PrometheUS/Sensing/EncoderSampler.cpp \
../Core/PrometheUS/Sensing/GenericAdcBurstSampler.cpp \
../Core/PrometheUS/Sensing/GenericAdcChannelSampler.cpp \
../Core/PrometheUS/Sensing/MotorVelocitySampler.cpp \
../Core/PrometheUS/Sensing/PotentiometersSampler.cpp 

OBJS += \
./Core/PrometheUS/Sensing/ClutchCurrentSampler.o \
./Core/PrometheUS/Sensing/ClutchesTemperatureSampler.o \
./Core/PrometheUS/Sensing/EncoderSampler.o \
./Core/PrometheUS/Sensing/GenericAdcBurstSampler.o \
./Core/PrometheUS/Sensing/GenericAdcChannelSampler.o \
./Core/PrometheUS/Sensing/MotorVelocitySampler.o \
./Core/PrometheUS/Sensing/PotentiometersSampler.o 

CPP_DEPS += \
./Core/PrometheUS/Sensing/ClutchCurrentSampler.d \
./Core/PrometheUS/Sensing/ClutchesTemperatureSampler.d \
./Core/PrometheUS/Sensing/EncoderSampler.d \
./Core/PrometheUS/Sensing/GenericAdcBurstSampler.d \
./Core/PrometheUS/Sensing/GenericAdcChannelSampler.d \
./Core/PrometheUS/Sensing/MotorVelocitySampler.d \
./Core/PrometheUS/Sensing/PotentiometersSampler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/PrometheUS/Sensing/%.o Core/PrometheUS/Sensing/%.su Core/PrometheUS/Sensing/%.cyclo: ../Core/PrometheUS/Sensing/%.cpp Core/PrometheUS/Sensing/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/PrometheUS -I../Core/PrometheUS/System -I../Core/PrometheUS/Control -I../Core/PrometheUS/Actuation -I../Core/PrometheUS/Sensing -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-PrometheUS-2f-Sensing

clean-Core-2f-PrometheUS-2f-Sensing:
	-$(RM) ./Core/PrometheUS/Sensing/ClutchCurrentSampler.cyclo ./Core/PrometheUS/Sensing/ClutchCurrentSampler.d ./Core/PrometheUS/Sensing/ClutchCurrentSampler.o ./Core/PrometheUS/Sensing/ClutchCurrentSampler.su ./Core/PrometheUS/Sensing/ClutchesTemperatureSampler.cyclo ./Core/PrometheUS/Sensing/ClutchesTemperatureSampler.d ./Core/PrometheUS/Sensing/ClutchesTemperatureSampler.o ./Core/PrometheUS/Sensing/ClutchesTemperatureSampler.su ./Core/PrometheUS/Sensing/EncoderSampler.cyclo ./Core/PrometheUS/Sensing/EncoderSampler.d ./Core/PrometheUS/Sensing/EncoderSampler.o ./Core/PrometheUS/Sensing/EncoderSampler.su ./Core/PrometheUS/Sensing/GenericAdcBurstSampler.cyclo ./Core/PrometheUS/Sensing/GenericAdcBurstSampler.d ./Core/PrometheUS/Sensing/GenericAdcBurstSampler.o ./Core/PrometheUS/Sensing/GenericAdcBurstSampler.su ./Core/PrometheUS/Sensing/GenericAdcChannelSampler.cyclo ./Core/PrometheUS/Sensing/GenericAdcChannelSampler.d ./Core/PrometheUS/Sensing/GenericAdcChannelSampler.o ./Core/PrometheUS/Sensing/GenericAdcChannelSampler.su ./Core/PrometheUS/Sensing/MotorVelocitySampler.cyclo ./Core/PrometheUS/Sensing/MotorVelocitySampler.d ./Core/PrometheUS/Sensing/MotorVelocitySampler.o ./Core/PrometheUS/Sensing/MotorVelocitySampler.su ./Core/PrometheUS/Sensing/PotentiometersSampler.cyclo ./Core/PrometheUS/Sensing/PotentiometersSampler.d ./Core/PrometheUS/Sensing/PotentiometersSampler.o ./Core/PrometheUS/Sensing/PotentiometersSampler.su

.PHONY: clean-Core-2f-PrometheUS-2f-Sensing

