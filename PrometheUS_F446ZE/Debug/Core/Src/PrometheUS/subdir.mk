################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/PrometheUS/ClutchCurrentSampler.cpp \
../Core/Src/PrometheUS/ClutchesTemperatureSampler.cpp \
../Core/Src/PrometheUS/EncoderSampler.cpp \
../Core/Src/PrometheUS/GenericAdcBurstSampler.cpp \
../Core/Src/PrometheUS/GenericAdcChannelSampler.cpp \
../Core/Src/PrometheUS/MotorVelocitySampler.cpp \
../Core/Src/PrometheUS/PotentiometersSampler.cpp \
../Core/Src/PrometheUS/PrometheUS.cpp 

OBJS += \
./Core/Src/PrometheUS/ClutchCurrentSampler.o \
./Core/Src/PrometheUS/ClutchesTemperatureSampler.o \
./Core/Src/PrometheUS/EncoderSampler.o \
./Core/Src/PrometheUS/GenericAdcBurstSampler.o \
./Core/Src/PrometheUS/GenericAdcChannelSampler.o \
./Core/Src/PrometheUS/MotorVelocitySampler.o \
./Core/Src/PrometheUS/PotentiometersSampler.o \
./Core/Src/PrometheUS/PrometheUS.o 

CPP_DEPS += \
./Core/Src/PrometheUS/ClutchCurrentSampler.d \
./Core/Src/PrometheUS/ClutchesTemperatureSampler.d \
./Core/Src/PrometheUS/EncoderSampler.d \
./Core/Src/PrometheUS/GenericAdcBurstSampler.d \
./Core/Src/PrometheUS/GenericAdcChannelSampler.d \
./Core/Src/PrometheUS/MotorVelocitySampler.d \
./Core/Src/PrometheUS/PotentiometersSampler.d \
./Core/Src/PrometheUS/PrometheUS.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/PrometheUS/%.o Core/Src/PrometheUS/%.su Core/Src/PrometheUS/%.cyclo: ../Core/Src/PrometheUS/%.cpp Core/Src/PrometheUS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/PrometheUS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-PrometheUS

clean-Core-2f-Src-2f-PrometheUS:
	-$(RM) ./Core/Src/PrometheUS/ClutchCurrentSampler.cyclo ./Core/Src/PrometheUS/ClutchCurrentSampler.d ./Core/Src/PrometheUS/ClutchCurrentSampler.o ./Core/Src/PrometheUS/ClutchCurrentSampler.su ./Core/Src/PrometheUS/ClutchesTemperatureSampler.cyclo ./Core/Src/PrometheUS/ClutchesTemperatureSampler.d ./Core/Src/PrometheUS/ClutchesTemperatureSampler.o ./Core/Src/PrometheUS/ClutchesTemperatureSampler.su ./Core/Src/PrometheUS/EncoderSampler.cyclo ./Core/Src/PrometheUS/EncoderSampler.d ./Core/Src/PrometheUS/EncoderSampler.o ./Core/Src/PrometheUS/EncoderSampler.su ./Core/Src/PrometheUS/GenericAdcBurstSampler.cyclo ./Core/Src/PrometheUS/GenericAdcBurstSampler.d ./Core/Src/PrometheUS/GenericAdcBurstSampler.o ./Core/Src/PrometheUS/GenericAdcBurstSampler.su ./Core/Src/PrometheUS/GenericAdcChannelSampler.cyclo ./Core/Src/PrometheUS/GenericAdcChannelSampler.d ./Core/Src/PrometheUS/GenericAdcChannelSampler.o ./Core/Src/PrometheUS/GenericAdcChannelSampler.su ./Core/Src/PrometheUS/MotorVelocitySampler.cyclo ./Core/Src/PrometheUS/MotorVelocitySampler.d ./Core/Src/PrometheUS/MotorVelocitySampler.o ./Core/Src/PrometheUS/MotorVelocitySampler.su ./Core/Src/PrometheUS/PotentiometersSampler.cyclo ./Core/Src/PrometheUS/PotentiometersSampler.d ./Core/Src/PrometheUS/PotentiometersSampler.o ./Core/Src/PrometheUS/PotentiometersSampler.su ./Core/Src/PrometheUS/PrometheUS.cyclo ./Core/Src/PrometheUS/PrometheUS.d ./Core/Src/PrometheUS/PrometheUS.o ./Core/Src/PrometheUS/PrometheUS.su

.PHONY: clean-Core-2f-Src-2f-PrometheUS

