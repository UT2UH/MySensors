/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2019 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */
#ifndef MyHwSTM32L0_h
#define MyHwSTM32L0_h

#include <SPI.h>
#include <avr/dtostrf.h>

#ifdef __cplusplus
#include <Arduino.h>
#endif

#define CRYPTO_LITTLE_ENDIAN

#ifndef MY_SERIALDEVICE
#define MY_SERIALDEVICE Serial
#endif

#ifndef MY_DEBUGDEVICE
#define MY_DEBUGDEVICE MY_SERIALDEVICE
#endif

#ifndef MY_STM32L0_TEMPERATURE_OFFSET
#define MY_STM32L0_TEMPERATURE_OFFSET (0.0f)
#endif

#ifndef MY_STM32L0_TEMPERATURE_GAIN
#define MY_STM32L0_TEMPERATURE_GAIN (1.0f)
#endif

// SS default
#ifndef SS
#define SS 17
#endif

// mapping
#define snprintf_P(s, f, ...) snprintf((s), (f), __VA_ARGS__)
#define vsnprintf_P(s, n, f, ...) vsnprintf((s), (n), (f), __VA_ARGS__)

#ifndef digitalPinToInterrupt
#define digitalPinToInterrupt(__pin) (__pin)
#endif

// Define these as macros to save valuable space
#define hwDigitalWrite(__pin, __value) digitalWrite(__pin, __value)
#define hwDigitalRead(__pin) digitalRead(__pin)
#define hwPinMode(__pin, __value) pinMode(__pin, __value)
//#define hwWatchdogReset() iwdg_feed()
//#define hwReboot() nvic_sys_reset()
#define hwMillis() millis()
#define hwRandomNumberInit() randomSeed(analogRead(MY_SIGNING_SOFT_RANDOMSEED_PIN))
#define hwGetSleepRemaining() (0ul)

bool hwInit(void);
void hwWatchdogReset(void);
void hwReboot(void);
void hwReadConfigBlock(void *buf, void *addr, size_t length);
void hwWriteConfigBlock(void *buf, void *addr, size_t length);
void hwWriteConfig(const int addr, uint8_t value);
uint8_t hwReadConfig(const int addr);

// SOFTSPI
#ifdef MY_SOFTSPI
#error Soft SPI is not available on this architecture!
#endif
#define hwSPI SPI //!< hwSPI


/**
 * Disable all interrupts.
 * Helper function for MY_CRITICAL_SECTION.
 */
static __inline__ uint8_t __disableIntsRetVal(void)
{
	__disable_irq();
	return 1;
}

/**
 * Restore priority mask register.
 * Helper function for MY_CRITICAL_SECTION.
 */
static __inline__ void __priMaskRestore(const uint32_t *priMask)
{
	__set_PRIMASK(*priMask);
}

#ifndef DOXYGEN
#define MY_CRITICAL_SECTION    for ( uint32_t __savePriMask __attribute__((__cleanup__(__priMaskRestore))) = __get_PRIMASK(), __ToDo = __disableIntsRetVal(); __ToDo ; __ToDo = 0 )
#endif  /* DOXYGEN */

#endif // #ifdef ARDUINO_ARCH_STM32L0
