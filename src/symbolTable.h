/*
 * symbolTable.h
 *
 *  Created on: 04.10.2016
 *      Author: yael
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

void initSymbolTable();
void addToSymbolTable(struct ybSymbol);
struct ybSymbol getFromSymbolTable(char *);
OBJ getOrAddFromSymbolTable(char *);

#endif /* SYMBOLTABLE_H_ */
