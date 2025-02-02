/*
De início, o vermelho será ligado por 3 segundos e terá seu timer ativado,
depois de 3 segundo um alarme ligará o amarelo e seu timer,
por fim, após 6 segundos, outro alarme ligará o verde e seu timer
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h" // temporização via hardware

#define GREEN 11
#define BLUE 12
#define RED 13


// Esta estrutura armazenará informações sobre o temporizador configurado.
//Foi necessário criar várias structs pra não dar conflito com os tempos
//Só é usada nos repeating timer, não é necessário colocar nos alarmes
struct repeating_timer timer1,timer2,timer3;

bool repeating_timer_callback_YELLOW(struct repeating_timer *);
bool repeating_timer_callback_RED(struct repeating_timer *);
bool repeating_timer_callback_GREEN(struct repeating_timer *);

int64_t alarm_callback_yellow(alarm_id_t id, void *user_data) {
 gpio_put(GREEN,1);
 gpio_put(BLUE,0);
 gpio_put(RED,1);
 add_repeating_timer_ms(9000, repeating_timer_callback_YELLOW, NULL, &timer2);
 puts("Ativando LED amarelo pela PRIMEIRA vez");
 return 0;
}

int64_t alarm_callback_green(alarm_id_t id, void *user_data) {
 gpio_put(GREEN,1);
 gpio_put(BLUE,0);
 gpio_put(RED,0);
 add_repeating_timer_ms(9000, repeating_timer_callback_GREEN, NULL, &timer3);
 puts("Ativando LED verde pela PRIMEIRA vez");
 return 0;
}

bool repeating_timer_callback_RED(struct repeating_timer *t) {

    puts("Ativando LED vermelho");
    gpio_put(GREEN,0);
    gpio_put(BLUE,0);
    gpio_put(RED,1);
    // se o retorno for verdadeiro; cria-se um loop na temporização
    return true;
}

bool repeating_timer_callback_YELLOW(struct repeating_timer *t) {
    puts("Ativando LED amarelo");
    gpio_put(GREEN,1);
    gpio_put(BLUE,0);
    gpio_put(RED,1);
    
    // se o retorno for verdadeiro; cria-se um loop na temporização
    return true;
}

bool repeating_timer_callback_GREEN(struct repeating_timer *t) {
    puts("Ativando LED verde");
    gpio_put(GREEN,1);
    gpio_put(BLUE,0);
    gpio_put(RED,0);
    
    // se o retorno for verdadeiro; cria-se um loop na temporização
    return true;
}


int main()
{
    stdio_init_all();

    gpio_init(RED);
    gpio_set_dir(RED, GPIO_OUT);

    gpio_init(GREEN);
    gpio_set_dir(GREEN, GPIO_OUT);

    gpio_init(BLUE);
    gpio_set_dir(BLUE, GPIO_OUT);

    //Aciona o vermelho uma única vez no início
    gpio_put(GREEN,0);
    gpio_put(BLUE,0);
    gpio_put(RED,1);

    add_repeating_timer_ms(9000, repeating_timer_callback_RED, NULL, &timer1);
    puts("Ativando LED vermelho pela primeira vez");
    
    add_alarm_in_ms(3000, alarm_callback_yellow, NULL, false);
    add_alarm_in_ms(6000, alarm_callback_green, NULL, false);

    // Loop infinito que mantém o programa em execução.
    while (true) {
        sleep_ms(1000);
        printf("Passou-se %d segundo(s)\n",to_ms_since_boot(get_absolute_time())/1000);
    }
    return 0;
}