/*
 * MEF.c
 *
 *  Created on: 10 may 2026
 *      Author: PabloB
 */
#include <stdio.h>
#include "SD2_board.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

#include "fxls8974.h"
#include "SD2_I2C.h"
#include "key.h"
#include "MEF.h"
#include "power_mode_switch.h"
#include "oled.h"

#define T250 250
#define T10S 10000

static bool TM_250ms_ft;
static bool TM_10s_ft;
static EST_enum estado;
static uint32_t tim250ms;
static uint32_t tim10s;

static void tim250ms_ST(){
	tim250ms = T250;
	TM_250ms_ft = false;
}
static void tim10s_ST(){
	tim10s = T10S;
	TM_10s_ft = false;
}

void MEF_init(){
	estado = INICIO;
}

void MEF(){
	switch(estado){
		case INICIO:
			 board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
			 // oled apagado
			 oled_clearScreen(OLED_COLOR_BLACK);
			 // resto de funciones
			 SET_configuracion(CONF_CAIDA_LIBRE);

			 estado = REPOSO;

			 // clock en bajo consumo
			 SET_clockMODE(BAJO_CONSUMO);

			 break;
		case REPOSO:

			if(Caida_detectada()){
				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
				//paso a la configuracion de impacto
				SET_configuracion(CONF_IMPACTO);
				tim250ms_ST();
				estado = CAIDA;
			}
			break;
		case CAIDA:

			if(TM_250ms_ft){
				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
				tim250ms_ST();
			}

			if(IMPACTO()){
				// Mostrar el maximo valor de ace en el oled
				Show_oled(Get_Ace_max());

				tim10s_ST();
				estado = MOSTRAR_ACE;
				key_getPressEv(BOARD_SW_ID_2);
			}
			break;
		case MOSTRAR_ACE:

			Show_oled(Get_Ace_max());

			if(TM_250ms_ft){
				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
				tim250ms_ST();
			}

			if(TM_10s_ft || key_getPressEv(BOARD_SW_ID_2)){
				// apago el led
				board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
				// resetear valores de aceleracion
				RESET_valores();
				//dejar de mostra aceleracion en el oled
				oled_clearScreen(OLED_COLOR_BLACK);
				// vuelvo a la configuracion de caida libre
				SET_configuracion(CONF_CAIDA_LIBRE);
				estado = REPOSO;
				//pasar a bajo consumo
				SET_clockMODE(BAJO_CONSUMO);
			}
			break;
	}
}

void MEF_task1ms(void){
    if(tim10s)
        tim10s--;
    else
        TM_10s_ft = true;

    if(tim250ms)
        tim250ms--;
    else
        TM_250ms_ft = true;
}
