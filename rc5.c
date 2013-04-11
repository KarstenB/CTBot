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

/*! @file 	rc5.c
 * @brief 	RC5-Fernbedienung
 * Um RC5-Codes fuer eine eigene Fernbedienung anzupassen, reicht es diese
 * in eine Header-Datei auszulagern und anstatt der rc5code.h einzubinden.
 * Die Maskierung fuer die Auswertung der Codes nicht vergessen!
 * @author 	Benjamin Benz (bbe@heise.de)
 * @date 	20.12.05
*/
#include "ct-Bot.h"
#include "ir-rc5.h"
#include "motor.h"
#include "motor-low.h"
#include "bot-logik.h"
#include "display.h"
#include "sensor.h"

#include "rc5-codes.h"

#include <stdlib.h>

#ifdef RC5_AVAILABLE

/*! 
 * Dieser Typ definiert Parameter fuer RC5-Kommando-Code Funktionen.
 */
typedef struct {
	uint16 value1;	/*!< Wert 1 */
	uint16 value2;	/*!< Wert 2 */
} RemCtrlFuncPar;

/*! Dieser Typ definiert die RC5-Kommando-Code Funktion. */
typedef void (*RemCtrlFunc)(RemCtrlFuncPar *par);

/*! Dieser Typ definiert den RC5-Kommando-Code und die auszufuehrende Funktion. */
typedef struct {
	uint16 command;		/*!< Kommando Code */
	RemCtrlFunc func;	/*!< Auszufuehrende Funktion */
	RemCtrlFuncPar par;	/*!< Parameter */
} RemCtrlAction;

/*!
 * Diese Funktion setzt das Display auf eine andere Ausgabe.
 * @param par Parameter mit dem zu setzenden Screen.
 */	
#ifdef DISPLAY_SCREENS_AVAILABLE
	static void rc5_screen_set(RemCtrlFuncPar *par);
#endif

#ifdef JOGDIAL
/*!
 * Diese Funktion setzt die Geschwindigkeit auf den angegebenen Wert.
 * @param par Parameter mit den zu setzenden Geschwindigkeiten.
 */	
static void rc5_bot_set_speed(RemCtrlFuncPar *par);
#endif

/*!
 * Diese Funktion aendert die Geschwindigkeit um den angegebenen Wert.
 * @param par Parameter mit den relativen Geschwindigkeitsaenderungen.
 */	
static void rc5_bot_change_speed(RemCtrlFuncPar *par);

/*!
 * Diese Funktion wechselt zwischen verschiednen Verhalten
 * @param par Parameter mit den zu setzenden Geschwindigkeiten.
 */	
void rc5_bot_next_behaviour(RemCtrlFuncPar *par);


/*!
 * Diese Funktion stellt die Not-Aus-Funktion dar. Sie laesst den Bot anhalten
 * und setzt alle Verhalten zurueck.
 * @param par notwendiger Dummy-Parameter.
 */	
static void rc5_emergency_stop(RemCtrlFuncPar *par);

/*!
* Verarbeitet die Zifferntasten in Abhaengigkeit vom eingestelltem Screen
* @param par Parameter mit der betaetigten Zahlentaste.
*/
void rc5_number(RemCtrlFuncPar *par);

#ifdef DISPLAY_BEHAVIOUR_AVAILABLE
	/*!
	 * Diese Funktion setzt die Aktivitaeten der Verhalten nach der Auswahl.
	 * Hierdurch erfolgt der Startschuss fuer Umschaltung der Verhalten
	 */	
	static void rc5_set_all_behaviours(void) ;
	  
	/*! 
	 * toggled ein Verhalten der Verhaltensliste an Position pos,
	 * die Aenderung erfolgt nur auf die Puffervariable  
	 * @param i Verhaltens-Listenposition, entspricht der Taste 1-6 der gewaehlten Verhaltensseite
	 */	 
	static void rc5_toggle_behaviour_new(int8 i);
#endif

/*! Steuert den Servo an
 * @param par Parameter mit Servo-Nummer und -Position
 */
void rc5_bot_servo(RemCtrlFuncPar *par);

uint16 RC5_Code;	/*!< Letzter empfangener RC5-Code */

/*! Fernbedienungsaktionen */
static RemCtrlAction gRemCtrlAction[] = {
	/* RC5-Code,		Funktion,				Parameter */
	{ RC5_CODE_PWR,		rc5_emergency_stop,		{ 0, 0 } },
	{ RC5_CODE_UP,		rc5_bot_change_speed,	{ 10, 10 } },
	{ RC5_CODE_DOWN,	rc5_bot_change_speed,	{ -10, -10 } },
	{ RC5_CODE_LEFT,	rc5_bot_change_speed,	{ 0, 10 } },
	{ RC5_CODE_RIGHT,	rc5_bot_change_speed,	{ 10, 0 } },
	{ RC5_CODE_0,		rc5_number,		        { 0, 0 } },
	{ RC5_CODE_1,		rc5_number,		        { 1, 1 } },
	{ RC5_CODE_2,		rc5_number,		        { 2, 2 } },
	{ RC5_CODE_3,		rc5_number,			    { 3, 3 } },
	{ RC5_CODE_4,		rc5_number,			    { 4, 4 } },
	{ RC5_CODE_5,		rc5_number,			    { 5, 5 } },
	{ RC5_CODE_6,		rc5_number,			    { 6, 6 } },
	{ RC5_CODE_7,		rc5_number,			    { 7, 7 } },
	{ RC5_CODE_8,		rc5_number,			    { 8, 8 } },
	{ RC5_CODE_9,		rc5_number,			    { 9, 9 } },
	{ RC5_CODE_I_II,		rc5_bot_next_behaviour,	{ 0, 0 } },
	{ RC5_CODE_BWD,		rc5_bot_servo,			{ SERVO1, SERVO_LEFT } },
	{ RC5_CODE_FWD,		rc5_bot_servo,			{ SERVO1, SERVO_RIGHT } },


	
#ifdef DISPLAY_SCREENS_AVAILABLE
	{ RC5_CODE_RED,		rc5_screen_set,			{ 0, 0 } },
	{ RC5_CODE_GREEN,	rc5_screen_set,			{ 1, 0 } },
	{ RC5_CODE_YELLOW,	rc5_screen_set,			{ 2, 0 } },
	{ RC5_CODE_BLUE,		rc5_screen_set,			{ 3, 0 } },
	{ RC5_CODE_TV_VCR,	rc5_screen_set,			{ DISPLAY_SCREEN_TOGGLE, 0 } },	
#endif
#ifdef JOGDIAL
	{ RC5_CODE_JOG_MID,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_L1,	rc5_bot_set_speed,		{ BOT_SPEED_FAST, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_L2,	rc5_bot_set_speed,		{ BOT_SPEED_NORMAL, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_L3,	rc5_bot_set_speed,		{ BOT_SPEED_SLOW, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_L4,	rc5_bot_set_speed,		{ BOT_SPEED_STOP, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_L5,	rc5_bot_set_speed,		{ -BOT_SPEED_NORMAL, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_L6,	rc5_bot_set_speed,		{ -BOT_SPEED_FAST, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_L7,	rc5_bot_set_speed,		{ -BOT_SPEED_MAX, BOT_SPEED_MAX } },
	{ RC5_CODE_JOG_R1,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, BOT_SPEED_FAST } },
	{ RC5_CODE_JOG_R2,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, BOT_SPEED_NORMAL } },
	{ RC5_CODE_JOG_R3,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, BOT_SPEED_SLOW } },
	{ RC5_CODE_JOG_R4,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, BOT_SPEED_STOP } },
	{ RC5_CODE_JOG_R5,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, -BOT_SPEED_NORMAL } },
	{ RC5_CODE_JOG_R6,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, -BOT_SPEED_FAST } },
	{ RC5_CODE_JOG_R7,	rc5_bot_set_speed,		{ BOT_SPEED_MAX, -BOT_SPEED_MAX } }
#endif	/* JOGDIAL */
};

/*!
 * Diese Funktion setzt das Display auf eine andere Ausgabe.
 * Fuer die Verhaltensausgabe werden hier die Verhalten durchgeblaettert
 * @param par Parameter mit dem zu setzenden Screen.
 */	
#ifdef DISPLAY_SCREENS_AVAILABLE
static void rc5_screen_set(RemCtrlFuncPar *par) {
	if (par) {
		if (par->value1 == DISPLAY_SCREEN_TOGGLE) {
				
		  #ifdef DISPLAY_BEHAVIOUR_AVAILABLE
		     /* erst nachsehen, ob noch weitere Verhalten auf anderen Pages vorhanden sind */
		     /* nur neuer Screen, wenn alle Verhalten angezeigt wurden */
		     if ((display_screen == 2) && (another_behaviour_page())
		     ) 
		           behaviour_page++;
		     else {
		     	display_screen ++;
		     	 
			       if (display_screen == 1) {
			         behaviour_page = 1;
			          set_behaviours_equal();
			       }
			   
		         }
	    
	    	#else
		      display_screen ++;
		    #endif     
	      
	    
		}
		else 
		{
		  /* Screen direkt waehlen */	 
		  #ifdef DISPLAY_BEHAVIOUR_AVAILABLE
			
			  /* Screen direkt waehlen und Verhaltens-Puffervariablen abgleichen*/
			  display_screen = par->value1;
		       if ((display_screen == 2)&& (behaviour_page == 1)) {
			      
			       set_behaviours_equal();
			     }  
			  behaviour_page = 1;
			  
		   #else
		     /* Screen direkt waehlen */
			  display_screen = par->value1;
		   #endif
		}

		
		display_screen %= DISPLAY_SCREENS;
		display_clear();
		
	}
}
#endif

/*! Steuert den Servo an
 * @param par Parameter mit Servo-Nummer und -Position
 */
void rc5_bot_servo(RemCtrlFuncPar *par){
		servo_set(par->value1,par->value2);
}
/*!
 * Diese Funktion wechselt zwiaschen verschiednen Verhalten
 * @param par Parameter mit den zu setzenden Geschwindigkeiten.
 */	
void rc5_bot_next_behaviour(RemCtrlFuncPar *par) {

	static uint8 state =0;

	state++;
	
	switch (state) {
		case 0: 
			break;
		case 1: activateBehaviour(bot_drive_square_behaviour);
			break;
		case 2: deactivateBehaviour(bot_drive_square_behaviour);
				 deactivateBehaviour(bot_goto_behaviour); 
				 
				 activateBehaviour(bot_olympic_behaviour);
			break;
		default:
			deactivateBehaviour(bot_olympic_behaviour);
			deactivateBehaviour( bot_drive_distance_behaviour);
			deactivateBehaviour( bot_turn_behaviour);
			deactivateBehaviour( bot_explore_behaviour);
			deactivateBehaviour( bot_do_slalom_behaviour);
		
			state=0;
	}
}

#ifdef JOGDIAL
/*!
 * Diese Funktion setzt die Geschwindigkeit auf den angegebenen Wert.
 * @param par Parameter mit den zu setzenden Geschwindigkeiten.
 */	
static void rc5_bot_set_speed(RemCtrlFuncPar *par) {
	if (par) {
		target_speed_l = par->value1;
		target_speed_r = par->value2;
	}
}
#endif

/*!
 * Diese Funktion stellt die Not-Aus-Funktion dar. Sie laesst den Bot anhalten
 * und setzt alle Verhalten zurueck mit Sicherung der vorherigen Aktivitaeten.
 * @param par notwendiger Dummy-Parameter.
 */	
 static void rc5_emergency_stop(RemCtrlFuncPar *par) {
			
            // Setzen der Geschwindigkeit auf 0	
			target_speed_l = 0 ;
		    target_speed_r = 0 ;
		    
		    // Alle Verhalten deaktivieren
		    deactivateAllBehaviours();  // alle Verhalten deaktivieren mit vorheriger Sicherung
		     #ifdef DISPLAY_BEHAVIOUR_AVAILABLE
		       display_clear();         // Screen zuerst loeschen
		       display_screen = 2;      // nach Notstop in den Verhaltensscreen mit Anzeige der alten Verhalten
		     #endif
}
 
/*!
 * Diese Funktion aendert die Geschwindigkeit um den angegebenen Wert.
 * @param par Parameter mit den relativen Geschwindigkeitsaenderungen.
 */	
static void rc5_bot_change_speed(RemCtrlFuncPar *par) {
	int old;
	if (par) {
		old=target_speed_l;
		target_speed_l += par->value1;
		if ((target_speed_l < -BOT_SPEED_MAX)|| (target_speed_l > BOT_SPEED_MAX))
			target_speed_l = old;
		
		old=target_speed_r;		
		target_speed_r += par->value2;
		if ((target_speed_r <-BOT_SPEED_MAX)||(target_speed_r > BOT_SPEED_MAX))
			target_speed_r = old;
	}
}

/*!
 * Liest ein RC5-Codeword und wertet es aus
 */
void rc5_control(void){
	uint16 run;
	static uint16 RC5_Last_Toggle = 0;   /*!< Toggle-Wert des zuletzt empfangenen RC5-Codes*/
	 
	uint16 rc5 = ir_read();
	
	if (rc5 != 0) {
		RC5_Code= rc5 & RC5_MASK;	/* Alle uninteressanten Bits ausblenden */
		/* Toggle kommt nicht im Simulator, immer gewechseltes Toggle Bit sicherstellen */ 
		#ifdef PC
		  RC5_Last_Toggle = !(rc5 & RC5_TOGGLE);
		#endif
		/* Bei Aenderung des Toggle Bits, entspricht neuem Tastendruck, gehts nur weiter */
		if ((rc5 & RC5_TOGGLE)   != RC5_Last_Toggle) {	/* Nur Toggle Bit abfragen, bei Ungleichheit weiter */
		  RC5_Last_Toggle = rc5 & RC5_TOGGLE;           /* Toggle Bit neu belegen */
		
		   /* Suchen der auszufuehrenden Funktion */
		   for(run=0; run<sizeof(gRemCtrlAction)/sizeof(RemCtrlAction); run++) {
			   /* Funktion gefunden? */
			   if (gRemCtrlAction[run].command == RC5_Code) {
				   /* Funktion ausfuehren */
				   gRemCtrlAction[run].func(&gRemCtrlAction[run].par);
			   }
		   }
		}
	}
}

#ifdef DISPLAY_BEHAVIOUR_AVAILABLE
/*! 
 * toggled ein Verhalten der Verhaltensliste an Position pos,
 * die Aenderung erfolgt nur auf die Puffervariable  
 * @param i Verhaltens-Listenposition, entspricht der Taste 1-6 der gewaehlten Verhaltensseite
 */	
  void rc5_toggle_behaviour_new(int8 i) {
	
		  toggleNewBehaviourPos(i);	
  } 
  
/*!
 * Diese Funktion setzt die Aktivitaeten der Verhalten nach der Auswahl.
 * Hierdurch erfolgt der Startschuss fuer Umschaltung der Verhalten
 */	  static void rc5_set_all_behaviours(void) {
	
		  set_behaviours_active_to_new();
	
  }
#endif  

/*!
 * Verarbeitet die Zifferntasten in Abhaengigkeit vom eingestelltem Screen
 * @param par Parameter mit der betaetigten Zahlentaste.
 */
void rc5_number(RemCtrlFuncPar *par) {
	#ifdef DISPLAY_SCREENS_AVAILABLE 
	switch (display_screen) {
		case 0:
	#endif
			switch (par->value1) {
				case 0:	
				target_speed_l=0;target_speed_r=0;break;
				case 1: target_speed_l = BOT_SPEED_SLOW; target_speed_r = BOT_SPEED_SLOW; break;
				case 2: bot_goto(100, 100, 0); break;
				case 3: target_speed_l = BOT_SPEED_NORMAL; target_speed_r = BOT_SPEED_NORMAL; break;
				case 4: bot_turn(0, 90); break;
				//case 5: bot_goto(0, 0, 0); break;
				case 5: bot_collect_obstacles(0); break;
				//case 5: bot_solve_maze(0); break;
				case 6: bot_turn(0, -90); break;
				case 7: bot_turn(0,180); break;
				case 8: sensDoor=!sensDoor; break;
				case 9: bot_turn(0, -180); break;
			}
	#ifdef DISPLAY_SCREENS_AVAILABLE 
	
			break;
		
	#ifdef DISPLAY_BEHAVIOUR_AVAILABLE
		case 2:
			switch (par->value1) {
				case 0:	break;
				case 1: rc5_toggle_behaviour_new(1); break;
				case 2: rc5_toggle_behaviour_new(2); break;
				case 3: rc5_toggle_behaviour_new(3); break;
				case 4: rc5_toggle_behaviour_new(4); break;
				case 5: rc5_toggle_behaviour_new(5); break;
				case 6: rc5_toggle_behaviour_new(6); break;
				case 7: break;
				case 8: break;
				case 9: rc5_set_all_behaviours(); break;
			}
			break;
	#endif
 	}
 	#endif
 }	
#endif
