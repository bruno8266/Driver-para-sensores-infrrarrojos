/**
 * @file sensor_infrarrojo.c
 * @brief Driver para interactuar con Sensores Infrarrojos
 * @author Bruno Benítez
 * @date 2024-7-24 año/mes/día
 * @version v0.2024724
 */
/*Driver: sensor infrarrojo*/



#include <avr/io.h>
#include <avr/iom1284p.h>
#include <sensor_infrarrojo.h>

#define MASK(x) (1 << x)


/**
 * @brief Configuramos el puerto utilizado por el sensor infrarrojo
 * 
 * @param ddr Data Direction Register a configurar
 * @param ddr_bit Bit del ddr a configurar
 * @param port Puerto a configurar
 * @param port_bit Bit del port a
 */
void IR_init(_SFR_BYTE ddr, uint8_t ddr_bit, _SFR_BYTE port, uint8_t port_bit)
{
    /*Configuración del puerto como entrada*/
    ddr &= ~MASK(ddr_bit);
    port |= MASK(port_bit); 
}

/**
 * @brief Lee el estado del sensor
 * 
 * @param port Es el puerto de entrada donde está el sensor
 * @param port_bit Es el bit a leer
 * 
 * @return uint8_t 1 si detecta luz, 0 en caso contrario
 */
uint8_t IR_fast_detect(_SFR_BYTE port, uint8_t port_bit)
{
    if(bit_is_set(port, MASK(port_bit)) == MASK(port_bit))/*Verificamos el estado del sensor*/
        return 1;/*Detección de luz*/
    
    return 0;/*No se detecta luz*/
}

/////////////////////////////////////////////////////////////////////////////
    //Y si le sumamos una magia +?  


/**
 * @brief Inicialización del modo ADC
 * 
 * @note Se justifica a la izquierda. Por esto se desprecia los 2(dos) bits menos significativos de ADC
 * @note Por esto se desprecia los 2(dos) bits menos significativos de ADC.
 */
void IR_adc_init()
{
    ADMUX = MASK(ADLAR) & /*Justifica a la izquierda*/
            ~(MASK(REFS1)) | MASK(REFS0); /*Tensión de referencia AVCC*/                       
}


/**
 * @brief Lectura del estado del sensor utilizando el modo ADC, conversión simple. 
 * El valor de retorno depende de porcentaje_umbral
 * 
 * @param adc_x Canal utilizado por el sensor, del ADC0 al ADC7.
 * @param porcentaje_umbral Porcentaje de VCC para comparar con el resultado de la conversión
 * @return uint8_t 1 si la conversión es mayor o igual al umbral elegido, 0 en caso contrario
 */
uint8_t IR_slow_detect(uint8_t adc_x, uint8_t porcentaje_umbral)
{
    uint8_t umbral = 256/100 * porcentaje_umbral - 1;/*Conversión del valor umbral. Ahora su rango es 0-255, para poder comparar con ADCH */

    ADMUX |= MASK(adc_x) & 0b00000111; /*Configuramos el puerto del sensor*/
    ADCSRA = MASK(ADSC);/*Seteamos ADSC para iniciar la conversión*/

    while (ADCSRA && MASK(ADIF) == MASK(ADIF)){}/*Esperamos que la conversion este lista*/

    if(ADCH >= umbral) /*Comparamos la conversión con el umbral*/
        {return 1; } /*Si la conversión supera al umbral, retorno 1*/

    return 0; /*Si la conversión no supera al umbral, retorno 0*/
}



//IR_analog_comparator_intit
/*Si quisieramos utilizar el comparador analógico del ATMEGA, necesitamos poner la tensión umbral en la pata negativa*/
/*Pero el ATMEGA no tiene DAC*/
/*Si se quisiera, se podría utilizar el PWM y filtrarlo para conseguir la tensión umbral
 ---> requiere añadir hardware, la librería perdería su gracia*/