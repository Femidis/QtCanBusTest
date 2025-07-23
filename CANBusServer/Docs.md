1. Установка библиотек для socketcan

	Нужна для работы модуля QCanBus 

	```bash
	sudo apt update
	sudo apt install libsocketcan-dev libsocketcan2
	```

2. Установка дополнительных утилит

	Нужны для создания виртуальных CAN-устройств
	и их соединения между собой

	```bash
	sudo apt install can-utils net-tools
	```














Скорость от расстояния CAN  
$ y=20090.6644\cdot x^{-0.806092} $