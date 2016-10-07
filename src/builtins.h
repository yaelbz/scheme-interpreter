/*
 * builtins.h
 *
 *  Created on: 03.10.2016
 *      Author: yael
 */

#ifndef BUILTINS_H_
#define BUILTINS_H_

OBJ builtinPlus(int);
OBJ builtinMinus(int);
OBJ builtinMultiplication(int);
OBJ builtinDivision(int);


OBJ builtinDefine(OBJ, OBJ);
OBJ builtinLambda(int);



#endif /* BUILTINS_H_ */
