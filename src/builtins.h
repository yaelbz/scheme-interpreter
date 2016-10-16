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
OBJ builtinEqvQ(int);
OBJ builtinEqQ(int);
OBJ builtinEqualOperator(int);
OBJ builtinEqualQ(int);
OBJ builtinNot(int);
OBJ builtinCons(int);
OBJ builtinCar(int);
OBJ builtinCdr(int);


OBJ builtinDefine(OBJ, OBJ);
OBJ builtinIf(OBJ, OBJ);
OBJ builtinLambda(OBJ, OBJ);
OBJ builtinQuote(OBJ, OBJ);



#endif /* BUILTINS_H_ */
