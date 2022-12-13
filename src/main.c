/** @file main.c
 * \brief Introducing sample to the use of ciaa board with a simple project
 **   
 **  This sample is part of the EITI course called Embedded Systems. This program turn on and off 
 **  different leds of the boards with different buttons

 ** | RV | YYYY.MM.DD | Author      | Changes description                     |
 ** |----|------------|-------------|-----------------------------------------|
 ** |  1 | 2022.09.14 | fedeiz      | V1.0
 ** 
 **
  ** @{ */

/* === Headers files inclusions =============================================================== */


#include "bsp.h" /** \brief Here we include the headers to use a HAL module for this board */
#include <stdbool.h>
#include <stdint.h>
#include "poncho.h"
#include "chip.h"
#include "pantalla.h"
#include "clock.h"    

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */
typedef enum {
  HORA_SIN_AJUSTAR,
  MOSTRANDO_HORA,
  AJUSTANDO_MINUTOS_ACTUAL,
  AJUSTANDO_HORAS_ACTUAL,
  AJUSTANDO_MINUTOS_ALARMA,
  AJUSTANDO_HORAS_ALARMA
} modo_t;
/* === Private variable declarations =========================================================== */
   
/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

static board_t board;
static modo_t modo; 
static clock_t clock;
static const uint8_t LIMITE_MINUTOS[] = {5,9};
static const uint8_t LIMITE_HORAS[] = {2,3};


/* === Private function implementation ========================================================= */
void CambiarModo(modo_t valor){
  modo = valor;
  switch (modo)
  {
  case HORA_SIN_AJUSTAR:
    DisplayFlashDigits(board->display, 0, 3, 250);
    break;
  case MOSTRANDO_HORA:
    DisplayFlashDigits(board->display, 0, 0, 0 );
    break;
  case AJUSTANDO_MINUTOS_ACTUAL:
    DisplayFlashDigits(board->display, 2, 3, 250);
    break;
  case AJUSTANDO_HORAS_ACTUAL:
    DisplayFlashDigits(board->display, 0, 1, 250);
    break;
  case AJUSTANDO_MINUTOS_ALARMA:
    DisplayFlashDigits(board->display, 2, 3, 250);
    break;
  case AJUSTANDO_HORAS_ALARMA:
    DisplayFlashDigits(board->display, 0, 1, 250);  
    break;
  default:
    break;
  }
}

void SonarAlarma(clock_t clock, bool state){}

void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]){
  numero[1]++;
  if(numero[1]>9){
    numero[1] = 0;
    numero[0]++;
  }
  if((numero[0]>limite[0])&&(numero[1]>limite[1])){
    numero[0] = 0;
    numero[1] = 0;
  }
}

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]){
  
}
/* === Public function implementation ========================================================= */
/**
 * This program has four different linking leds. 
 * 1) The blue led remain on while the first button is pressed
 * 2) The red led toggle whith consecutive second button event 
 * 3) The yellow led can be turn on with a third button and off with the fourth button
 * 4) The green led blink beetween 5 cicles program 
 */


int main(void) {  
    
    uint8_t entrada[4]; 
    

    board_t board = BoardCreate();
    clock = createClock(100, SonarAlarma);

    SisTick_Init(1000); // configuro el ciclo de tiempo de la interrupcion
    CambiarModo(HORA_SIN_AJUSTAR); // RELOJ ARRANCA CON HORA SIN AJUSTAR

    if(DigitalInputHasActivated(board->accept)){
      
    }

    if(DigitalInputHasActivated(board->cancel)){

    }

    if(DigitalInputHasActivated(board->set_time)){
      CambiarModo(AJUSTANDO_MINUTOS_ACTUAL);
      getTimeClock(clock, entrada, sizeof(entrada));
      DisplayWriteBCD(board->display, entrada, sizeof(entrada)); 
    }

    if(DigitalInputHasActivated(board->set_alarm)){}

    if(DigitalInputHasActivated(board->decrement)){
      if((modo == AJUSTANDO_MINUTOS_ACTUAL)||(modo == AJUSTANDO_MINUTOS_ALARMA)){

      }
      else if((modo == AJUSTANDO_HORAS_ACTUAL)||(modo==AJUSTANDO_HORAS_ALARMA)){

      }
    }

    if(DigitalInputHasActivated(board->increment)){
      if((modo == AJUSTANDO_MINUTOS_ACTUAL)||(modo == AJUSTANDO_MINUTOS_ALARMA)){
        IncrementarBCD(&entrada[2], LIMITE_MINUTOS);
      }
      else if((modo == AJUSTANDO_HORAS_ACTUAL)||(modo==AJUSTANDO_HORAS_ALARMA)){
        IncrementarBCD(&entrada[0], LIMITE_HORAS);
      } 
    }

    // DisplayWriteBCD(board->display, numbers, sizeof(numbers)); // writing 1 2 3 4 in the screen

    while (true) {

     // DisplayRefresh(board->display);

      for (int delay = 0; delay < 25000; delay++) {
          __asm("NOP");
      }   
    }
}

void Systick_Handler(void){ // Rutina de interrupcion
  uint8_t hora[4]; 
  DisplayRefresh(board->display);
  tickClock(clock); 
  if(modo<=MOSTRANDO_HORA){
    getTimeClock(clock, hora, sizeof(hora));
    DisplayWriteBCD(board->display,hora, sizeof(hora));
  }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
