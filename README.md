# Assembly++

Assembly++ is an assembly language designed to be more readable than tradional assembly languages by featuring intuitive syntax with operators like +, = and <.

For specifications of the language, see **Asm++ Reference Sheet.pdf**.

To get up and running:
1. Compile asmppc.cpp and asmppvm.cpp with an optional C++ compiler (e.g. g++).
2. Compile example code in multiplication.asmpp with ``.\asmppc multiplication.asmpp multiplication.asmppbin``
3. Run the binary in the virtual machine with ``.\asmppvm multiplication.asmppbin``

For debug output in the virtual machine, add the parameter ``-debug`` and it will display all instructions run and the value of every register. To slow down the program, sufix the ``-debug`` parameter with a delay in milliseconds.
Ex: ``.\asmppvm multiplication.asmppbin -debug 1500``
