# # STM32 Shell Simulator

## Develop Docs

### File Tree

```
├── build
│   ├── sh
│   └── startup
├── include
│   └── sh
├── src
│   └── sh
├── startup
└── STM32F10x_StdPeriph_Lib_V3.6.0
```

### organize file

```
shell read-> shell parse -> shell event -> shell out
			 -> shell error -> error code
```


```
		usart I/O
		    |
		shell parse
```
