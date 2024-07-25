#include <avr/io.h>
#include "util/delay.h"
#include <HotWheels.h>
#include <infrarrojo.h>


// Variables del motor
int motor_speed_base = 50;
int motor_speed_left = 0;
int motor_speed_right = 0;

int main(void)
{
    /*Variable del PID*/
    int8_t Kp = 1;
    int8_t Ki = 1;
    int8_t Kd = 5;
    int8_t error = 0;
    int8_t error_previo = 0;
    int8_t integral = 0;
    int8_t derivada = 0;
    int8_t correccion = 0;

    // Variables del sensor
    uint8_t sensor_left = 0;
    uint8_t sensor_right = 0;
    // Inicialización de sensores y motores
    infrarrojo_init();
    onYourMarks();

    while(1) 
    {
        // Leer sensores
        sensor_left = detectar_linea_izquierda();
        sensor_right = detectar_linea_derecha();

        // Calcular error basado en los sensores
        if (sensor_left && sensor_right) {
            error = 0; // El robot está centrado
        } else if (sensor_left && !sensor_right) {
            error = 1; // El robot necesita girar a la derecha
        } else if (!sensor_left && sensor_right) {
            error = -1; // El robot necesita girar a la izquierda
        }

        // Calcular términos del PID
        integral = integral + error;
        derivada = error - error_previo;
        correccion = Kp * error + Ki * integral + Kd * derivada;

        // Ajustar velocidad de motores basado en la corrección
        motor_speed_left = motor_speed_base + correccion;
        motor_speed_right = motor_speed_base - correccion;

        // Limitar velocidad de motores a valores permitidos
        if(motor_speed_left > 100)
            {motor_speed_left = 100;}
        else if(motor_speed_left < 0)
            {motor_speed_left = 0;}
        if(motor_speed_right > 100)
            {motor_speed_right = 100;}
        else if(motor_speed_right < 0)
            {motor_speed_right = 0;}

        // Establecer velocidades de motores
        setMotorSpeedLeft(motor_speed_left);
        setMotorSpeedRight(motor_speed_right);

        // Guardar el error actual para el siguiente cálculo de derivada
        error_previo = error;

        // Pequeño retraso para estabilizar la lectura de sensores y el control
        _delay_ms(10);
    }
}