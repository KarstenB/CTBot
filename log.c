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

/*! @file 	log.c
 * @brief 	Routinen zum Loggen von Informationen. Es sollten ausschliesslich nur
 * die Log-Makros: LOG_DEBUG(), LOG_INFO(), LOG_WARN(), LOG_ERROR() und LOG_FATAL()
 * verwendet werden.
 * Eine Ausgabe kann wie folgt erzeugt werden:
 * LOG_DEBUG(("Hallo Welt!"));
 * LOG_INFO(("Wert x=%d", x));
 * Wichtig ist die doppelte Klammerung. Bei den Ausgaben kann auf ein Line Feed
 * '\n' am Ende des Strings verzichtet werden, da dies automatisch angeh�ngt
 * hinzugefuegt wird.
 * 
 * @author 	Andreas Merkle (mail@blue-andi.de)
 * @date 	27.02.06
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"
#include "display.h"
#include "command.h"
#include "uart.h"

#ifdef LOG_AVAILABLE

#ifdef PC
#include <pthread.h>
#endif	/* PC */

#ifdef LOG_DISPLAY_AVAILABLE
	/*! Nummer des Screens auf den Loggings ausgegeben werden sollen. */
	#define LOG_TO_SCREEN		4
	
	/*! Groesse des Puffers fuer die Logausgaben bei Verwendung des LCD-Displays. */
	#define LOG_BUFFER_SIZE		(DISPLAY_LENGTH + 1)
#else
	/*! Groesse des Puffers fuer die Logausgaben ueber UART und ueber TCP/IP. */
	#define LOG_BUFFER_SIZE		200
#endif	/* LOG_DISPLAY_AVAILABLE */


#ifdef PC
	/*! Schuetzt den Ausgabepuffer */
	#define LOCK()		pthread_mutex_lock(&log_buffer_mutex);
	/*! Hebt den Schutz fuer den Ausgabepuffer wieder auf */
	#define UNLOCK()	pthread_mutex_unlock(&log_buffer_mutex);
#else
	/*! Schuetzt den Ausgabepuffer */
	#define LOCK()		/* TODO */
	/*! Hebt den Schutz fuer den Ausgabepuffer wieder auf */
	#define UNLOCK()	/* TODO */
#endif	/* PC */

/*!
 * Liefert den Log-Typ als String.
 * @param log_type Log-Typ
 * @return char*
 */
static char* log_get_type_str(LOG_TYPE log_type);

/*! Puffer fuer das Zusammenstellen einer Logausgabe */
static char log_buffer[LOG_BUFFER_SIZE];

#ifdef PC
	/*! Schuetzt den Ausgabepuffer */
	static pthread_mutex_t log_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif	/* PC */

#ifdef LOG_DISPLAY_AVAILABLE
	/*! Zeile in der die naechste Logausgabe erfolgt. */
	static uint16 log_line = 0;
#endif	/* LOG_DISPLAY_AVAILABLE */

/*!
 * Schreibt Angaben ueber Datei, Zeilennummer und den Log-Typ in den Puffer.
 * Achtung, Mutex wird gelockt und muss explizit durch log_end() wieder
 * freigegeben werden!
 * @param filename Dateiname
 * @param line Zeilennummer
 * @param log_type Log-Typ
 */
extern void log_begin(char *filename, unsigned int line, LOG_TYPE log_type) {

/* Ausgaben ueber das LCD-Display werden ohne Dateiname und Zeilennumer
 * gemacht. Der Log-Typ versteckt sich im ersten Buchstaben. Durch eine
 * die Markierung mit '>' erkennt man das letzte Logging.
 * Nur bei Ausgaben ueber UART und an ct-Sim werden Dateiname, Zeilennummer
 * und der Log-Typ vollstaendig ausgegeben.
 */
	#ifdef LOG_DISPLAY_AVAILABLE
	
		LOCK();
	
		/* Nur auf einem bestimmten Screen werden Loggings ausgegeben. */
		if (display_screen != LOG_TO_SCREEN) {
			log_buffer[0] = '\0';
			return;
		}
	
		/* Zum ersten Mal aufgerufen? */
		if (log_line == 0) {
			log_line++;
		} else {
			/* Alte Markierung loeschen */
			if (log_line - 1 == 0) {
				display_cursor(4, 1);
			}
			else {
				display_cursor(log_line - 1, 1);
			}
			display_printf(" ");
		}
		display_cursor(log_line, 1);
		log_line++;
		if (log_line >= DISPLAY_SCREENS) {
			log_line = 1;
		}
		
		snprintf(log_buffer, LOG_BUFFER_SIZE, ">%s:", log_get_type_str(log_type));
	
	#else
	
		char *ptr = NULL;
	
		/* Nur den Dateinamen loggen, ohne Verzeichnisangabe */
		ptr = strrchr(filename, '/');
	
		if (ptr == NULL)
			ptr = filename;
		else 
			ptr++;
	
		LOCK();
	
		snprintf(log_buffer, LOG_BUFFER_SIZE, "%s(%d)\t%s\t", 
			ptr, line, log_get_type_str(log_type));
		
	#endif
		
	return;	
}

/*!
 * Schreibt die eigentliche Ausgabeinformation in den Puffer.
 * @param format Format
 */
extern void log_printf(char *format, ...) {

	va_list	args;
	unsigned int len = strlen(log_buffer);
	
	#ifdef LOG_DISPLAY_AVAILABLE
		/* Nur auf einem bestimmten Screen werden Loggings ausgegeben. */
		if (display_screen != LOG_TO_SCREEN)
			return;
	#endif
	
	va_start(args, format);
	vsnprintf(&log_buffer[len], LOG_BUFFER_SIZE - len, format, args);
	va_end(args);

	return;
}

/*!
 * Gibt den Puffer entsprechend aus.
 */
extern void log_end(void) {

	#ifdef LOG_UART_AVAILABLE
		/* String ueber UART senden, ohne '\0'-Terminierung */
		uart_write(log_buffer, strlen(log_buffer));
		/* Line feed senden */
		uart_write(LINE_FEED,strlen(LINE_FEED));
	#endif	/* LOG_UART_AVAILABLE */
	
	#ifdef LOG_CTSIM_AVAILABLE
		/* Kommando an ct-Sim senden, ohne Line feed am Ende. */
		command_write_data(CMD_LOG, SUB_CMD_NORM, NULL, NULL, log_buffer);
	#endif	/* LOG_CTSIM_AVAILABLE */
	
	#ifdef LOG_DISPLAY_AVAILABLE
		/* Nur auf einem bestimmten Screen werden Loggings ausgegeben. */
		if (display_screen == LOG_TO_SCREEN) {
			/* String aufs LCD-Display schreiben */
			display_printf("%s", log_buffer);
		}
	#endif	/* LOG_DISPLAY_AVAILABLE */

	/* Wenn das Logging aktiviert und keine Ausgabeschnittstelle
	 * definiert ist, dann wird auf dem PC auf die Konsole geschrieben.
	 */
	 
	#ifdef LOG_STDOUT_AVAILABLE
		printf("%s\n", log_buffer);
	#endif	/* LOG_STDOUT_AVAILABLE */

	UNLOCK();
	
	return;
}

#ifdef LOG_DISPLAY_AVAILABLE

/*!
 * Liefert den Log-Typ als String.
 * @param log_type Log-Typ
 * @return char*
 */
static char* log_get_type_str(LOG_TYPE log_type) {
	
	switch(log_type) {
		case LOG_TYPE_DEBUG:
			return "D";
			
		case LOG_TYPE_INFO:
			return "I";
			
		case LOG_TYPE_WARN:
			return "W";
			
		case LOG_TYPE_ERROR:
			return "E";
			
		case LOG_TYPE_FATAL:
			return "F";
			
		default:
			break;
	}

	return "";
}

#else

/*!
 * Liefert den Log-Typ als String.
 * @param log_type Log-Typ
 * @return char*
 */
static char* log_get_type_str(LOG_TYPE log_type) {
	
	switch(log_type) {
		case LOG_TYPE_DEBUG:
			return "- DEBUG -";
			
		case LOG_TYPE_INFO:
			return "- INFO -";
			
		case LOG_TYPE_WARN:
			return "- WARNING -";
			
		case LOG_TYPE_ERROR:
			return "- ERROR -";
			
		case LOG_TYPE_FATAL:
			return "- FATAL -";
			
		default:
			break;
	}

	return "";
}

#endif	/* LOG_DISPLAY_AVAILABLE */

#endif	/* LOG_AVAILABLE */
