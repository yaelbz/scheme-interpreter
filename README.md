# ybscheme

Scheme interpreter written in C

##Setup
- compile with -std=c99

##Features
###read:
- quote
- lists (cons)
- strings
- numbers, positive and negative
-- decimal
-- binary
-- octal
-- hexadecimal
-- true, false, nil, void
-- symbol
Other features in reader:
- unread char stack
- ignore comments
###eval:
- builtin functions
-- +
-- -
-- *
-- /
-- eq?
-- =
-- eqv?
-- not
-- cons
-- car
-- cdr
- builtin syntax
-- quote
-- if
-- lambda
-- define
Other features in eval:
- 

##Issues
- float
- file stream
- error handling
-- handle memory
-- errorNums in enum
-- return to main execution
