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
#include "mrt.h"

#define TRIG LPC_GPIO_PORT->B0[17]
#define ECHO LPC_GPIO_PORT->B0[16]
#define M1SR LPC_GPIO_PORT->B0[14]
#define M1PWM LPC_GPIO_PORT->B0[15]
#define M2SR LPC_GPIO_PORT->B0[18]
#define M2PWM LPC_GPIO_PORT->B0[20]

#define borne_mrt0_bas 60000
#define borne_mrt0_haut 60010

uint32_t compteur_mrt0=0;

void MRT_IRQHandler(void){
	if ((LPC_MRT->Channel[0].STAT & 1) == 1){ //Interruption due au MRT0
		if (TRIG == 0 && compteur_mrt0 < borne_mrt0_bas){
			compteur_mrt0++;
		}
		else if (TRIG == 0 && compteur_mrt0 >= borne_mrt0_bas){
			compteur_mrt0++;
			TRIG=1;
		}
		else if (TRIG == 1 && compteur_mrt0 < borne_mrt0_haut){
			compteur_mrt0++;
		}
		else if (TRIG == 1 && compteur_mrt0 >= borne_mrt0_haut){
			compteur_mrt0=0;
			TRIG=0;
		}
		LPC_MRT->Channel[0].STAT |= 1; //On pense a abaisser le drapeau d'interruption
	}
}

void CTIMER0_IRQHandler(void) {
	int delta_t = 0;
	int distance = 0;
	char temps[32];
	delta_t = LPC_CTIMER0->CR[0] / 15000000;
	distance = (delta_t*34000)/2;
	sprintf(temps, "Distance :  %d cm", distance);
	lcd_puts(temps);
	lcd_gohome();
	LPC_CTIMER0->IR |= (1 << 4);

int main(void) {

	//Activation du périphérique d'entrées/sorties TOR
	LPC_SYSCON->SYSAHBCLKCTRL0 |= GPIO | CTIMER0 | SWM | GPIO_INT | MRT;

	//Configuration en sortie des broches P0_11, 17, 19 et 21
	LPC_GPIO_PORT->DIR0 |= (1 << 17) | (1 << 14) | (1 << 18) | (1 << 20);

	// Configuration de l'horloge a sa frequence maximale
	LPC_PWRD_API->set_fro_frequency(30000);

	// PWM envoyant les impulsions pour les salves d'ultrasons
	LPC_MRT->Channel[0].CTRL |= (1<<0); //Active l'interruption du MRT0
	LPC_MRT->Channel[0].CTRL |= (0<<1); //Active le mode a interruptions repetees

	NVIC_EnableIRQ(MRT_IRQn);  //Active les interruptions du MRT

	LPC_MRT->Channel[0].INTVAL = 15; //La valeur du timer du MRT0 diminue chaque microseconde



	// Configuration de la capture de la duree de l'onde recue
	LPC_CTIMER0->TCR = (1 << CEN);
	LPC_SWM->PINASSIGN3 &= ~(0xFF00);
	LPC_SWM->PINASSIGN3 |= (16 << 8);	// Le capture se fait vis à vis du BP1
	LPC_CTIMER0->CTCR |= (1 << ENCC) | (0 << SELCC); // Remise à zéro du timer lors d'un front montant
	LPC_CTIMER0->CCR |= (0b011 << 0); // On capture sur front montant
	NVIC_EnableIRQ(CTIMER0_IRQn);

	while(1){

	}
	return 0;

}

