PhysicsCalculator
====
Calculating values with physics unit

##Description
This tool is to calculate values that with various physics
units. Only use for your physics homework!

##Install
You can use your g++ compiler as this command:

    g++ basicNumeric.cpp -std=c++11 -lreadline -o basicNumeric

Or using 'make' to install:

    make

##Usage
A normal mathematical formulas will be recognaized as values.
For examples:

1 plus 2:

    1+2

Set value of variable x as 1.0:

    x=1.0

Add a unit 't' in type of mass:

    #setUnit t=1000 mass

Change the default unit to show mass:

    #setDefault t mass

Read commands in file:

    #read filename

Get the value of an unit 'ABC':

    %ABC


##Requirement
libreadline 6

regex

c++11

##Licence
Copyright (c) 2016 holmesfems

Released under MIT license

(http://opensource.org/licenses/mit-license.php)

##Author
[holmesfems](https://github.com/holmesfems)
