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

/*! @file 	rc5-codes.h
 * @brief 	RC5-Fernbedienungscodes
 * @author 	Andreas Merkle (mail@blue-andi.de)
 * @date 	15.02.06
 * Wer diese Datei angepasst hat, kann Sie durch einen .cvsignore Eintrag schützen. 
 * Dann Ueberschreibt Eclipse Sie nicht mehr automatisch
*/

#ifndef RC5CODE_H_
#define RC5CODE_H_

#define RC5_TOGGLE		0x0800		/*!< Das RC5-Toggle-Bit */
#define RC5_ADDRESS	0x07C0		/*!< Der Adressbereich */
#define RC5_COMMAND	0x103F		/*!< Der Kommandobereich */

/* Im Normalbetrieb hilft die Beschränkung der RC5_MASK auf RC5_COMMAND dem Bot, 
 * möglichst viele FBs zu erkennen. 
 * Zum erfassen neuer  Codes sollte man unbedingt RC5_MASK auf (RC5_COMMAND|RC5_ADDRESS) setzen */

// Normalbetrieb
#define RC5_MASK (RC5_COMMAND)					/*!< Welcher Teil des Kommandos wird ausgewertet? */
// Erfassen neuer FB-Codes
//#define RC5_MASK (RC5_COMMAND|RC5_ADDRESS)	/*!< Welcher Teil des Kommandos wird ausgewertet? */



/*!
 * Definition RC5-Codes verschiedener Fernbedienungen. Wer eine neue FB
 * einfuegen will, sollte
 * 
 * 	- 	eine Definition per cut&paste duplizieren,
 * 	- 	eine geeignete Konstante RC_HAVE_{Herstellername}_{Fernbedienungsname}
 * 		waehlen,
 * 	-	diese Konstante in die obere Liste der FB-Selektion eintragen,
 * 	-	die alte FB-Konstante im kopierten Bereich ersetzen,
 * 	-	festlegen, ob die FB ein Jog-Dial hat, ob der RC5_NOT_AVAIL-Code
 * 		wirklich nie von der FB generiert werden kann (0xFFFF ist meisst ok)
 * 	-	die eigentlichen, herausgefunden Codes eintragen.
 * 
 * Ist das erledigt, und funktioniert die neue FB-Definition, kann diese
 * in der c't-bot-Entwicklerliste vorgestellt werden. (mb/18.03.2006)
 */

/* Nur auf dem realen Bot gibt es verschiedene FBs */
#ifdef MCU

	// Dies ist die Standard-Fernbedienung unter anderem fuer den Sim
	#define RC_HAVE_HQ_RC_UNIVERS29_334
	
	//#define RC_HAVE_HAUPPAUGE_WINTV
	//#define RC_HAVE_HAUPPAUGE_MediaMPV
	//#define RC_HAVE_CONRAD_PROMO8
	//#define RC_HAVE_VIVANCO_UR89
	//#define RC_HAVE_VIVANCO_UR89_TV_CODE_089
	//#define RC_HAVE_Technisat_TTS35AI

 #else
	// Dies ist die Standard-Fernbedienung unter anderem fuer den Sim
	//#define RC_HAVE_HQ_RC_UNIVERS29_334
	#define RC_HAVE_DEFAULT
#endif



// Code um zu kennzeichnen, dass eine Taste nicht vorhanden ist
#define RC5_NOT_AVAIL	(0xFFFF)				/*!< Code fuer Taste nicht vorhanden */



/*!
 * Default-Fernbedienung 
 * HQ RC Univers 29, Geraetecode 334
 */

#ifdef RC_HAVE_HQ_RC_UNIVERS29_334
	#define RC5_CODE_PWR		(0x118C & RC5_MASK)		/*!< Taste An/Aus */
	
	#define RC5_CODE_0			(0x1180 & RC5_MASK)		/*!< Taste 0 /10*/
	#define RC5_CODE_1			(0x1181 & RC5_MASK)		/*!< Taste 1 */
	#define RC5_CODE_2			(0x1182 & RC5_MASK)		/*!< Taste 2 */
	#define RC5_CODE_3			(0x1183 & RC5_MASK)		/*!< Taste 3 */
	#define RC5_CODE_4			(0x1184 & RC5_MASK)		/*!< Taste 4 */
	#define RC5_CODE_5			(0x1185 & RC5_MASK)		/*!< Taste 5 */
	#define RC5_CODE_6			(0x1186 & RC5_MASK)		/*!< Taste 6 */
	#define RC5_CODE_7			(0x1187 & RC5_MASK)		/*!< Taste 7 */
	#define RC5_CODE_8			(0x1188 & RC5_MASK)		/*!< Taste 8 */
	#define RC5_CODE_9			(0x1189 & RC5_MASK)		/*!< Taste 9 */
	#define RC5_CODE_11		(0x118A & RC5_MASK)		/*!< Taste 11 /1-*/
	#define RC5_CODE_12		(0x11A3 & RC5_MASK)		/*!< Taste 12 /2-*/
		
	#define RC5_CODE_GREEN		(0x01BA & RC5_MASK)		/*!< Gruene Taste */
	#define RC5_CODE_RED		(0x01BD & RC5_MASK)		/*!< Rote Taste */
	#define RC5_CODE_YELLOW	(0x01B1 & RC5_MASK)		/*!< Gelbe Taste */
	#define RC5_CODE_BLUE		(0x01B0 & RC5_MASK)		/*!< Blaue Taste */
	
	#define RC5_CODE_I_II		(0x11AB & RC5_MASK)		/*!< I/II-Taste */
	#define RC5_CODE_TV_VCR	(0x11B8 & RC5_MASK)		/*!< TV/VCR-Taste */

	#define RC5_CODE_DOT		(0x11AB & RC5_MASK)		/*!< Taste mit rundem Punkt */
	
	#define RC5_CODE_PLAY		(0x11B5 & RC5_MASK)		/*!< PLAY-Taste */
	#define RC5_CODE_STILL		(0x11A9 & RC5_MASK)		/*!< Pause Taste */
	#define RC5_CODE_STOP		(0x11B6 & RC5_MASK)		/*!< Pause Taste */
	#define RC5_CODE_BWD		(0x11B2 & RC5_MASK)		/*!< Backward Taste */
	#define RC5_CODE_FWD		(0x11B4 & RC5_MASK)		/*!< Forward Taste */

	#define RC5_CODE_CH_PC		(0x11BF & RC5_MASK)		/*!< CH*P/C Taste */
	#define RC5_CODE_MUTE		(0x01BF & RC5_MASK)		/*!< Mute-Taste */

	#define RC5_VOL_PLUS		(0x1190 & RC5_MASK)		/*!< Vol + Taste */
	#define RC5_VOL_MINUS		(0x1191 & RC5_MASK)		/*!< Vol - Taste */

	#define RC5_CH_PLUS		(0x11A0 & RC5_MASK)		/*!< Ch + Taste */
	#define RC5_CH_MINUS		(0x11A1 & RC5_MASK)		/*!< Ch - Taste */
	
	
	#define RC5_CODE_UP		RC5_CODE_STILL			/*!< Taste Hoch */
	#define RC5_CODE_DOWN		RC5_CODE_STOP			/*!< Taste Runter */
	#define RC5_CODE_LEFT		RC5_CODE_BWD				/*!< Taste Links */
	#define RC5_CODE_RIGHT		RC5_CODE_FWD				/*!< Taste Rechts */
#endif

/*!
 * Default-Fernbedienung 
 * HQ RC Univers 29, Geraetecode 335
 */

#ifdef RC_HAVE_HQ_RC_UNIVERS29_335
	#define RC5_CODE_PWR		(0x100C & RC5_MASK)		/*!< Taste An/Aus */
	
	#define RC5_CODE_0			(0x1000 & RC5_MASK)		/*!< Taste 0 /10*/
	#define RC5_CODE_1			(0x1001 & RC5_MASK)		/*!< Taste 1 */
	#define RC5_CODE_2			(0x1002 & RC5_MASK)		/*!< Taste 2 */
	#define RC5_CODE_3			(0x1003 & RC5_MASK)		/*!< Taste 3 */
	#define RC5_CODE_4			(0x1004 & RC5_MASK)		/*!< Taste 4 */
	#define RC5_CODE_5			(0x1005 & RC5_MASK)		/*!< Taste 5 */
	#define RC5_CODE_6			(0x1006 & RC5_MASK)		/*!< Taste 6 */
	#define RC5_CODE_7			(0x1007 & RC5_MASK)		/*!< Taste 7 */
	#define RC5_CODE_8			(0x1008 & RC5_MASK)		/*!< Taste 8 */
	#define RC5_CODE_9			(0x1009 & RC5_MASK)		/*!< Taste 9 */
	#define RC5_CODE_11		(0x100A & RC5_MASK)		/*!< Taste 11 /1-*/
	#define RC5_CODE_12		(0x1003 & RC5_MASK)		/*!< Taste 12 /2-*/
		
	#define RC5_CODE_GREEN		(0x101E & RC5_MASK)		/*!< Gruene Taste */
	#define RC5_CODE_RED		(0x101D & RC5_MASK)		/*!< Rote Taste */
	#define RC5_CODE_YELLOW	(0x1027 & RC5_MASK)		/*!< Gelbe Taste */
	#define RC5_CODE_BLUE		(0x101C & RC5_MASK)		/*!< Blaue Taste */
	
	#define RC5_CODE_I_II		(0x1023 & RC5_MASK)		/*!< I/II-Taste */
	#define RC5_CODE_TV_VCR	(0x1038 & RC5_MASK)		/*!< TV/VCR-Taste */

	#define RC5_CODE_DOT		(0x1037 & RC5_MASK)		/*!< Taste mit rundem Punkt */
	
	#define RC5_CODE_PLAY		(0x11B5 & RC5_MASK)		/*!< PLAY-Taste */
	#define RC5_CODE_STILL		(0x1029 & RC5_MASK)		/*!< Pause Taste */
	#define RC5_CODE_STOP		(0x1036 & RC5_MASK)		/*!< Pause Taste */
	#define RC5_CODE_BWD		(0x1032 & RC5_MASK)		/*!< Backward Taste */
	#define RC5_CODE_FWD		(0x1034 & RC5_MASK)		/*!< Forward Taste */

	#define RC5_CODE_CH_PC		(0x100B & RC5_MASK)		/*!< CH*P/C Taste */
	#define RC5_CODE_MUTE		(0x003F & RC5_MASK)		/*!< Mute-Taste */

	#define RC5_VOL_PLUS		(0x1010 & RC5_MASK)		/*!< Vol + Taste */
	#define RC5_VOL_MINUS		(0x1011 & RC5_MASK)		/*!< Vol - Taste */

	#define RC5_CH_PLUS		(0x1020 & RC5_MASK)		/*!< Ch + Taste */
	#define RC5_CH_MINUS		(0x1021 & RC5_MASK)		/*!< Ch - Taste */
	
	
	#define RC5_CODE_UP		RC5_CODE_STILL			/*!< Taste Hoch */
	#define RC5_CODE_DOWN		RC5_CODE_STOP			/*!< Taste Runter */
	#define RC5_CODE_LEFT		RC5_CODE_BWD				/*!< Taste Links */
	#define RC5_CODE_RIGHT		RC5_CODE_FWD				/*!< Taste Rechts */
#endif

/*!
 * Fernbedienung Hauppauge (simple WinTV-Karten Fernbedienung)
 */

#ifdef RC_HAVE_HAUPPAUGE_WINTV
	#define RC_HAVE_CODES							/*!< Definiert Codes */
	#undef JOG_DIAL									/*!< Hat keinen Jog Dial */

	#define RC5_CODE_0			(0x1000 & RC5_MASK)		/*!< Taste 0 */
	#define RC5_CODE_1			(0x1001 & RC5_MASK)		/*!< Taste 1 */
	#define RC5_CODE_2			(0x1002 & RC5_MASK)		/*!< Taste 2 */
	#define RC5_CODE_3			(0x1003 & RC5_MASK)		/*!< Taste 3 */
	#define RC5_CODE_4			(0x1004 & RC5_MASK)		/*!< Taste 4 */
	#define RC5_CODE_5			(0x1005 & RC5_MASK)		/*!< Taste 5 */
	#define RC5_CODE_6			(0x1006 & RC5_MASK)		/*!< Taste 6 */
	#define RC5_CODE_7			(0x1007 & RC5_MASK)		/*!< Taste 7 */
	#define RC5_CODE_8			(0x1008 & RC5_MASK)		/*!< Taste 8 */
	#define RC5_CODE_9			(0x1009 & RC5_MASK)		/*!< Taste 9 */

	#define RC5_CODE_PWR		(0x1026 & RC5_MASK)		/*!< Taste Minimize */
	#define RC5_CODE_FULL		(0x102E & RC5_MASK)		/*!< Taste Full Screen */
	#define RC5_CODE_SOURCE	(0x1022 & RC5_MASK)		/*!< Taste Source */

	#define RC5_CODE_UP		(0x1020 & RC5_MASK)		/*!< Taste CH + */
	#define RC5_CODE_DOWN		(0x1021 & RC5_MASK)		/*!< Taste CH - */
	#define RC5_CODE_LEFT		(0x1011 & RC5_MASK)		/*!< Taste VOL- */
	#define RC5_CODE_RIGHT		(0x1010 & RC5_MASK)		/*!< Taste VOL+ */

	#define RC5_CODE_FWD		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_BWD		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_RED		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_GREEN		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_YELLOW	RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_BLUE		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */

	#define RC5_CODE_I_II		RC5_CODE_SOURCE
	#define RC5_CODE_TV_VCR	RC5_CODE_FULL
#endif /* RC_HAVE_HAUPPAUGE_WINTV */

#ifdef RC_HAVE_HAUPPAUGE_MediaMPV
	#define RC_HAVE_CODES							/*!< Definiert Codes */
	#undef JOG_DIAL									/*!< Hat keinen Jog Dial */

	#define RC5_CODE_0			(0x17C0 & RC5_MASK)		/*!< Taste 0 */
	#define RC5_CODE_1			(0x17C1 & RC5_MASK)		/*!< Taste 1 */
	#define RC5_CODE_2			(0x17C2 & RC5_MASK)		/*!< Taste 2 */
	#define RC5_CODE_3			(0x17C3 & RC5_MASK)		/*!< Taste 3 */
	#define RC5_CODE_4			(0x17C4 & RC5_MASK)		/*!< Taste 4 */
	#define RC5_CODE_5			(0x17C5 & RC5_MASK)		/*!< Taste 5 */
	#define RC5_CODE_6			(0x17C6 & RC5_MASK)		/*!< Taste 6 */
	#define RC5_CODE_7			(0x17C7 & RC5_MASK)		/*!< Taste 7 */
	#define RC5_CODE_8			(0x17C8 & RC5_MASK)		/*!< Taste 8 */
	#define RC5_CODE_9			(0x17C9 & RC5_MASK)		/*!< Taste 9 */

	#define RC5_CODE_SELECT	RC5_NOT_AVAIL			/*!< Taste Source */

	#define RC5_CODE_UP		(0x17E0 & RC5_MASK)		/*!< Taste CH + */
	#define RC5_CODE_DOWN		(0x17E1 & RC5_MASK)		/*!< Taste CH - */
	#define RC5_CODE_LEFT		(0x17D1 & RC5_MASK)		/*!< Taste VOL- */
	#define RC5_CODE_RIGHT		(0x17D0 & RC5_MASK)		/*!< Taste VOL+ */

	#define RC5_CODE_OK		(0x17E5 & RC5_MASK)		/*!< Taste OK */
	
	#define RC5_CODE_PWR   	(0x17FD & RC5_MASK)		/*!< Taste An/Aus */ 
	
	#define RC5_CODE_RED   	(0x17CB & RC5_MASK)		/*!< Taste Rot */
	#define RC5_CODE_GREEN		(0x17EE & RC5_MASK)		/*!< Taste Gruen */
	#define RC5_CODE_YELLOW	(0x17F8 & RC5_MASK)		/*!< Taste Gelb */
	#define RC5_CODE_BLUE		(0x17E9 & RC5_MASK)		/*!< Taste Blau */
	
	#define RC5_CODE_FWD		(0x17F4 & RC5_MASK)		/*!< Taste >> */
	#define RC5_CODE_BWD		(0x17F2 & RC5_MASK)		/*!< Taste << */
	#define RC5_CODE_PLAY		(0x17F5 & RC5_MASK)		/*!< Taste > */
	#define RC5_CODE_RECORD	(0x17F7 & RC5_MASK)		/*!< Taste Aufnahme */
	#define RC5_CODE_STOP		(0x17F6 & RC5_MASK)		/*!< Taste Stop */
	#define RC5_CODE_WAIT		(0x17F0 & RC5_MASK)		/*!< Taste Pause */
	#define RC5_CODE_REPLAY	(0x17E4 & RC5_MASK)		/*!< Taste Anfang |< */
	#define RC5_CODE_SKIP		(0x17DE & RC5_MASK)		/*!< Taste Ende >| */
	
	#define RC5_CODE_MUTE		(0x17CF & RC5_MASK)		/*!< Taste Mute */
	#define RC5_CODE_VIEW		(0x17CC & RC5_MASK)		/*!< Taste View zwischen Mute und Full */
	#define RC5_CODE_FULL		(0x17FC & RC5_MASK)		/*!< Taste Full */
	
	#define RC5_CODE_BACK		(0x17DF & RC5_MASK)		/*!< Taste Back/Exit */
	#define RC5_CODE_MENU		(0x17CD & RC5_MASK)		/*!< Taste Menue */
	#define RC5_CODE_GO		(0x17FB & RC5_MASK)		/*!< Taste GO */

	#define RC5_CODE_I_II		RC5_CODE_SELECT
	#define RC5_CODE_TV_VCR	RC5_CODE_VIEW

#endif /* RC_HAVE_HAUPPAUGE_MediaMPV */

/*!
 * Fernbedienung Conrad Promo 8
 */

#ifdef RC_HAVE_CONRAD_PROMO8
	#define RC_HAVE_CODES							/*!< Definiert Codes */
	#undef JOG_DIAL									/*!< Hat keinen Jog Dial */

	#define RC5_CODE_0			(0x3000 & RC5_MASK)		/*!< Taste 0 */
	#define RC5_CODE_1			(0x3001 & RC5_MASK)		/*!< Taste 1 */
	#define RC5_CODE_2			(0x3002 & RC5_MASK)		/*!< Taste 2 */
	#define RC5_CODE_3			(0x3003 & RC5_MASK)		/*!< Taste 3 */
	#define RC5_CODE_4			(0x3004 & RC5_MASK)		/*!< Taste 4 */
	#define RC5_CODE_5			(0x3005 & RC5_MASK)		/*!< Taste 5 */
	#define RC5_CODE_6			(0x3006 & RC5_MASK)		/*!< Taste 6 */
	#define RC5_CODE_7			(0x3007 & RC5_MASK)		/*!< Taste 7 */
	#define RC5_CODE_8			(0x3008 & RC5_MASK)		/*!< Taste 8 */
	#define RC5_CODE_9			(0x3009 & RC5_MASK)		/*!< Taste 9 */

	#define RC5_CODE_UP		(0x2010 & RC5_MASK)		/*!< Taste Hoch */
	#define RC5_CODE_DOWN		(0x2011 & RC5_MASK)		/*!< Taste Runter */
	#define RC5_CODE_LEFT		(0x2015 & RC5_MASK)		/*!< Taste Links */
	#define RC5_CODE_RIGHT		(0x2016 & RC5_MASK)		/*!< Taste Rechts */

	#define RC5_CODE_ENTER 	(0x2017 & RC5_MASK)  	/*!< Enter-Taste*/
	#define RC5_CODE_PWR   	(0x100C & RC5_MASK)     /*!< Enter Taste als Ersatz fuer Taste An/Aus */

	#define RC5_CODE_RED		(0x202B & RC5_MASK)		/*!< Rote Taste */
	#define RC5_CODE_GREEN		(0x202C & RC5_MASK)		/*!< Gruene Taste */
	#define RC5_CODE_YELLOW	(0x202D & RC5_MASK)		/*!< Gelbe Taste */
	#define RC5_CODE_BLUE		(0x202E & RC5_MASK)		/*!< Blaue Taste */
	#define RC5_CODE_VIEW		(0x2012 & RC5_MASK)		/*!< Instant View Taste */


	#define RC5_CODE_SELECT	(0x300D & RC5_MASK)		/*!< Select Taste */
	#define RC5_CODE_BWD		(0x3011 & RC5_MASK)		/*!< Backward Taste */
	#define RC5_CODE_FWD		(0x3010 & RC5_MASK)		/*!< Forward Taste */

	#define RC5_CODE_I_II		RC5_CODE_SELECT
	#define RC5_CODE_TV_VCR	RC5_CODE_VIEW
#endif

/*!
 * Fernbedienung VIVANCO UR89, vor Verwendung auf VCR druecken
 * @author Andreas Staudenmayer
 */
#ifdef RC_HAVE_VIVANCO_UR89
	#define RC_HAVE_CODES							/*!< Definiert Codes */
	#undef JOG_DIAL									/*!< Hat keinen Jog Dial */

	#define RC5_CODE_0			(0x1140 & RC5_MASK)		/*!< Taste 0 			   */
	#define RC5_CODE_1			(0x1141 & RC5_MASK)		/*!< Taste 1 			   */
	#define RC5_CODE_2			(0x1142 & RC5_MASK)		/*!< Taste 2 			   */
	#define RC5_CODE_3			(0x1143 & RC5_MASK)		/*!< Taste 3 			   */
	#define RC5_CODE_4			(0x1144 & RC5_MASK)		/*!< Taste 4 			   */
	#define RC5_CODE_5			(0x1145 & RC5_MASK)		/*!< Taste 5 			   */
	#define RC5_CODE_6			(0x1146 & RC5_MASK)		/*!< Taste 6 			   */
	#define RC5_CODE_7			(0x1147 & RC5_MASK)		/*!< Taste 7 			   */
	#define RC5_CODE_8			(0x1148 & RC5_MASK)		/*!< Taste 8 			   */
	#define RC5_CODE_9			(0x1149 & RC5_MASK)		/*!< Taste 9 			   */

	#define RC5_CODE_PWR		(0x114C & RC5_MASK)		/*!< Taste An, Aus         */
	#define RC5_CODE_VIEW		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_SELECT	RC5_NOT_AVAIL		/*!< Taste nicht vorhanden */

	#define RC5_CODE_UP		(0x1160 & RC5_MASK)		/*!< Taste Hoch            */
	#define RC5_CODE_DOWN		(0x1161 & RC5_MASK)		/*!< Taste Runter          */
	#define RC5_CODE_LEFT		(0x0171 & RC5_MASK)		/*!< Taste Links           */
	#define RC5_CODE_RIGHT		(0x0170 & RC5_MASK)		/*!< Taste Rechts          */

	#define RC5_CODE_FWD		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_BWD		RC5_NOT_AVAIL			/*!< Taste nicht vorhanden */
	#define RC5_CODE_RED		(0x1172 & RC5_MASK)		/*!< rote Taste            */
	#define RC5_CODE_GREEN		(0x1176 & RC5_MASK)		/*!< gruene Taste          */
	#define RC5_CODE_YELLOW	(0x1175 & RC5_MASK)		/*!< gelbe Taste           */
	#define RC5_CODE_BLUE		(0x1174 & RC5_MASK)		/*!< blaue Taste           */

	#define RC5_CODE_I_II		RC5_CODE_SELECT
	#define RC5_CODE_TV_VCR	RC5_CODE_VIEW
#endif /* RC_HAVE_VIVANCO_UR89 */


/*!
 * Fernbedienung VIVANCO UR89, TV Modus (Alle Tasten funktionieren)
 * vor Verwendung fuer TV den Code 089 auf der Fernbedienung programmieren
 * @author Ulrich Scheffler
 */
#ifdef RC_HAVE_VIVANCO_UR89_TV_CODE_089
	#define RC_HAVE_CODES							/*!< Definiert Codes */
	#undef JOG_DIAL									/*!< Hat keinen Jog Dial */

	#define RC5_NOT_AVAIL		(0xFFFF)			/*!< Code fuer Taste nicht vorhanden */
	/* Jede Taste bekommt erstmal die Bezeichnung, die aufgedruckt ist */
	#define RC5_CODE_PWR						(0x100C & RC5_MASK)		/*!< Taste An/Aus-Symbol (rot)*/
	#define RC5_CODE_MUTE						(0x100D & RC5_MASK)		/*!< Taste Mute-Symbol (gruen) */
	#define RC5_CODE_CH_PLUS					(0x1020 & RC5_MASK)		/*!< Taste CH +  (blau) */
	#define RC5_CODE_CH_MINUS					(0x1021 & RC5_MASK)		/*!< Taste CH -  (blau) */
	#define RC5_CODE_VOL_MINUS					(0x1011 & RC5_MASK)		/*!< Taste VOL - (blau) */
	#define RC5_CODE_VOL_PLUS					(0x1010 & RC5_MASK)		/*!< Taste VOL + (blau) */
	#define RC5_CODE_BOX_WITH_DOT				(0x0017 & RC5_MASK)		/*!< Taste Quadrat mit Punkt */
	#define RC5_CODE_TV_VCR					(0x1038 & RC5_MASK)		/*!< Taste TV/VCR (gleicher Code wie Taste A.B) */
	#define RC5_CODE_0							(0x1000 & RC5_MASK)		/*!< Taste 0 */
	#define RC5_CODE_1							(0x1001 & RC5_MASK)		/*!< Taste 1 */
	#define RC5_CODE_2							(0x1002 & RC5_MASK)		/*!< Taste 2 */
	#define RC5_CODE_3							(0x1003 & RC5_MASK)		/*!< Taste 3 */
	#define RC5_CODE_4							(0x1004 & RC5_MASK)		/*!< Taste 4 */
	#define RC5_CODE_5							(0x1005 & RC5_MASK)		/*!< Taste 5 */
	#define RC5_CODE_6							(0x1006 & RC5_MASK)		/*!< Taste 6 */
	#define RC5_CODE_7							(0x1007 & RC5_MASK)		/*!< Taste 7 */
	#define RC5_CODE_8							(0x1008 & RC5_MASK)		/*!< Taste 8 */
	#define RC5_CODE_9							(0x1009 & RC5_MASK)		/*!< Taste 9 */
	#define RC5_CODE_RETURN					(0x1022 & RC5_MASK)  	/*!< Taste Return-Symbol (gleicher Code wie Taste EXIT) */
	#define RC5_CODE_A_DOT_B					(0x1038 & RC5_MASK)		/*!< Taste A.B (gleicher Code wie Taste TV/VCR) */
	#define RC5_CODE_WAIT						(0x002F & RC5_MASK)		/*!< Taste Pause-Symbol */
	#define RC5_CODE_GREEN						(0x002C & RC5_MASK)		/*!< Taste Gruen & Stop-Symbol */
	#define RC5_CODE_RECORD					(0x102E & RC5_MASK)		/*!< Taste Aufnahme-Symbol */
	#define RC5_CODE_BOX						(0x103F & RC5_MASK)		/*!< Taste Quadrat */
	#define RC5_CODE_RED						(0x002B & RC5_MASK)		/*!< Taste Rot  & << */
	#define RC5_CODE_YELLOW					(0x002D & RC5_MASK)		/*!< Taste Gelb & >  */
	#define RC5_CODE_BLUE						(0x002E & RC5_MASK)		/*!< Taste Blau & >> */
	#define RC5_CODE_BOX_WITH_3_EQUAL_LINES	(0x103C & RC5_MASK)		/*!< Taste Quadrat mit 3 gleichlangen Linien */
	#define RC5_CODE_GREEN_UP					(0x0010 & RC5_MASK)		/*!< Taste hoch   (gruen) */
	#define RC5_CODE_GREEN_LEFT				(0x0015 & RC5_MASK)		/*!< Taste links  (gruen) */
	#define RC5_CODE_GREEN_RIGHT				(0x0016 & RC5_MASK)		/*!< Taste rechts (gruen) */
	#define RC5_CODE_GREEN_DOWN				(0x0011 & RC5_MASK)		/*!< Taste runter (gruen) */
	#define RC5_CODE_BOX_WITH_BOX				(0x1029 & RC5_MASK)		/*!< Taste Quadrat mit innerem Rechteck und Pfeilen */
	#define RC5_CODE_BOX_WITH_3_UNEQUAL_LINES	(0x102E & RC5_MASK)		/*!< Taste Quadrat mit 3 ungleichlangen Linien */
	#define RC5_CODE_OK						(0x1023 & RC5_MASK)		/*!< Taste OK (gruen)  */
	#define RC5_CODE_MENU						(0x0012 & RC5_MASK)		/*!< Taste MENU */
	#define RC5_CODE_EXIT						(0x1022 & RC5_MASK)		/*!< Taste EXIT (gleicher Code wie Taste Return) */
	
	/* Vorhandene Tasten werden hier mit der Wunsch-Funktion belegt (Umwidmung)*/
	#define RC5_CODE_UP		RC5_CODE_CH_PLUS		/*!< Taste CH +  wird genutzt fuer UP-Funktion */
	#define RC5_CODE_DOWN		RC5_CODE_CH_MINUS		/*!< Taste CH -  wird genutzt fuer  DOWN-Funktion */
	#define RC5_CODE_LEFT		RC5_CODE_VOL_MINUS		/*!< Taste VOL - wird genutzt fuer LEFT-Funktion */
	#define RC5_CODE_RIGHT		RC5_CODE_VOL_PLUS		/*!< Taste VOL + wird genutzt fuer RIGHT-Funktion */
	#define RC5_CODE_VIEW		RC5_CODE_TV_VCR			/*!< Taste TV/VCR & A.B werden genutzt fuer VIEW-Funktion*/
	#define RC5_CODE_SELECT	RC5_CODE_RETURN			/*!< Taste Return & Exit werden genutzt fuer SELECT-Funktion */
	#define RC5_CODE_BWD		RC5_CODE_GREEN_LEFT		/*!< Taste links  (gruen) wird genutzt fuer BWD-Funktion (backward)*/
	#define RC5_CODE_FWD		RC5_CODE_GREEN_RIGHT	/*!< Taste rechts (gruen) wird genutzt fuer FWD-Funktion (forward)*/
	
	#define RC5_CODE_I_II		RC5_CODE_SELECT
	#define RC5_CODE_TV_VCR	RC5_CODE_VIEW
#endif /* RC_HAVE_VIVANCO_UR89_TV_CODE_089 */ 

/*!
 * Fernbedienung Technisat_TTS35AI (Receiver Digit CIP)
 * @author Joerg Bullmann
 */
#ifdef RC_HAVE_Technisat_TTS35AI
	#define RC_HAVE_CODES							/*!< Definiert Codes */
	#undef JOG_DIAL									/*!< Hat keinen Jog Dial */

	#define RC5_NOT_AVAIL	(0xFFFF)				/*!< Code fuer Taste nicht vorhanden */

	#define RC5_CODE_0		(0x1289 & RC5_MASK)		/*!< Taste 0 			   */
	#define RC5_CODE_1		(0x1281 & RC5_MASK)		/*!< Taste 1 			   */
	#define RC5_CODE_2		(0x1282 & RC5_MASK)		/*!< Taste 2 			   */
	#define RC5_CODE_3		(0x1283 & RC5_MASK)		/*!< Taste 3 			   */
	#define RC5_CODE_4		(0x1284 & RC5_MASK)		/*!< Taste 4 			   */
	#define RC5_CODE_5		(0x1285 & RC5_MASK)		/*!< Taste 5 			   */
	#define RC5_CODE_6		(0x1286 & RC5_MASK)		/*!< Taste 6 			   */
	#define RC5_CODE_7		(0x1287 & RC5_MASK)		/*!< Taste 7 			   */
	#define RC5_CODE_8		(0x1288 & RC5_MASK)		/*!< Taste 8 			   */
	#define RC5_CODE_9		(0x1289 & RC5_MASK)		/*!< Taste 9 			   */

	#define RC5_CODE_PWR	(0x128C & RC5_MASK)		/*!< Taste An, Aus         */
	#define RC5_CODE_INFO	(0x028F & RC5_MASK)		/*!< Taste i               */
	#define RC5_CODE_OK	(0x0297 & RC5_MASK)		/*!< Taste ok              */

	#define RC5_CODE_UP	(0x12A0 & RC5_MASK)		/*!< Taste Hoch            */
	#define RC5_CODE_DOWN	(0x12A1 & RC5_MASK)		/*!< Taste Runter          */
	#define RC5_CODE_LEFT	(0x1291 & RC5_MASK)		/*!< Taste Links           */
	#define RC5_CODE_RIGHT	(0x1290 & RC5_MASK)		/*!< Taste Rechts          */

	#define RC5_CODE_TV	(0x0293 & RC5_MASK)		/*!< Taste TV              */
	#define RC5_CODE_MENU	(0x0292 & RC5_MASK)		/*!< Taste Menu            */
	#define RC5_CODE_RED	(0x02AB & RC5_MASK)		/*!< rote Taste            */
	#define RC5_CODE_GREEN	(0x02AC & RC5_MASK)		/*!< gruene Taste          */
	#define RC5_CODE_YELLOW	(0x02AD & RC5_MASK)		/*!< gelbe Taste           */
	#define RC5_CODE_BLUE	(0x02AE & RC5_MASK)		/*!< blaue Taste           */

	#define RC5_CODE_FWD	RC5_CODE_TV				/*!< Taste TV - umgewidmet als FWD-Taste */
	#define RC5_CODE_BWD	RC5_CODE_MENU			/*!< Taste Menu - umgewidmet als BWD-Taste */
	#define RC5_CODE_TV_VCR	RC5_CODE_INFO			/*!< Taste INFO - umgewidmet als View-Taste */
	#define RC5_CODE_I_II	RC5_CODE_OK			/*!< Taste OK - umgewidmet als Select-Taste */

#endif /* RC_HAVE_Technisat_TTS35AI */

/*!
 * Default-Philips-Fernbedienung mit Jog-Dial-Rad
 */

#ifdef RC_HAVE_DEFAULT			/*!< Default RC5-Codes falls keine FB definiert wurde */
 
	#undef JOG_DIAL
	
	#define RC5_CODE_0			(0x3940 & RC5_MASK)		/*!< Taste 0 */
	#define RC5_CODE_1			(0x3941 & RC5_MASK)		/*!< Taste 1 */
	#define RC5_CODE_2			(0x3942 & RC5_MASK)		/*!< Taste 2 */
	#define RC5_CODE_3			(0x3943 & RC5_MASK)		/*!< Taste 3 */
	#define RC5_CODE_4			(0x3944 & RC5_MASK)		/*!< Taste 4 */
	#define RC5_CODE_5			(0x3945 & RC5_MASK)		/*!< Taste 5 */
	#define RC5_CODE_6			(0x3946 & RC5_MASK)		/*!< Taste 6 */
	#define RC5_CODE_7			(0x3947 & RC5_MASK)		/*!< Taste 7 */
	#define RC5_CODE_8			(0x3948 & RC5_MASK)		/*!< Taste 8 */
	#define RC5_CODE_9			(0x3949 & RC5_MASK)		/*!< Taste 9 */
	
	#define RC5_CODE_UP		(0x2950 & RC5_MASK)		/*!< Taste Hoch */
	#define RC5_CODE_DOWN		(0x2951 & RC5_MASK)		/*!< Taste Runter */
	#define RC5_CODE_LEFT		(0x2955 & RC5_MASK)		/*!< Taste Links */
	#define RC5_CODE_RIGHT		(0x2956 & RC5_MASK)		/*!< Taste Rechts */
	
	#define RC5_CODE_PWR		(0x394C & RC5_MASK)		/*!< Taste An/Aus */
	
	#define RC5_CODE_RED		(0x100B & RC5_MASK)		/*!< Rote Taste */
	#define RC5_CODE_GREEN		(0x102E & RC5_MASK)		/*!< Gruene Taste */
	#define RC5_CODE_YELLOW	(0x1038 & RC5_MASK)		/*!< Gelbe Taste */
	#define RC5_CODE_BLUE		(0x1029 & RC5_MASK)		/*!< Blaue Taste */
	#define RC5_CODE_VIEW		(0x000F & RC5_MASK)		/*!< Instant View Taste */
	
	#define RC5_CODE_SELECT	(0x100B & RC5_MASK)		/*!< Select Taste */
	
	#define RC5_CODE_BWD		(0x1025 & RC5_MASK)		/*!< Backward Taste */
	#define RC5_CODE_FWD		(0x1026 & RC5_MASK)		/*!< Forward Taste */

	#define RC5_CODE_I_II		RC5_CODE_SELECT
	#define RC5_CODE_TV_VCR	RC5_CODE_VIEW

	#ifdef JOG_DIAL		
		/* Jogdial geht nur inkl. Adresscode */
		#undef RC5_MASK
		#define RC5_MASK (RC5_COMMAND | RC5_ADDRESS)
	
		#define RC5_CODE_JOG_MID	(0x3969 & RC5_MASK)	/*!< Taste Jog-Dial Mitte */
		#define RC5_CODE_JOG_L1	(0x3962 & RC5_MASK)	/*!< Taste Jog-Dial Links 1 */
		#define RC5_CODE_JOG_L2	(0x396F & RC5_MASK)	/*!< Taste Jog-Dial Links 2 */
		#define RC5_CODE_JOG_L3	(0x395F & RC5_MASK)	/*!< Taste Jog-Dial Links 3 */
		#define RC5_CODE_JOG_L4	(0x3A6C & RC5_MASK)	/*!< Taste Jog-Dial Links 4 */
		#define RC5_CODE_JOG_L5	(0x3A6B & RC5_MASK)	/*!< Taste Jog-Dial Links 5 */
		#define RC5_CODE_JOG_L6	(0x396C & RC5_MASK)	/*!< Taste Jog-Dial Links 6 */
		#define RC5_CODE_JOG_L7	(0x3A6A & RC5_MASK)	/*!< Taste Jog-Dial Links 7 */
		
		#define RC5_CODE_JOG_R1	(0x3968 & RC5_MASK)	/*!< Taste Jog-Dial Rechts 1 */
		#define RC5_CODE_JOG_R2	(0x3975 & RC5_MASK)	/*!< Taste Jog-Dial Rechts 2 */
		#define RC5_CODE_JOG_R3	(0x396A & RC5_MASK)	/*!< Taste Jog-Dial Rechts 3 */
		#define RC5_CODE_JOG_R4	(0x3A6D & RC5_MASK)	/*!< Taste Jog-Dial Rechts 4 */
		#define RC5_CODE_JOG_R5	(0x3A6E & RC5_MASK)	/*!< Taste Jog-Dial Rechts 5 */
		#define RC5_CODE_JOG_R6	(0x396E & RC5_MASK)	/*!< Taste Jog-Dial Rechts 6 */
		#define RC5_CODE_JOG_R7	(0x3A6F & RC5_MASK)	/*!< Taste Jog-Dial Rechts 7 */
	#endif	/* JOG_DIAL */

#endif /* !RC_HAVE_CODES */

#endif /* RC5CODE_H_ */
