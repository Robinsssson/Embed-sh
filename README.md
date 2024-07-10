# STM32 Shell Simulator

## Develop Docs
the project is based on the STM32F103C8T6 development board, and the development environment is Keil uVision5. The project is a shell simulator that can be used to simulate the operation of a shell program on a microcontroller. The project includes a shell program, a startup program, and a standard peripheral library for STM32F10x microcontrollers.
The project is organized as follows:
- `build`: The build directory contains the compiled files for the project.
- `include`: The include directory contains the header files for the project.
- `src`: The source directory contains the source files for the project.
- `startup`: The startup directory contains the startup files for the project.
- `STM32F10x_StdPeriph_Lib_V3.6.0`: The standard peripheral library for STM32F10x microcontrollers.

## Usage
To use the project, you need to have a STM32F103C8T6 development board and Keil uVision5 development environment. You can follow the steps below to build and run the project:
1. Open Keil uVision5 and create a new project.
2. Add the source files from the `src` directory to the project.
3. Add the startup files from the `startup` directory to the project.
4. Add the standard peripheral library from the `STM32F10x_StdPeriph_Lib_V3.6.0` directory to the project.
5. Configure the project settings to use the STM32F103C8T6 microcontroller.
6. Build the project and run it on the development board.
compile the project and run it on the development board.
use the `make all` `make flash` to compile and flash the project.
## Features
The project includes the following features:
- A shell program that can parse and execute commands.
- A startup program that initializes the microcontroller and sets up the system clock.
- A standard peripheral library for STM32F10x microcontrollers.
- Support for UART communication for input and output.
## License
The project is licensed under the MIT License.

### File Tree
the project file tree is as follows:
```
├── 3rdparty
│   └── sds
├── build
│   ├── sh
│   ├── startup
│   └── string
├── include
│   └── sh
├── lib
│   └── sds
├── src
│   ├── sh
│   └── string
├── startup
└── STM32F10x_StdPeriph_Lib_V3.6.0
```
diractory:
- `3rdparty`: The third-party libraries used in the project.
- `build`: The build directory contains the compiled files for the project.
- `include`: The include directory contains the header files for the project.
- `lib`: The library directory contains the library files for the project.
- `src`: The source directory contains the source files for the project.
- `startup`: The startup directory contains the startup files for the project.
- `STM32F10x_StdPeriph_Lib_V3.6.0`: The standard peripheral library for STM32F10x microcontrollers.

### organize file
the code is organized as follows:
```
shell read-> shell parse -> shell event -> shell out
			      └── shell error -> error code
```
use buffer to read the shell input, and then parse the input to execute the corresponding command. The output of the command is then sent to the shell output.
If there is an error, the error code is sent to the shell error.

```
		usart I/O
		    |
		shell parse
```
if you want to reuse the project to other MCU, just realize of USART I/O.
