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
static const uint8_t LIMITE_MINUTOS[] = {6,0};
static const uint8_t LIMITE_HORAS[] = {2,4};



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
  if((numero[0]==limite[0])&&(numero[1]==limite[1])){
    numero[0] = 0;
    numero[1] = 0;
  }
}

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]){
  numero[1]--;
  if(numero[1]>9){
    numero[1] = 0;
    numero[0]--;
  }
  if((numero[0] >= limite[0])&&(numero[1] > limite[1])){
    numero[0] = 0;
    numero[1] = 0;
  }
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
      if(modo == MOSTRANDO_HORA){
        if(!clockGetAlarm(clock, entrada, sizeof(entrada))){
          clockToggleAlarm(clock);
        }
      } else if(modo == AJUSTANDO_MINUTOS_ACTUAL) {
        modo = AJUSTANDO_HORAS_ACTUAL;
      } else if(modo == AJUSTANDO_HORAS_ACTUAL){
        configClock(clock, entrada, sizeof(entrada));
        CambiarModo(MOSTRANDO_HORA);
      } else if(modo == AJUSTANDO_MINUTOS_ALARMA){
        modo = AJUSTANDO_HORAS_ALARMA;
      } else if(modo == AJUSTANDO_HORAS_ALARMA){
        clockSetUpAlarm(clock, entrada, sizeof(entrada));
        CambiarModo(MOSTRANDO_HORA);
      }

    }

    if(DigitalInputHasActivated(board->cancel)){
      if(modo == MOSTRANDO_HORA){
        if(clockGetAlarm(clock, entrada, sizeof(entrada))){
          clockToggleAlarm(clock);
        }
      }
      else{
        if(getTimeClock(clock, entrada, sizeof(entrada))){
          CambiarModo(MOSTRANDO_HORA);
        }
        else {
          CambiarModo(HORA_SIN_AJUSTAR);
        }
      }
    }

    if(DigitalInputHasActivated(board->set_time)){
      CambiarModo(AJUSTANDO_MINUTOS_ACTUAL);
      getTimeClock(clock, entrada, sizeof(entrada));
      DisplayWriteBCD(board->display, entrada, sizeof(entrada)); 
    }

    if(DigitalInputHasActivated(board->set_alarm)){
      CambiarModo(AJUSTANDO_MINUTOS_ALARMA);
      clockGetAlarm(clock, entrada, sizeof(entrada));
      DisplayWriteBCD(board->display, entrada, sizeof(entrada));
      DisplayToggleDots(board->display,0,3);
    }

    if(DigitalInputHasActivated(board->decrement)){
      if((modo == AJUSTANDO_MINUTOS_ACTUAL)||(modo == AJUSTANDO_MINUTOS_ALARMA)){
        DecrementarBCD(&entrada[2], LIMITE_MINUTOS);
      }
      else if((modo == AJUSTANDO_HORAS_ACTUAL)||(modo==AJUSTANDO_HORAS_ALARMA)){
        DecrementarBCD(&entrada[0], LIMITE_HORAS);
      }

      if((modo == AJUSTANDO_MINUTOS_ACTUAL)||(modo == AJUSTANDO_HORAS_ACTUAL)){
        DisplayWriteBCD(board->display, entrada, sizeof(entrada));
      }
      else if((modo == AJUSTANDO_MINUTOS_ALARMA)||(modo==AJUSTANDO_HORAS_ALARMA)){
        DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        DisplayToggleDots(board->display,0,3);
      }
    }

    if(DigitalInputHasActivated(board->increment)){
      if((modo == AJUSTANDO_MINUTOS_ACTUAL)||(modo == AJUSTANDO_MINUTOS_ALARMA)){
        IncrementarBCD(&entrada[2], LIMITE_MINUTOS);
      }
      else if((modo == AJUSTANDO_HORAS_ACTUAL)||(modo==AJUSTANDO_HORAS_ALARMA)){
        IncrementarBCD(&entrada[0], LIMITE_HORAS);
      }

      if((modo == AJUSTANDO_MINUTOS_ACTUAL)||(modo == AJUSTANDO_HORAS_ACTUAL)){
        DisplayWriteBCD(board->display, entrada, sizeof(entrada));
      }
      else if((modo == AJUSTANDO_MINUTOS_ALARMA)||(modo==AJUSTANDO_HORAS_ALARMA)){
        DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        DisplayToggleDots(board->display,0,3);
      } 
    }

    

    while (true) {



      for (int delay = 0; delay < 25000; delay++) {
          __asm("NOP");
      }   
    }
}

void Systick_Handler(void){ // Rutina de interrupcion se ejecuta 1000 veces por segundo
  uint16_t contador = 0; 
  uint8_t hora[4]; 
  
  DisplayRefresh(board->display);
  tickClock(clock); 
  
  contador = (contador + 1) % 1000;

  if(modo<=MOSTRANDO_HORA){
    getTimeClock(clock, hora, sizeof(hora));
    DisplayWriteBCD(board->display,hora, sizeof(hora));
    if(contador > 500){
      DisplayToggleDots(board->display,1,1);
    }
    if(clockGetAlarm(clock, hora, sizeof(hora))){
      DisplayToggleDots(board->display,3,3);
    }
  }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
