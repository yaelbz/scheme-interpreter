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


OBJ builtinEqvQ(OBJ, OBJ);
OBJ builtinEqQ(OBJ, OBJ);
OBJ builtinEqualOperator(OBJ, OBJ);
OBJ builtinEqualQ(OBJ, OBJ);
OBJ builtinDefine(OBJ, OBJ);
OBJ builtinIf(OBJ, OBJ);
OBJ builtinLambda(int);



#endif /* BUILTINS_H_ */
