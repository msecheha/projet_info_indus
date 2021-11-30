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

int main(void) {

	//Activation du périphérique d'entrées/sorties TOR
	LPC_SYSCON->SYSAHBCLKCTRL0 |= GPIO | CTIMER0 | SWM;

	//Configuration en sortie des broches P0_11, 17, 19 et 21
	LPC_GPIO_PORT->DIR0 |= (1 << 17) | (1 << 14) | (1 << 18) | (1 << 20);

	// Configuration de l'horloge a sa frequence maximale
	LPC_PWRD_API->set_fro_frequency(30000);

	return 0;

}

