/*
 ===============================================================================
 Name        : test2_LPC804.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#include <cr_section_macros.h>
#include <stdio.h>
#include "LPC8xx.h"
#include <stdint.h>
#include "fro.h"
#include "rom_api.h"
#include "syscon.h"
#include "swm.h"
#include "i2c.h"
#include "ctimer.h"
#include "core_cm0plus.h"
#include "dac.h"
#include "lib_ENS_II1_lcd.h"

#define TRIG LPC_GPIO_PORT->B0[17]
#define ECHO LPC_GPIO_PORT->B0[16]
#define M1SR LPC_GPIO_PORT->B0[14]
#define M1PWM LPC_GPIO_PORT->B0[15]
#define M2SR LPC_GPIO_PORT->B0[18]
#define M2PWM LPC_GPIO_PORT->B0[20]

int main(void) {

	//Activation du périphérique d'entrées/sorties TOR
	LPC_SYSCON->SYSAHBCLKCTRL0 |= GPIO | CTIMER0 | SWM;

	//Configuration en sortie des broches P0_11, 17, 19 et 21
	LPC_GPIO_PORT->DIR0 |= (1 << 17) | (1 << 14) | (1 << 18) | (1 << 20);

	// Configuration de l'horloge a sa frequence maximale
	LPC_PWRD_API->set_fro_frequency(30000);

	// PWM envoyant les impulsions pour les salves d'ultrasons
	LPC_CTIMER0->PR = 14;	// TC s'incremente toutes les microsecondes
	LPC_CTIMER0->MR[3] = 60000; // Le signal est periodique de 60 ms
	LPC_CTIMER0->MR[1] = 59990; // Il est a zero pendant 59,99 ms
	LPC_CTIMER0->PWMC |= (1 << 1); // La sortie MAT1 est en mode PWM
	LPC_CTIMER0->MCR |= (1 << 10); // Reset TC a chaque match avec MR [3]
	LPC_CTIMER0->EMR |= (3 << 4);
	LPC_SWM->PINASSIGN4 &= ~(0xFF00);
	LPC_SWM->PINASSIGN4 |= (17 << 8);

	return 0;

}

