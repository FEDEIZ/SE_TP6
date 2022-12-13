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

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */
/**
 * This program has four different linking leds. 
 * 1) The blue led remain on while the first button is pressed
 * 2) The red led toggle whith consecutive second button event 
 * 3) The yellow led can be turn on with a third button and off with the fourth button
 * 4) The green led blink beetween 5 cicles program 
 */


int main(void) {  
    

    uint8_t numbers[4] = {1,2,3,4}; 

    board_t board = BoardCreate();

    SisTick_Init(1000); // configuro el ciclo de tiempo de la interrupcion
    
    if(DigitalInputHasActivated(board->accept)){
      DisplayWriteBCD(board->display,(uint8_t []){1,2,3,4}, 4);
      DisplayToggleDots(board->display,1,2)
    }

    if(DigitalInputHasActivated(board->cancel)){
      DisplayWriteBCD(board->display,NULL, 0);
    }

    if(DigitalInputHasActivated(board->set_time)){}

    if(DigitalInputHasActivated(board->set_alarm)){}

    if(DigitalInputHasActivated(board->decrement)){}

    if(DigitalInputHasActivated(board->increment)){}

    DisplayWriteBCD(board->display, numbers, sizeof(numbers)); // writing 1 2 3 4 in the screen

    while (true) {

     // DisplayRefresh(board->display);

      for (int delay = 0; delay < 25000; delay++) {
          __asm("NOP");
      }   
    }
}

void Systick_Handler(void){ // Rutina de interrupcion
  DisplayRefresh(board->display);
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
