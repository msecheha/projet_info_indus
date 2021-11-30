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

	LPC_CTIMER0->PR = 14;	// TC s'incremente toutes les microsecondes

	return 0;

}

void impulsion_capteur()
	// Fonction qui genere l'impulsion de 10 microsecondes
	// necessaire pour lancer la salve d'ultrasons
	{
	LPC_CTIMER0->TCR = (1 << CEN);
	while (LPC_CTIMER0->TC < 10) {
		LPC_GPIO_PORT->PIN0 = (1 << 17);
		// On genere une impulsion de 10 microsecondes
	}
	LPC_GPIO_PORT->NOT0 = (1 << 17);
	LPC_CTIMER0->TCR = (0 << CEN);
	// On pense a desactiver le compteur apres
}

