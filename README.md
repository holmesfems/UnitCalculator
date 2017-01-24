PhysicsCalculator
====
Calculating values with physics unit

##Description
This tool is to calculate values that with various physics
units. Only use for your physics homework!

##Download
Please download from [Release Page](https://github.com/holmesfems/UnitCalculator/releases)

##Install

Using 'make' to install:

    make

    sudo ./install.sh

##Usage
A normal mathematical formulas will be recognaized as values.
For examples:

1 plus 2:

    1+2

Set the value of a variable:

    x=1.0

Set the value of basic unit:

    Add a unit 't' in type of mass:

    #setUnit t=1000 mass

Gat the value an unit:

    %kg

Set the value of a derived unit:

    %N=%kg*%m/%s^2

Change the default unit to show mass:

    #setDefault t mass

Read commands in file:

    #read filename

##Requirement
libreadline 6

regex

c++11

##Changelog
1.4:

    Improved #list command

    Added new function value(x):get the showing value of x

    Added new function unit(x):get the unit of x

1.3b:

    Make it to clean up memory

    Make it throw an error while calculating an unknown operator

    Fixed a bug to set value of unit

1.2:

    Add a flag to inverse the order to calculate multiplication and division

    Add a command to show all commands:#help

    Add a command to show all groups:#list

    Fixed a bug that will save unit token to the global group

1.1r1:

    Fit the compile at windows by mingw++ compiler

##Licence
Copyright (c) 2016 holmesfems

Released under MIT license

(http://opensource.org/licenses/mit-license.php)

##Author
[holmesfems](https://github.com/holmesfems)
