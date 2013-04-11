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

/*! @file 	ct-Bot.c
 * @brief 	Demo-Hauptprogramm
 * @author 	Benjamin Benz (bbe@heise.de)
 * @date 	26.12.05
*/

#include "ct-Bot.h"

#ifdef MCU
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/signal.h>
	#include <avr/wdt.h>
	#include "bot-2-pc.h"
#endif
	
#ifdef PC
	#include "bot-2-sim.h"
	#include "tcp.h"
	#include "tcp-server.h"
	#include <pthread.h>
	#include <unistd.h>
	#include <stdlib.h>
#endif

#ifdef TWI_AVAILABLE
	#include "TWI_driver.h"
#endif

#include <string.h>
#include <stdio.h>

#include "global.h"
#include "display.h"
#include "led.h"
#include "ena.h"
#include "shift.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "sensor.h"
#include "log.h"


#include "motor.h"
#include "sensor-low.h"
#include "bot-logik.h"
#include "mouse.h"

#include "command.h"
#include "ir-rc5.h"
#include "rc5.h"
#include "timer.h"

/* Nimmt den Status von MCUCSR bevor dieses Register auf 0x00 gesetzt wird */
#ifdef DISPLAY_SCREEN_RESETINFO
	uint8 reset_flag; 
#endif

#ifdef TEST_AVAILABLE_COUNTER
	#include <avr/eeprom.h>
	uint8 resetsEEPROM  __attribute__ ((section (".eeprom")))=0;
	uint8 resetInfoEEPROM  __attribute__ ((section (".eeprom")));
	uint8 resets;
#endif
/*!
 * Der Mikrocontroller und der PC-Simulator brauchen ein paar Einstellungen, 
 * bevor wir loslegen koennen.
 */
void init(void){
	
	#ifdef MCU
		PORTA=0; DDRA=0;		//Alles Eingang alles Null
		PORTB=0; DDRB=0;
		PORTC=0; DDRC=0;
		PORTD=0; DDRD=0;
		
		// Watchdog aus!	
		wdt_disable();
		#ifdef DISPLAY_SCREEN_RESETINFO
			reset_flag = MCUCSR & 0x1F;	//Lese Grund fuer Reset und sichere Wert
			MCUCSR = 0;	//setze Register auf 0x00 (loeschen)
		#endif		
	#endif

	#ifdef UART_AVAILABLE
		uart_init();
	#endif

	#ifdef BOT_2_PC_AVAILABLE
		bot_2_pc_init();
	#endif


	#ifdef PC
		bot_2_sim_init();
	#endif

	#ifdef DISPLAY_AVAILABLE
		display_init();
		display_update=1;
	#endif

	#ifdef LED_AVAILABLE
		LED_init();
	#endif

	motor_init();
	bot_sens_init();
	bot_behave_init();
	
	#ifdef MCU
		#ifdef RC5_AVAILABLE
			ir_init();
		#endif
	#endif
			
	#ifdef MAUS_AVAILABLE
		maus_sens_init();
	#endif
	
	#ifdef DISPLAY_BEHAVIOUR_AVAILABLE
	    behaviour_page = 1;
	#endif

	#ifdef TWI_AVAILABLE
		Init_TWI();
		Close_TWI();
	#endif

}

#ifdef DISPLAY_AVAILABLE

/*!
 * Zeigt ein paar Informationen an
 */
	void display(void){
		
		#ifdef DISPLAY_BEHAVIOUR_AVAILABLE
		 /*!
          * Definitionen fuer die Verhaltensanzeige
          */
		  #undef TEST_AVAILABLE_COUNTER
		  Behaviour_t	*ptr	= behaviour;
		  int8 colcounter       = 0;
		  int8 linecounter      = 0;
		  int8 firstcol         = 0; 
		#endif 
		
		#ifdef TEST_AVAILABLE_COUNTER
			static int counter=0;
		#endif
 		if (display_update >0)
 			#ifdef DISPLAY_SCREENS_AVAILABLE
			switch (display_screen) {
				case 0:
			#endif
					display_cursor(1,1);
					display_printf("P=%03X %03X D=%03d %03d ",sensLDRL,sensLDRR,sensDistL,sensDistR);
		
					display_cursor(2,1);
					display_printf("B=%03X %03X L=%03X %03X ",sensBorderL,sensBorderR,sensLineL,sensLineR);
		
					display_cursor(3,1);
					display_printf("R=%2d %2d F=%d K=%d T=%d ",sensEncL % 10,sensEncR %10,sensError,sensDoor,sensTrans);
		
					display_cursor(4,1);
					display_printf("I=%04X M=%05d %05d",RC5_Code,sensMouseX,sensMouseY);
			#ifdef 	DISPLAY_SCREENS_AVAILABLE					
					break;
				case 1:
					#ifdef TIME_AVAILABLE
						display_cursor(1,1);
						display_printf("Zeit: %04d:%03d", timer_get_s(), timer_get_ms());
					#endif

					display_cursor(2,1);
					display_printf("TS=%+4d %+4d",target_speed_l,target_speed_r);
					#ifdef SRF10_AVAILABLE		
						display_cursor(2,15);
						display_printf("US%+4d",sensSRF10);
					#endif
		
					display_cursor(3,1);
					display_printf("RC=%+4d %+4d",sensEncL,sensEncR);
		
					display_cursor(4,1);
					display_printf("Speed= %04d %04d",v_left,v_right);
					break;

				case 2:
					display_cursor(1,1);
					  
                   #ifdef DISPLAY_BEHAVIOUR_AVAILABLE
                    /*!
                     * zeilenweise Anzeige der Verhalten
                     */ 
					display_printf("Verhalten (Pri/Akt)%d",behaviour_page);
					
					colcounter = 0; linecounter = 2;
					/* je nach Seitenwahl die ersten  Saetze ueberlesen bis richtige Seite */
					firstcol = (behaviour_page -1)*6;
					 
					/*!
					 * max. 3 Zeilen mit 6 Verhalten anzeigbar wegen Ueberschrift
					 * Seitensteuerung bei mehr Verhalten 
					 */ 
					while((ptr != NULL)&& (linecounter<5))	{
					 
					  if  ((ptr->priority > 2) &&(ptr->priority <= 200)) {
                        if   (colcounter >= firstcol) { 
				          display_cursor(linecounter,((colcounter % 2)* 12)+1);
					      display_printf(" %3d,%2d",ptr->priority,ptr->active_new);				      
					      colcounter++;
					    
					      /* bei colcounter 0 neue Zeile */
					      if (colcounter % 2 == 0)
					  	    linecounter++;
					      
					    }
					    else
					    colcounter ++;
					  }
					  ptr = ptr->next;
				    }  
					
				    #endif
					 

					#ifdef TEST_AVAILABLE_COUNTER	
					    display_printf("Screen 3");					
						display_cursor(2,1);
						display_printf("count %d",counter++);

						display_cursor(3,1);
						display_printf("Reset-Counter %d",resets);
					#endif
					break;

				case 3:
					display_cursor(1,1);
					#ifdef DISPLAY_SCREEN_RESETINFO
						/* Zeige den Grund fuer Resets an */
						display_printf("MCUCSR - Register");
												
						display_cursor(2,1);
						display_printf("PORF :%d  WDRF :%d",binary(reset_flag,0),binary(reset_flag,3)); 

						display_cursor(3,1);
						display_printf("EXTRF:%d  JTRF :%d",binary(reset_flag,1),binary(reset_flag,4)); 
									
						display_cursor(4,1);
						display_printf("BORF :%d",binary(reset_flag,2)); 
					#else
						display_printf("Screen 4");
					#endif
					break;
					
				case 4:
					/* Wird zur Ausgabe von Loggings verwendet. */
					break;
			}
			#endif	
	}
#endif

#ifdef TEST_AVAILABLE
	/*! Zeigt den internen Status der Sensoren mit den LEDs an */
	void show_sensors(void){
		uint8 led=0x00;
		led_t * status = (led_t *)&led;
		#ifdef TEST_AVAILABLE_ANALOG
			(*status).rechts	= (sensDistR >> 9) & 0x01;
			(*status).links		= (sensDistL >> 9) & 0x01;
			(*status).rot		= (sensLineL >> 9) & 0x01;
			(*status).orange	= (sensLineR >> 9) & 0x01;
			(*status).gelb		= (sensLDRL >> 9) & 0x01;
			(*status).gruen		= (sensLDRR >> 9) & 0x01;
			(*status).tuerkis	= (sensBorderL >> 9) & 0x01;
			(*status).weiss		= (sensBorderR >> 9) & 0x01;
		#endif
		#ifdef TEST_AVAILABLE_DIGITAL
			(*status).rechts	= sensEncR  & 0x01;
			(*status).links		= sensEncL  & 0x01;
			(*status).rot		= sensTrans & 0x01;
			(*status).orange	= sensError & 0x01;
			(*status).gelb		= sensDoor  & 0x01;
			(*status).gruen		= (sensMouseDX >>1)  & 0x01;
			(*status).tuerkis	= (sensMouseDY >>1) & 0x01;
			(*status).weiss		= RC5_Code & 0x01;
		#endif
				
		LED_set(led);
	}
#endif

#ifdef PC
	/*!
	 * Zeigt Informationen zu den moeglichen Kommandozeilenargumenten an.
	 * Das Programm wird nach Anzeige des Hilfetextes per exit() beendet.
	 */
	void usage(void){
		puts("USAGE: ct-Bot [-t host] [-h] [-s]");
		puts("\t-t\tHostname oder IP Adresse zu der Verbunden werden soll");
		puts("\t-s\tServermodus");
		puts("\t-h\tZeigt diese Hilfe an");
		exit(1);
	}
#endif

#ifdef MCU
/*! 
 * Hauptprogramm des Bots. Diese Schleife kuemmert sich um seine Steuerung.
 */
	int main (void){

#endif

#ifdef PC

/*! 
 * Hauptprogramm des Bots. Diese Schleife kuemmert sich um seine Steuerung.
 */
 	int main (int argc, char *argv[]){

		int ch;	
		int start_server = 0;	/*!< Wird auf 1 gesetzt, falls -s angegeben wurde */
		char *hostname = NULL;	/*!< Speichert den per -t uebergebenen Hostnamen zwischen */

		// Die Kommandozeilenargumente komplett verarbeiten
		while ((ch = getopt(argc, argv, "hst:")) != -1) {
			switch (ch) {
			case 's':
				// Servermodus [-s] wird verlangt
				start_server = 1;
				break;
			case 't':
				// Hostname, auf dem ct-Sim laeuft wurde 
				// uebergeben. Der String wird in hostname
				// gesichert.
				{
					const int len = strlen(optarg);
					hostname = malloc(len + 1);
					if (NULL == hostname)
						exit(1);
					strcpy(hostname, optarg);
				}
				break;
			case 'h':
			default:
				// -h oder falscher Parameter, Usage anzeigen
				usage();
			}
		}
		argc -= optind;
		argv += optind;
		
	if (start_server != 0)    // Soll der TCP-Server gestartet werden?
    {
    	printf("ARGV[0]= %s\n",argv[1]);
       tcp_server_init();
       tcp_server_run();
    } else {
    	printf("c't-Bot\n");
        if (hostname)
            // Hostname wurde per Kommandozeile uebergeben
            tcp_hostname = hostname;
        else {
            // Der Zielhost wird per default durch das Macro IP definiert und
            // tcp_hostname mit einer Kopie des Strings initialisiert.
            tcp_hostname = malloc(strlen(IP) + 1);
            if (NULL == tcp_hostname)
                exit(1);
            strcpy(tcp_hostname, IP);
        }
    }
    
    
#endif
	#ifdef  TEST_AVAILABLE_MOTOR
		uint16 calls=0;	/*!< Im Testfall zaehle die Durchlaeufe */
	#endif

	#ifdef LOG_AVAILABLE
		printf("Logging is on (");
		#ifdef LOG_UART_AVAILABLE
				printf("UART");	
		#endif
	
		#ifdef LOG_CTSIM_AVAILABLE
				printf("CTSIM");	
		#endif
	
		#ifdef LOG_DISPLAY_AVAILABLE
				printf("DISPLAY");	
		#endif
		
		#ifdef LOG_STDOUT_AVAILABLE
				printf("STDOUT");	
		#endif
		printf(")\n");			
	#else
			printf("Logging is off!\n ");
	#endif	


	init();		

	
	#ifdef WELCOME_AVAILABLE
		display_cursor(1,1);
		display_printf("c't-Roboter");
		LED_set(0x00);
		#ifdef LOG_AVAILABLE
			LOG_DEBUG(("Hallo Welt!"));
		#endif	
	#endif
	
	#ifdef TEST_AVAILABLE_COUNTER
		display_screen=2;

	 	resets=eeprom_read_byte(&resetsEEPROM)+1;
	    eeprom_write_byte(&resetsEEPROM,resets);
	    /* Lege den Grund für jeden Reset im EEPROM ab */	
	    eeprom_write_byte(&resetInfoEEPROM+resets,reset_flag);
	#endif	
	/*! Hauptschleife des Bot */
	
	for(;;){
		#ifdef MCU
			bot_sens_isr();
		#endif
		#ifdef TEST_AVAILABLE
			show_sensors();
		#endif

		// Testprogramm, dass den Bot erst links, dann rechtsrum dreht
		#ifdef  TEST_AVAILABLE_MOTOR
			calls++;
			if (calls == 1)
				motor_set(BOT_SPEED_SLOW,-BOT_SPEED_SLOW);
			else if (calls == 501)
				motor_set(-BOT_SPEED_SLOW,BOT_SPEED_SLOW);
			else if (calls== 1001)
				motor_set(BOT_SPEED_STOP,BOT_SPEED_STOP);
			else
		#endif
		// hier drin steckt der Verhaltenscode
		#ifdef BEHAVIOUR_AVAILABLE
			if (sensors_initialized ==1 )
				bot_behave();
			else
				printf("sensors not initialized\n");
		#endif
			
		#ifdef MCU
			#ifdef BOT_2_PC_AVAILABLE
//				static int16 lastTimeCom =0;

				bot_2_pc_inform();				// Den PC ueber Sensorern und aktuatoren informieren
				bot_2_pc_listen();				// Kommandos vom PC empfangen
					
//				if (timer_get_s() != lastTimeCom) {	// sollte genau 1x pro Sekunde zutreffen
//					lastTimeCom = timer_get_s();		
					
//				}
			#endif
		#endif
		
		#ifdef LOG_AVAILABLE
			//LOG_DEBUG(("LOG TIME %d s", timer_get_s()));
		#endif	
		
		// Alles Anzeigen
		#ifdef DISPLAY_AVAILABLE
			display();
		#endif
		#ifdef PC
			wait_for_time(100000);
		#endif
		#ifdef MCU
//			delay(10);
		#endif
	}
	
	/*! Falls wir das je erreichen sollten ;-) */
	return 1;	
}
