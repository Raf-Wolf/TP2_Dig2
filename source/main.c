/*
 * Copyright 2016-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    SD2_Tp2_Acelerometro.c
 * @brief   Application entry point.
 */
#include "SD2_board.h"
#include "board.h"
#include "SD2_I2C.h"
#include "oled.h"
#include "key.h"
#include "MEF.h"
#include "fsl_smc.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

	/* inicializa interrupción de systick cada 1 ms */

	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

    /* Se habilita la posibilidad de operar con todos los modos de bajo consumo */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000U);
	board_init();
	SD2_I2C_init();
	//init del oled
	board_configSPI0();
	oled_setContrast(16);
	oled_init();

	key_init();
	MEF_init();
	//bool Caida = false;

    while(1) {
    	MEF();
    }
    return 0 ;
}

void SysTick_Handler(void)
{
	key_periodicTask1ms();
	MEF_task1ms();

}
