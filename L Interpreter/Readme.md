# L Language Interpreter

## L Language

The L language is a made-up, functional, Lisp like programming language. The interpreter accepts as input a '.l' file and outputs either a number or a list made up of numbers or other lists.

## Function invocations

A list containing ```(f x)``` will be evaluated as long as f is:
- a lambda expression
- a standard function

The standard functions are:
- +: when applied on a list, it computes the sum of every integer found in that list, no matter the level of imbrication
    - ```(+ (1 2 3)) evaluates to 6```
    - ```(+ (1 (2 3) 4)) evaluates to 10```
    - ```(+ (())) evaluates to 0```
- ++: when applied on a list, it concatenates all components found within that list
    - ```(++ (1 (2 3))) evaluates to (1 2 3)```
    - ```(++ ((1 2) (3 4) 5)) evaluates to (1 2 3 4 5)```
    - ```(++ ((1 2) () 3)) evaluates to (1 2 3)```
## Lambda expressions

The L language supports lambda functions. The lambda syntax is the following:

```bash
lambda {id}: {expr}
```
- lambda: specific keyword
- {id}: id which only contains letters
- {expr}: the resulting list, it may contain {id}

## Note

Every file outside of the ```src``` directory is provided by the ```Formal Languages and Automata``` team of ASC

