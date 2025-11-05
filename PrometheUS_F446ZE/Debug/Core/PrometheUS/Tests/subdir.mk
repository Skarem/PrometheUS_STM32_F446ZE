################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/PrometheUS/Tests/TestClutchCurrents.cpp \
../Core/PrometheUS/Tests/TestClutchTemperatures.cpp \
../Core/PrometheUS/Tests/TestPotentiometers.cpp \
../Core/PrometheUS/Tests/TestPwm.cpp \
../Core/PrometheUS/Tests/TestSender.cpp 

OBJS += \
./Core/PrometheUS/Tests/TestClutchCurrents.o \
./Core/PrometheUS/Tests/TestClutchTemperatures.o \
./Core/PrometheUS/Tests/TestPotentiometers.o \
./Core/PrometheUS/Tests/TestPwm.o \
./Core/PrometheUS/Tests/TestSender.o 

CPP_DEPS += \
./Core/PrometheUS/Tests/TestClutchCurrents.d \
./Core/PrometheUS/Tests/TestClutchTemperatures.d \
./Core/PrometheUS/Tests/TestPotentiometers.d \
./Core/PrometheUS/Tests/TestPwm.d \
./Core/PrometheUS/Tests/TestSender.d 


# Each subdirectory must supply rules for building sources it contributes
Core/PrometheUS/Tests/%.o Core/PrometheUS/Tests/%.su Core/PrometheUS/Tests/%.cyclo: ../Core/PrometheUS/Tests/%.cpp Core/PrometheUS/Tests/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/PrometheUS -I../Core/PrometheUS/Tests -I../Core/PrometheUS/System -I../Core/PrometheUS/Control -I../Core/PrometheUS/Actuation -I../Core/PrometheUS/Sensing -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-PrometheUS-2f-Tests

clean-Core-2f-PrometheUS-2f-Tests:
	-$(RM) ./Core/PrometheUS/Tests/TestClutchCurrents.cyclo ./Core/PrometheUS/Tests/TestClutchCurrents.d ./Core/PrometheUS/Tests/TestClutchCurrents.o ./Core/PrometheUS/Tests/TestClutchCurrents.su ./Core/PrometheUS/Tests/TestClutchTemperatures.cyclo ./Core/PrometheUS/Tests/TestClutchTemperatures.d ./Core/PrometheUS/Tests/TestClutchTemperatures.o ./Core/PrometheUS/Tests/TestClutchTemperatures.su ./Core/PrometheUS/Tests/TestPotentiometers.cyclo ./Core/PrometheUS/Tests/TestPotentiometers.d ./Core/PrometheUS/Tests/TestPotentiometers.o ./Core/PrometheUS/Tests/TestPotentiometers.su ./Core/PrometheUS/Tests/TestPwm.cyclo ./Core/PrometheUS/Tests/TestPwm.d ./Core/PrometheUS/Tests/TestPwm.o ./Core/PrometheUS/Tests/TestPwm.su ./Core/PrometheUS/Tests/TestSender.cyclo ./Core/PrometheUS/Tests/TestSender.d ./Core/PrometheUS/Tests/TestSender.o ./Core/PrometheUS/Tests/TestSender.su

.PHONY: clean-Core-2f-PrometheUS-2f-Tests

