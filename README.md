# minipng-parser
## Author : Saad MOUTAOUAKIL

This project is part of the practical work of the secure programming class at INSA Rennes.
It aims to provide a secure command line MiniPNG parser (a simplified version of PNG). 
The program should be injection proof and not allow any bufferoverflow or stack overflow. 
## Requirements 

Only gcc is necessary, no other library is used.
## Compiling

The Makefile handles compiling the program 

```bash 
make
```
this command generates the mini-png executable.

```bash
make clean
``` 
This command removes the temporary files and cleans the workspace.
## Running and Usage

Using the mini-png executable, you can parse the files by running the following command.

```bash
./mini-png "path to your picture" 
``` 

In the case of this program, the only working extension file is .mp to avoid any breach or parsing of any file ressembling the structure of Mini-PNG files.

## Licence 

This program is distributed under the CC-BY-SA license.



