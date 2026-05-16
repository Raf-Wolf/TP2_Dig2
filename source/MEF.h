/*
 * MEF.h
 *
 *  Created on: 10 may 2026
 *      Author: PabloB
 */

#ifndef MEF_H_
#define MEF_H_

typedef enum{
	INICIO = 0,
	REPOSO,
	CAIDA,
	MOSTRAR_ACE
} EST_enum;

extern void MEF_task1ms(void);
extern void MEF(void);
extern void MEF_init(void);
#endif /* MEF_H_ */
