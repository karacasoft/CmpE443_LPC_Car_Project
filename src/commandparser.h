/*
 * commandparser.h
 *
 *  Created on: Dec 20, 2017
 *      Author: bahadir
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#define DIR_NORTH 0
#define DIR_SOUTH 1
#define DIR_EAST 2
#define DIR_WEST 3

typedef struct {
	uint8_t direction1;
	uint8_t magnitude1;
	uint8_t direction2;
	uint8_t magnitude2;
} movement_command_t;

movement_command_t* parse_command(char* command);

#endif /* COMMANDPARSER_H_ */
