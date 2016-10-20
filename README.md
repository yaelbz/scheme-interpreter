# DEPRECATED!!!
see here: https://github.com/yaelbz/ybscheme


--------
# ybscheme

Scheme simple interpreter written in C

Made with
- Eclipse 

##Features
###read:
- quote
- lists (cons)
- strings
- numbers, positive and negative
 - decimal
 - binary
 - octal
 - hexadecimal
 - true, false, nil, void
 - symbol
 
Other features in reader:
- unread char stack
- ignore comments

###eval:
- builtin functions
 - +
 - -
 - *
 - /
 - eq?
 - =
 - eqv?
 - not
 - cons
 - car
 - cdr
- builtin syntax
 - quote
 - if
 - lambda
 - define
 
Other features in eval:
- 

##Issues
- bool
- file stream
- detect incorrect user input, e.g. closing bracket without opening first
- better feedback
 - concrete undefined message
- more number types
 - float
 - sqrt
 - sin, cos, tan
- more builtins
 - list
 - equal?
- error handling
 - handle memory
 - errorNums in enum
-- return to main execution
