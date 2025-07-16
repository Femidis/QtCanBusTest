sudo cangw -F

# 2. Отключаем и удаляем интерфейсы
sudo ip link set down vcan0
sudo ip link set down vcan1
sudo ip link del vcan0
sudo ip link del vcan1

# 3. Проверяем
ip link show | grep vcan  # Ничего не должно быть

# 4. Выгружаем модули (опционально)

sudo modprobe -r vcan can-gw
