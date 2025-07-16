# Загружаем модуль ядра для виртуального CAN
sudo modprobe vcan

# Создаем два виртуальных интерфейса
sudo ip link add dev vcan0 type vcan
#sudo ip link add dev vcan1 type vcan

# Активируем их
sudo ip link set up vcan0
#sudo ip link set up vcan1

# Проверяем, что интерфейсы появились
ip link show vcan0
#ip link show vcan1
