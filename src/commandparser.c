/*
 * commandparser.c
 *
 *  Created on: Dec 20, 2017
 *      Author: bahadir
 */
#include <stdint.h>

#include "commandparser.h"
#include <string.h>

movement_command_t command_t;

movement_command_t* parse_command(char* command) {
	char *id;
	char number[3];
	char commandstr[7];
	int direction1, dir1mag, direction2, dir2mag;
	id = strstr (command, "ID");
	if (id)
	{
		id += 2;
		strncpy (number, id, 2);
		number[2] = '\0';
		id += 2;
		strncpy (commandstr, id, 6);
		commandstr[6] = '\0';
		if(commandstr[0] == 'N') { direction1 = DIR_NORTH; }
		else if(commandstr[0] == 'S') { direction1 = DIR_SOUTH; }
		else if(commandstr[0] == 'W') { direction1 = DIR_WEST; }
		else if(commandstr[0] == 'E') { direction1 = DIR_EAST; }
		else { return -1; }
		dir1mag = (commandstr[1]-48)*10+commandstr[2]-48;
		if(dir1mag < 0 || dir1mag > 100) { return -1; }
		if(commandstr[3] == 'N') { direction2 = DIR_NORTH; }
		else if(commandstr[3] == 'S') { direction2 = DIR_SOUTH; }
		else if(commandstr[3] == 'W') { direction2 = DIR_WEST; }
		else if(commandstr[3] == 'E') { direction2 = DIR_EAST; }
		else { return -1; }
		dir2mag = (commandstr[4]-48)*10+commandstr[5]-48;
		if(dir2mag < 0 || dir2mag > 100) { return -1; }
		if(dir1mag+dir2mag == 1 || dir1mag + dir2mag == 5) { return -1; }

		command_t.direction1 = direction1;
		command_t.magnitude1 = dir1mag;
		command_t.direction2 = direction2;
		command_t.magnitude2 = dir2mag;
		return &command_t;
	}
	else
	{
		return -1;
	}
}
