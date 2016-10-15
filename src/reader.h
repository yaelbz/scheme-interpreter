/*
 * reader.h
 * Definitionen vom reader
 *
 *  Created on: 01.09.2016
 *      Author: yael
 */

#ifndef READER_H_
#define READER_H_

#include "global.h"

void initReader();
void flushReaderInputStream();
OBJ ybRead(FILE*);


#endif /* READER_H_ */
