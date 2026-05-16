/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Power mode definition used in application. */
typedef enum{
	BAJO_CONSUMO = 0,
	CONSUMO_NORMAL
} clkMODO_enum;

extern void SET_clockMODE(clkMODO_enum modo);

#endif /* _POWER_MODE_SWITCH_H_ */
