################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Consonant.cpp \
../src/PlotFileGenerator.cpp \
../src/PraatFileIO.cpp \
../src/Syllable.cpp \
../src/SystemTA.cpp \
../src/TrainingFileGenerator.cpp \
../src/Vowel.cpp \
../src/Word.cpp \
../src/_findqta.cpp \
../src/_linkqta.cpp \
../src/_plotqta.cpp \
../src/_sampa2vec.cpp \
../src/utilities.cpp 

OBJS += \
./src/Consonant.o \
./src/PlotFileGenerator.o \
./src/PraatFileIO.o \
./src/Syllable.o \
./src/SystemTA.o \
./src/TrainingFileGenerator.o \
./src/Vowel.o \
./src/Word.o \
./src/_findqta.o \
./src/_linkqta.o \
./src/_plotqta.o \
./src/_sampa2vec.o \
./src/utilities.o 

CPP_DEPS += \
./src/Consonant.d \
./src/PlotFileGenerator.d \
./src/PraatFileIO.d \
./src/Syllable.d \
./src/SystemTA.d \
./src/TrainingFileGenerator.d \
./src/Vowel.d \
./src/Word.d \
./src/_findqta.d \
./src/_linkqta.d \
./src/_plotqta.d \
./src/_sampa2vec.d \
./src/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -D__cplusplus=201103L -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


