/**
 * @file sensor_infrarrojo.c
 * @brief Driver para interactuar con Sensores Infrarrojos
 * @author Bruno Benítez
 * @date 2024-8-5 año/mes/día
 * @version v0.202485
 */
/*Driver: sensor infrarrojo*/



#include <avr/io.h>
#include <sensorInfrarrojo.h>


/**
 * @brief Configuramos el puerto utilizado por el sensor infrarrojo
 * 
 * @param ddr Data Direction Register a configurar
 * @param ddr_bit Bit del ddr a configurar
 * @param port Puerto a configurar
 * @param port_bit Bit del port a configurar
 */

void IR_init(volatile uint8_t *ddr, uint8_t ddr_bit, volatile uint8_t *port, uint8_t port_bit) /*_MMIO_BYTE*/
{
    /*Configuración del puerto como entrada*/
    *ddr &= ~(1 << ddr_bit);
    *port &= ~( 1<< port_bit);; 
}

/**
 * @brief Lee el estado del sensor
 * 
 * @param port Es el puerto de entrada donde está el sensor
 * @param port_bit Es el bit a leer
 * 
 * @return uint8_t 1 si detecta luz, 0 en caso contrario
 */
uint8_t IR_fast_detect(volatile uint8_t *pin, uint16_t pin_bit)
{

	if( ((*pin) & 1<<(pin_bit)) == (1<<pin_bit))
		return 1;
    
    return 0;
}

/**
 * @brief Inicialización del modo ADC
 * 
 */
void IR_adc_init()
{
	ADCSRA = MASK(ADEN) /* Habilitamos el modo ADC*/
			 | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); /* Divisor de frecuencia de 128*/
	ADMUX = (0<<REFS1) | (1<<REFS0) /*Tensión de referencia: AVCC*/
			 | (0<<ADLAR) /*Justificado a la derecha*/
			 | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);
																								 
	/* Realizamos una conversión para completar IR_adc_init*/
	ADCSRA |= (1<<ADSC);

	/* Esperamos que se termine la conversión*/
	while ((ADCSRA & (1<<ADSC)) != 0);                      
}

/**
 * @brief Lectura de la intensidad de luz recibida por el sensor, utilizando el modo ADC: conversión simple. 
 * 
 * @param uint8_t canal Canal utilizado por el sensor, del ADC0 al ADC7.
 * @return uint16_t ADC resultado de la conversión, con valores de 0 a 1023
 * @note Para calcular el valor de tensión equivalente Vin, calcular Vin = (ADC * Vcc)/1023
 */
uint16_t IR_adc_detect(uint8_t canal)
{
	canal &= ((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
	ADMUX = (ADMUX & 0xF0) | canal;
    ADCSRA |= (1<<ADSC);/*Seteamos ADSC (ADC Start Conversion) para iniciar la conversión*/

    while ((ADCSRA & (1<<ADSC)) != 0);/*Esperamos que la conversion este lista*/
                                        /*Normalmente la conversión toma 13 ciclos*/

    return ADC;
}

/**
 * @brief Calibración del umbral de luz para el sensor aplicado para un seguidor de línea
 * 
 * @param canal Canal a calibrar
 * @param cantidad_iteraciones Cantidad de mediciones del estado del sensor
 * @return uint16_t El umbral calculado como el promedio entre la lectura de mayor y menor valor
 */
uint16_t calibracion_sensor(uint8_t canal, uint16_t cantidad_iteraciones)/* Calculo el valor umbral para el color negro*/
{
	uint16_t i = 0;
	uint16_t lectura_adc[cantidad_iteraciones];
	uint16_t lectura_max = 0;
	uint16_t lectura_min = 0;
	
	/*Almacenamos las lecturas del sensor*/
	for (i=0; i < cantidad_iteraciones; i++)
	{
		lectura_adc[i] = IR_adc_detect(canal);
	}
	
	/*Buscamos los valores extremos*/
	for (i = 0; i < cantidad_iteraciones; i++)
	{
		if(lectura_adc[i] > lectura_max){
			lectura_max = lectura_adc[i];
		}
		else if(lectura_adc[i] < lectura_min){
			lectura_min = lectura_adc[i];
		}
	}

	uint16_t umbral = (lectura_max + lectura_min)/2;/*Umbral es el promedio de los valores extremos*/
	return umbral;
}


//IR_analog_comparator_intit
/*Si quisieramos utilizar el comparador analógico del ATMEGA, necesitamos poner la tensión umbral en la pata negativa*/
/*Pero el ATMEGA no tiene DAC*/
/*Si se quisiera, se podría utilizar el PWM y filtrarlo para conseguir la tensión umbral
 ---> requiere añadir hardware, la librería perdería su gracia*/