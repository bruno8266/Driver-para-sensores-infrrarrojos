/**
 * @file sensor_infrarrojo.c
 * @brief Driver para interactuar con Sensores Infrarrojos
 * @author Bruno Benítez
 * @date 2024-7-24 año/mes/día
 * @version v0.2024724
 */
/*Driver: sensor infrarrojo*/

#ifndef SENSOR_INFRARROJO_H
#define SENSOR_INFRARROJO_H

#include <avr/io.h>

/**
 * @brief Configuramos el puerto utilizado por el sensor infrarrojo
 * 
 * @param ddr Data Direction Register a configurar
 * @param ddr_bit Bit del ddr a configurar
 * @param port Puerto a configurar
 * @param port_bit Bit del port a
 */
void IR_init(_SFR_BYTE ddr, uint8_t ddr_bit, _SFR_BYTE port, uint8_t port_bit)

/**
 * @brief Lee el estado del sensor
 * 
 * @param port Es el puerto de entrada donde está el sensor
 * @param port_bit Es el bit a leer
 * 
 * @return uint8_t 1 si detecta luz, 0 en caso contrario
 */
uint8_t IR_fast_detect(_SFR_BYTE port, uint8_t port_bit)

/**
 * @brief Inicialización del modo ADC
 * 
 * @note Se justifica a la izquierda. Por esto se desprecia los 2(dos) bits menos significativos de ADC
 * @note Por esto se desprecia los 2(dos) bits menos significativos de ADC.
 */
void IR_adc_init();

/**
 * @brief Lectura del estado del sensor utilizando el modo ADC, conversión simple. 
 * El valor de retorno depende de porcentaje_umbral
 * 
 * @param adc_x Canal utilizado por el sensor, del ADC0 al ADC7.
 * @param porcentaje_umbral porcentaje de VCC para comparar con el resultado de la conversión
 * @return uint8_t 1 si la conversión es mayor o igual al umbral elegido, 0 en caso contrario
 */
uint8_t IR_slow_detect(uint8_t adc_x, uint8_t porcentaje_umbral);

#endif