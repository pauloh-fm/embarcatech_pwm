#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições dos pinos
#define PWM_SERVO 22   // Pino do Servo
#define PWM_LED 12     // Pino do LED RGB
#define PWM_FREQ 50    // Frequência do PWM = 50Hz (20ms de período)

// Definições do PWM
const uint16_t WRAP_VALUE = 40000;  // 20ms período total (50Hz)
const float PWM_DIVIDER = 40.0;     // Divisor do clock do PWM

// Variável de controle
int modo_operacao = 1;  // 1 = Servo, 2 = LED

// Função para configurar o PWM na GPIO especificada
void pwm_setup(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);  // Configura a GPIO como PWM
    uint slice = pwm_gpio_to_slice_num(pin); // Obtém o slice PWM do pino

    pwm_set_clkdiv(slice, PWM_DIVIDER);  // Configura o divisor do clock
    pwm_set_wrap(slice, WRAP_VALUE);     // Define o período PWM (20ms)
    pwm_set_gpio_level(pin, 500);        // Define duty cycle inicial
    pwm_set_enabled(slice, true);        // Ativa o PWM
}

// Função para ajustar o duty cycle
void set_pwm_duty(uint pin, uint16_t duty) {
    pwm_set_gpio_level(pin, duty);
    printf("Modo: %s | Ciclo ativo: %d us\n", (modo_operacao == 1) ? "Servo" : "LED", duty);
}

int main() {
    stdio_init_all();

    // Configura o PWM conforme o modo de operação
    if (modo_operacao == 1) {
        pwm_setup(PWM_SERVO);
    } else {
        pwm_setup(PWM_LED);
    }

    while (true) {
        if (modo_operacao == 1) {
            // Controle do Servo Motor
            set_pwm_duty(PWM_SERVO, 2400);  // Posição 180°
            sleep_ms(5000);

            set_pwm_duty(PWM_SERVO, 1470);  // Posição 90°
            sleep_ms(5000);

            set_pwm_duty(PWM_SERVO, 500);   // Posição 0°
            sleep_ms(5000);

            // Movimento  periódico
            for (uint16_t i = 500; i <= 2400; i += 5) {
                set_pwm_duty(PWM_SERVO, i);
                sleep_ms(10);
            }
            for (uint16_t i = 2400; i >= 500; i -= 5) {
                set_pwm_duty(PWM_SERVO, i);
                sleep_ms(10);
            }
        } else {
            // Controle do LED RGB
            set_pwm_duty(PWM_LED, 2400);  // LED mais brilhante
            sleep_ms(5000);

            set_pwm_duty(PWM_LED, 1470);  // LED brilho médio
            sleep_ms(5000);

            set_pwm_duty(PWM_LED, 500);   // LED mais fraco
            sleep_ms(5000);

            // Movimento  periódico de brilho
            for (uint16_t i = 500; i <= 2400; i += 5) {
                set_pwm_duty(PWM_LED, i);
                sleep_ms(10);
            }
            for (uint16_t i = 2400; i >= 500; i -= 5) {
                set_pwm_duty(PWM_LED, i);
                sleep_ms(10);
            }
        }
    }

    return 0;
}
