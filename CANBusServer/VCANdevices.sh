# Загружаем модуль ядра для виртуального CAN
sudo modprobe vcan 
sudo modprobe can-gw


# Создаем два виртуальных интерфейса
sudo ip link add dev vcan0 type vcan
sudo ip link add dev vcan1 type vcan

sudo ip link set vcan0 mtu 72  # 72 - максимальный размер для CAN FD
sudo ip link set vcan1 mtu 72

# Активируем их
sudo ip link set up vcan0 
sudo ip link set up vcan1 

# Проверяем, что интерфейсы появились
ip link show vcan0 
ip link show vcan1 

sudo cangw -A -s vcan0 -d vcan1 -e
sudo cangw -A -s vcan1 -d vcan0 -e

sudo cangw -L
