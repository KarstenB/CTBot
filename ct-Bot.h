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

/*! @file 	ct-Bot.h
 * @brief 	Demo-Hauptprogramm
 * @author 	Benjamin Benz (bbe@heise.de)
 * @date 	26.12.05
*/

#include "global.h"

/************************************************************
* Module switches, to make code smaller if features are not needed
************************************************************/
#define LOG_CTSIM_AVAILABLE		/*!< Logging ueber das ct-Sim (PC und MCU) */
//#define LOG_DISPLAY_AVAILABLE		/*!< Logging ueber das LCD-Display (PC und MCU) */
//#define LOG_UART_AVAILABLE			/*!< Logging ueber UART (NUR für MCU) */
//#define LOG_STDOUT_AVAILABLE 		/*!< Logging auf die Konsole (NUR für PC) */

#define LED_AVAILABLE		/*!< LEDs for local control */

#define IR_AVAILABLE		/*!< Infrared Remote Control */
#define RC5_AVAILABLE		/*!< Key-Mapping for IR-RC	 */

#define BOT_2_PC_AVAILABLE	/*!< Soll der Bot mit dem PC kommunmizieren? */

#define TIME_AVAILABLE		/*!< Gibt es eine Systemzeit? */

#define DISPLAY_AVAILABLE	/*!< Display for local control */
#define DISPLAY_REMOTE_AVAILABLE /*!< Sende LCD Anzeigedaten an den Simulator */
#define DISPLAY_SCREENS_AVAILABLE	/*!< Ermoeglicht vier verschiedene Screen */
#define DISPLAY_SCREEN_RESETINFO	/*!< Zeigt auf Screen 4 Informationen über Resets an */

#define DISPLAY_BEHAVIOUR_AVAILABLE  /*!< Anzeige der Verhalten im Display Screen 3, ersetzt Counteranzeige */


//#define WELCOME_AVAILABLE	/*!< kleiner Willkommensgruss */

#define ADC_AVAILABLE		/*!< A/D-Converter for sensing Power */

#define MAUS_AVAILABLE		/*!< Maus Sensor */

#define ENA_AVAILABLE		/*!< Enable-Leitungen */
#define SHIFT_AVAILABLE		/*!< Shift Register */

//#define TEST_AVAILABLE_ANALOG	/*!< Sollen die LEDs die analoge Sensorwerte anzeigen */
#define TEST_AVAILABLE_DIGITAL	/*!< Sollen die LEDs die digitale Sensorwerte anzeigen */
//#define TEST_AVAILABLE_MOTOR	/*!< Sollen die Motoren ein wenig drehen */
//#define TEST_AVAILABLE_COUNTER /*!< Gibt einen Endlos-Counter auf Screen 3 aus und aktiviert Screen 3 */
//#define DOXYGEN		/*!< Nur zum Erzeugen der Doku, wenn dieser schalter an ist, jammert der gcc!!! */

#define BEHAVIOUR_AVAILABLE /*!< Nur wenn dieser Parameter gesetzt ist, exisitiert das Verhaltenssystem */

//#define SPEED_CONTROL_AVAILABLE /*!< Aktiviert die Motorregelung */

//#define SRF10_AVAILABLE		/*!< Ultraschallsensor SRF10 vorhanden */

/************************************************************
* Some Dependencies!!!
************************************************************/

#ifdef DOXYGEN
	#define PC			/*!< Beim generieren der Doku alles anschalten */
	#define MCU		/*!< Beim generieren der Doku alles anschalten */
	#define TEST_AVAILABLE_MOTOR	/*!< Beim generieren der Doku alles anschalten */
#endif


#ifndef DISPLAY_AVAILABLE
	#undef WELCOME_AVAILABLE
   #undef DISPLAY_REMOTE_AVAILABLE
   #undef DISPLAY_SCREENS_AVAILABLE
   #undef DISPLAY_SCREEN_RESETINFO
#endif

#ifndef IR_AVAILABLE
	#undef RC5_AVAILABLE
#endif

#ifdef PC
	#ifndef DOXYGEN
		#undef UART_AVAILABLE
		#undef BOT_2_PC_AVAILABLE
		#undef SRF10_AVAILABLE
		#undef TWI_AVAILABLE
		
	#endif
	#define COMMAND_AVAILABLE		/*!< High-Level Communication */
   #undef DISPLAY_SCREEN_RESETINFO
   #undef TEST_AVAILABLE_COUNTER
#endif

#ifdef MCU
	#ifdef LOG_CTSIM_AVAILABLE
		#define BOT_2_PC_AVAILABLE
	#endif
	#ifdef BOT_2_PC_AVAILABLE
		#define UART_AVAILABLE	/*!< Serial Communication */
		#define COMMAND_AVAILABLE	/*!< High-Level Communication */
	#endif
#endif


#ifdef TEST_AVAILABLE_MOTOR
	#define TEST_AVAILABLE			/*!< brauchen wir den Testkrams */
	#define TEST_AVAILABLE_DIGITAL /*!< Sollen die LEDs die digitale Sensorwerte anzeigen */
#endif

#ifdef TEST_AVAILABLE_DIGITAL
	#define TEST_AVAILABLE			/*!< brauchen wir den Testkrams */
	#undef TEST_AVAILABLE_ANALOG
#endif

#ifdef TEST_AVAILABLE_ANALOG
	#define TEST_AVAILABLE			/*!< brauchen wir den Testkrams */
#endif

#ifdef TEST_AVAILABLE_COUNTER
	#define TEST_AVAILABLE			/*!< brauchen wir den Testkrams */
	#define DISPLAY_SCREENS_AVAILABLE
	#define DISPLAY_SCREEN_RESETINFO
#endif

#ifdef LOG_UART_AVAILABLE
	#define LOG_AVAILABLE
#endif 
#ifdef LOG_CTSIM_AVAILABLE
	#define LOG_AVAILABLE
#endif 
#ifdef LOG_DISPLAY_AVAILABLE
	#define LOG_AVAILABLE
#endif 
#ifdef LOG_STDOUT_AVAILABLE
	#define LOG_AVAILABLE
#endif 

#ifdef LOG_AVAILABLE
	#ifdef PC
		/* Auf dem PC gibts kein Logging ueber UART. */
		#undef LOG_UART_AVAILABLE
	#endif
	
	#ifdef MCU
		/* Mit Bot zu PC Kommunikation auf dem MCU gibts kein Logging ueber UART.
		 * Ohne gibts keine Kommunikation ueber ct-Sim.
		 */
		#undef LOG_STDOUT_AVAILABLE		/*!< MCU hat kein STDOUT */
		#ifdef BOT_2_PC_AVAILABLE
			#undef LOG_UART_AVAILABLE
		#else
			#undef LOG_CTSIM_AVAILABLE
		#endif
	#endif
	
	/* Ohne Display gibts auch keine Ausgaben auf diesem. */
	#ifndef DISPLAY_AVAILABLE
		#undef LOG_DISPLAY_AVAILABLE
	#endif
	
	/* Logging aufs Display ist nur moeglich, wenn mehrere Screens
	 * unterstuetzt werden.
	 */
	#ifndef DISPLAY_SCREENS_AVAILABLE
		#undef LOG_DISPLAY_AVAILABLE
	#endif
	
	/* Es kann immer nur ueber eine Schnittstelle geloggt werden. */
	
	#ifdef LOG_UART_AVAILABLE
		#define UART_AVAILABLE
		#undef LOG_CTSIM_AVAILABLE
		#undef LOG_DISPLAY_AVAILABLE
		#undef LOG_STDOUT_AVAILABLE
	#endif
	
	#ifdef LOG_CTSIM_AVAILABLE
		#undef LOG_DISPLAY_AVAILABLE
		#undef LOG_STDOUT_AVAILABLE
	#endif
	
	#ifdef LOG_DISPLAY_AVAILABLE
		#undef LOG_STDOUT_AVAILABLE
	#endif

	// Wenn keine sinnvolle Log-Option mehr uebrig, loggen wir auch nicht
	#ifndef LOG_CTSIM_AVAILABLE
		#ifndef LOG_DISPLAY_AVAILABLE
			#ifndef LOG_UART_AVAILABLE
				#ifndef LOG_STDOUT_AVAILABLE
					#undef LOG_AVAILABLE
				#endif
			#endif
		#endif
	#endif

#endif


#ifdef SRF10_AVAILABLE
	#define TWI_AVAILABLE				/*!< TWI-Schnittstelle (I2C) nutzen */
#endif


#define F_CPU	16000000L    /*!< Crystal frequency in Hz */
#define XTAL F_CPU			 /*!< Crystal frequency in Hz */

#define LINE_FEED "\n\r"	/*!< Windows und Linux unterscheiden beim Linefeed. Windows erwarten \n\r, Linux nur \n */
