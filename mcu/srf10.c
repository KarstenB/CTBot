/*
 * c't-Sim - Robotersimulator fuer den c't-Bot
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your
 * option) any later version. 
 * This program is distributed in the hope that it will be 
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public 
 * License along with this program; if not, write to the Free 
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307, USA.
 * 
 */

/*! @file 	srf10.c  
 * @brief 	Ansteuerung des Ultraschall Entfernungssensors SRF10
 * @author 	Carsten Giesen (info@cnau.de)
 * @date 	08.04.06
*/
#ifdef MCU 
#include <avr/io.h>
#include "TWI_driver.h"
#include "srf10.h"
#include "delay.h"

static uint8 address=SRF10_UNIT_0;

/*!
 * SRF10 initialsieren
 */
 
void srf10_init(void){
	srf10_set_range(SRF10_MAX_RANGE);
	//srf10_set_range(6);	//Mit diesem Wert muss man spielen um das Optimum zu ermitteln
return;
}

/*!
 * Verstaerkungsfaktor setzen
 * @param gain Verstaerkungsfaktor
 */
 
void srf10_set_gain(unsigned char gain){
    if(gain>16) { gain=16; }

	uint8 temp[2];
	uint8 state;
	tx_type tx_frame[2];
	
	state = SUCCESS;

	tx_frame[0].slave_adr = address+W;
	tx_frame[0].size = 2;
	tx_frame[0].data_ptr = temp;
	tx_frame[0].data_ptr[0] = 1;
	tx_frame[0].data_ptr[1] = gain;

	tx_frame[1].slave_adr = OWN_ADR;

	state = Send_to_TWI(tx_frame);
}

/*!
 * Reichweite setzen, hat auch Einfluss auf die Messdauer
 * @param millimeters Reichweite in mm
 */
 
void srf10_set_range(unsigned int millimeters){
	uint8 temp[2];
	uint8 state;
	tx_type tx_frame[2];
	
	state = SUCCESS;

    millimeters= (millimeters/43); 

	tx_frame[0].slave_adr = address+W;
	tx_frame[0].size = 2;
	tx_frame[0].data_ptr = temp;
	tx_frame[0].data_ptr[0] = 2;
	tx_frame[0].data_ptr[1] = millimeters;

	tx_frame[1].slave_adr = OWN_ADR;

	state = Send_to_TWI(tx_frame);
}

/*!
 * Messung ausloesen
 * @param metric_unit 0x50 in Zoll, 0x51 in cm, 0x52 ms
 * @return Resultat der Aktion
 */

uint8 srf10_ping(uint8 metric_unit){
	uint8 temp[2];
	uint8 state;
	tx_type tx_frame[2];
	
	state = SUCCESS;

	tx_frame[0].slave_adr = address+W;
	tx_frame[0].size = 2;
	tx_frame[0].data_ptr = temp;
	tx_frame[0].data_ptr[0] = SRF10_COMMAND;
	tx_frame[0].data_ptr[1] = metric_unit;

	tx_frame[1].slave_adr = OWN_ADR;

	state = Send_to_TWI(tx_frame);
	
	return state;
}

/*!
 * Register auslesen
 * @param srf10_register welches Register soll ausgelsen werden
 * @return Inhalt des Registers
 */

uint8 srf10_read_register(uint8 srf10_register){
	uint8 temp;
	uint8 value;
	uint8 state;
	tx_type tx_frame[3];

	state = SUCCESS;
	value = 0;

	tx_frame[0].slave_adr = address+W;
	tx_frame[0].size = 1;
	tx_frame[0].data_ptr = &temp;
	tx_frame[0].data_ptr[0] = srf10_register;

	tx_frame[1].slave_adr = address+R;
	tx_frame[1].size = 1;
	tx_frame[1].data_ptr = &value;

	tx_frame[2].slave_adr = OWN_ADR;

	state = Send_to_TWI(tx_frame);
	
	return value;
}

/*!
 * Messung starten Ergebniss aufbereiten und zurueckgeben
 * @return Messergebniss
 */

uint16 srf10_get_measure(){
	char hib;
	char lob;
	char state;

	state = SUCCESS;

	state = srf10_ping(SRF10_CENTIMETERS);
	delay(10);									//Optimierungs Potential
	lob=srf10_read_register(SRF10_LOB);
	delay(10);									//Optimierungs Potential
	hib=srf10_read_register(SRF10_HIB);
	
	return (hib*256)+lob;
}

#endif
