#include <msp430.h>

unsigned int timer_period = 10000;  // Период таймера для управления миганием

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;      // Останавливаем сторожевой таймер

    // Настраиваем выводы для светодиодов
    P1DIR |= BIT0;                 // P1.0 как выход (LED1)
    P8DIR |= BIT1;                 // P8.1 как выход (LED2)
    P1OUT |= BIT0;                // LED1 выключен
    P8OUT &= ~BIT1;                // LED2 выключен

    // Настройка кнопки S2 (P2.2)
    P2DIR &= ~BIT2;                // P2.2 как вход
    P2REN |= BIT2;                 // Включаем подтягивающий резистор
    P2OUT |= BIT2;                 // Подтягиваем P2.2 к VCC
    P2IE |= BIT2;                  // Разрешаем прерывание от P2.2
    P2IES |= BIT2;                 // Прерывание по спаду
    P2IFG &= ~BIT2;                // Сбрасываем флаг прерывания

    // Настройка таймера TA2
    TA2CCTL0 = CCIE;               // Разрешаем прерывание от таймера
    TA2CCR0 = timer_period;        // Устанавливаем начальный период таймера
    TA2CTL = TASSEL_2 + MC_1 + ID_3; // Источник SMCLK, режим UP, делитель 8

    __bis_SR_register(GIE);        // Разрешаем глобальные прерывания

    while(1)
    {
        __no_operation();          // Основной цикл ничего не делает, управление через прерывания
    }
}

// Обработчик прерывания от таймера TA2
#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A(void)
{
    P1OUT ^= BIT0;                 // Переключаем LED1
    P8OUT ^= BIT1;                 // Переключаем LED2
}

// Обработчик прерывания от кнопки S2
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (P2IFG & BIT2)              // Если прерывание вызвано кнопкой S2
    {
        P2IFG &= ~BIT2;            // Сбрасываем флаг прерывания

        timer_period += 10000;       // Увеличиваем период мигания
        if (timer_period > 100000)   // Ограничиваем максимальную частоту
            timer_period = 10000;    // Возвращаемся к минимальной частоте

        TA2CCR0 = timer_period;    // Применяем новый период таймера
    }
}
