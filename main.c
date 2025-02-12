#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições dos pinos
#define PWM_SERVO 22  // GPIO do Servo Motor
#define PWM_LED 12    // GPIO do LED RGB

// Definição do modo de operação (1 = Servo, 2 = LED)
#define modo_operacao 1  // TODO:Altere para 2 para testar com LED

const uint16_t WRAP_PERIOD = 40000; // Valor máximo do contador PWM para 50Hz
const float PWM_DIVISER = 64.0;      // Divisor do clock para PWM

// Ciclos ativos em microssegundos para posições do servo
const uint16_t ANGLE_0 = 500;    // =~0 graus
const uint16_t ANGLE_90 = 1470;  // =~90 graus
const uint16_t ANGLE_180 = 2400; // =~ 180 graus
const uint16_t INCREMENTO = 5;   // Incremento de 5µs para suavidade

// Função para configurar o módulo PWM
void pwm_setup(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM); // Configura GPIO como PWM
    uint slice = pwm_gpio_to_slice_num(gpio); // Obtém o canal PWM

    pwm_set_clkdiv(slice, PWM_DIVISER); // Define o divisor de clock do PWM
    pwm_set_wrap(slice, WRAP_PERIOD); // Define o valor de wrap para 50Hz
    pwm_set_gpio_level(gpio, 0); // Inicializa PWM com ciclo de trabalho 0
    pwm_set_enabled(slice, true); // Habilita PWM no slice correspondente
}

// Define o ciclo de trabalho do servo em microssegundos
void set_servo_position(uint gpio, uint16_t ANGLE_width_us) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint16_t duty_cycle = (ANGLE_width_us * WRAP_PERIOD) / 20000; // Converte para ciclo PWM
    pwm_set_gpio_level(gpio, duty_cycle);
}

// Define o brilho do LED RGB baseado na posição do servo
void set_led_brightness(uint gpio, uint16_t ANGLE_width_us) {
    uint16_t brightness = (ANGLE_width_us * WRAP_PERIOD) / 2400; // Ajusta brilho proporcionalmente
    pwm_set_gpio_level(gpio, brightness);
}

int main() {
    stdio_init_all();
    
    if (modo_operacao == 1) {
        pwm_setup(PWM_SERVO); // Configura PWM para servo
    } else if (modo_operacao == 2) {
        pwm_setup(PWM_LED); // Configura PWM para LED
    }

    while (true) {
        if (modo_operacao == 1) {
            // **Controle do Servo**
            set_servo_position(PWM_SERVO, ANGLE_180);
            sleep_ms(5000);

            set_servo_position(PWM_SERVO, ANGLE_90);
            sleep_ms(5000);

            set_servo_position(PWM_SERVO, ANGLE_0);
            sleep_ms(5000);

            // Movimento contínuo entre 0 e 180 graus
            while (true) {
                for (uint16_t ANGLE = ANGLE_0; ANGLE <= ANGLE_180; ANGLE += INCREMENTO) {
                    set_servo_position(PWM_SERVO, ANGLE);
                    sleep_ms(10);
                }

                sleep_ms(1000); // Espera 1s em 180 graus

                for (uint16_t ANGLE = ANGLE_180; ANGLE >= ANGLE_0; ANGLE -= INCREMENTO) {
                    set_servo_position(PWM_SERVO, ANGLE);
                    sleep_ms(10);
                }

                sleep_ms(1000); // Espera 1s em 0 graus
            }
        } 
        
        else if (modo_operacao == 2) {
            // **Controle do LED RGB**
            set_led_brightness(PWM_LED, ANGLE_180);
            sleep_ms(5000);

            set_led_brightness(PWM_LED, ANGLE_90);
            sleep_ms(5000);

            set_led_brightness(PWM_LED, ANGLE_0);
            sleep_ms(5000);

            // Simulação do movimento do servo ajustando brilho do LED
            while (true) {
                for (uint16_t ANGLE = ANGLE_0; ANGLE <= ANGLE_180; ANGLE += INCREMENTO) {
                    set_led_brightness(PWM_LED, ANGLE);
                    sleep_ms(10);
                }

                sleep_ms(1000); // Espera 1s no máximo brilho

                for (uint16_t ANGLE = ANGLE_180; ANGLE >= ANGLE_0; ANGLE -= INCREMENTO) {
                    set_led_brightness(PWM_LED, ANGLE);
                    sleep_ms(10);
                }

                sleep_ms(1000); // Espera 1s no mínimo brilho
            }
        }
    }

    return 0;
}
