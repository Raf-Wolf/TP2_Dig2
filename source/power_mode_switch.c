/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "fsl_port.h"
#include "power_mode_switch.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/



/*==================[internal functions definition]==========================*/

void APP_SetClockVlpr(void);
void APP_SetClockRunFromVlpr(void);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
//extern void APP_SetClockRunFromVlpr(void);

/*
 * Hook function called before power mode switch.
 */
//extern void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*
 * Hook function called after power mode switch.
 */
//extern void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static clkMODO_enum curPowerState = CONSUMO_NORMAL;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * Set the clock configuration for VLPR mode.
 */
void APP_SetClockVlpr(void) {
	const mcglite_config_t mcgliteConfig = {
	        .outSrc                  = kMCGLITE_ClkSrcLirc,
	        .irclkEnableMode         = kMCGLITE_IrclkEnable,
	        .ircs                    = kMCGLITE_Lirc2M,
	        .fcrdiv                  = kMCGLITE_LircDivBy1,
	        .lircDiv2                = kMCGLITE_LircDivBy1,
	        .hircEnableInNotHircMode = false,
	    };

	    const sim_clock_config_t simConfig = {
	        .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
	#if (defined(FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION) && FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION)
	        .er32kSrc = 0U,         /* SIM_SOPT1[OSC32KSEL]. */
	#endif
	    };

	    CLOCK_SetSimSafeDivs();
	    CLOCK_SetMcgliteConfig(&mcgliteConfig);
	    CLOCK_SetSimConfig(&simConfig);
}

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
void APP_SetClockRunFromVlpr(void) {
	const mcglite_config_t mcgliteConfig = {.outSrc                  = kMCGLITE_ClkSrcHirc,
	                                            .irclkEnableMode         = 0U,
	                                            .ircs                    = kMCGLITE_Lirc8M,
	                                            .fcrdiv                  = kMCGLITE_LircDivBy1,
	                                            .lircDiv2                = kMCGLITE_LircDivBy1,
	                                            .hircEnableInNotHircMode = true};

	    const sim_clock_config_t simConfig = {
	        .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
	#if (defined(FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION) && FSL_FEATURE_SIM_OPT_HAS_OSC32K_SELECTION)
	        .er32kSrc = 0U,         /* SIM_SOPT1[OSC32KSEL]. */
	#endif
	    };

	    CLOCK_SetSimSafeDivs();
	    CLOCK_SetMcgliteConfig(&mcgliteConfig);
	    CLOCK_SetSimConfig(&simConfig);
}

/*
 * Sets the clock configuration to RUN or VLPR mode. Allowed parameters are BAJO_CONSUMO and CONSUMO_NORMAL
 */
extern void SET_clockMODE(clkMODO_enum modo){
	if(modo == BAJO_CONSUMO){
		APP_SetClockVlpr();
        SMC_SetPowerModeVlpr(SMC);
        while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
        {
        }
	}else if(modo == CONSUMO_NORMAL){
		SMC_SetPowerModeRun(SMC);
		while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
		{
		}
        /* If enter RUN from VLPR, change clock after the power mode change. */
		if (BAJO_CONSUMO == curPowerState)
		{
			APP_SetClockRunFromVlpr();
		}
	}
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000U);
	curPowerState = modo;
}
