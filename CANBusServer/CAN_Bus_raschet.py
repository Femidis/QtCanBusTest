import math

def calculate_max_nodes(
    # Параметры трансивера (из даташита)
    R_driver_ol_max=55.0,    # Макс. выходное сопротивление драйвера в доминантном состоянии [Ом]
    V_OL_max=1.75,           # Макс. напряжение CAN_L в доминантном состоянии [В]
    Iol_max=0.07,            # Макс. ток драйвера в низком уровне [А] (70 мА)
    Rin=20000.0,             # Входное сопротивление узла [Ом]
    Cin=20.0,                # Входная емкость узла [пФ]

    # Параметры шины
    bit_rate=500000.0,       # Скорость передачи [бит/с] (для фазы арбитража в CAN FD)
    V_diff=1.5,              # Минимальная разница напряжений CAN_H - CAN_L для распознавания [В]
    Rterm=120.0,             # Сопротивление терминатора [Ом]
    cable_capacitance=50.0,  # Емкость кабеля [пФ/м]
    cable_length=40.0,       # Длина шины [м]
    stub_capacitance=10.0,   # Емкость отвода к узлу [пФ]
    connector_capacitance=5.0, # Емкость коннектора [пФ]
    connectors_per_node=1.0, # Количество коннекторов на узел

    # Коэффициент запаса
    safety_margin=0.7,       # Рекомендуемый запас (0.7 = 30% запас)
):
    """
    Рассчитывает максимальное количество узлов на шине CAN/CAN FD на основе:
    1) Ограничения по напряжению в доминантном состоянии (DC VOL)
    2) Ограничения по емкостной нагрузке (целостность сигнала)
    Возвращает рекомендуемое количество узлов с учетом запаса.
    """

    # --- 1. Расчет по напряжению (DC VOL) ---
    Rterm_total = Rterm / 2  # Два терминатора параллельно
    V_canL = V_OL_max        # Для оценки берем максимально допустимое

    # Ток через терминаторы
    I_term = V_diff / Rterm_total

    # Ток утечки на один узел
    I_leak_per_node = V_canL / Rin

    # Макс. количество узлов по напряжению
    try:
        N_max_voltage = ((V_OL_max / R_driver_ol_max) - I_term) / I_leak_per_node
        N_max_voltage = max(0, N_max_voltage)  # Не может быть отрицательным
    except ZeroDivisionError:
        N_max_voltage = float('inf')

    # --- 2. Расчет по емкости (AC) ---
    bit_time = 1.0 / bit_rate
    t_rise_max = 0.25 * bit_time  # Макс. время нарастания (25% битового времени)

    # Общая емкость кабеля
    C_cable_total = cable_capacitance * cable_length

    # Емкость на один узел (вход + отвод + коннекторы)
    C_per_node = Cin + stub_capacitance + (connector_capacitance * connectors_per_node)

    # Макс. общая емкость шины из требования к скорости нарастания
    try:
        C_bus_max = t_rise_max / R_driver_ol_max
        C_bus_max = C_bus_max * 1e12  # Переводим в пФ

        # Макс. количество узлов по емкости
        N_max_capacitance = (C_bus_max - C_cable_total) / C_per_node
        N_max_capacitance = max(0, N_max_capacitance)
    except ZeroDivisionError:
        N_max_capacitance = float('inf')

    # --- 3. Определяем лимитирующий фактор ---
    N_max_system = min(N_max_voltage, N_max_capacitance)

    # Применяем запас
    N_recommended = math.floor(N_max_system * safety_margin)

    # --- 4. Проверка по току драйвера (обычно не лимитирует) ---
    I_total = I_term + (N_recommended * I_leak_per_node)
    current_ok = I_total <= Iol_max

    # --- Формируем результаты ---
    results = {
        'N_max_voltage': N_max_voltage,
        'N_max_capacitance': N_max_capacitance,
        'N_max_system': N_max_system,
        'N_recommended': N_recommended,
        'current_check': current_ok,
        'I_total': I_total * 1000,  # в мА
        'C_bus_total': C_cable_total + (N_recommended * C_per_node),
        'limiting_factor': 'voltage' if N_max_voltage < N_max_capacitance else 'capacitance',
    }

    return results

# Пример использования с параметрами TJA1051
if __name__ == "__main__":
    config = {
        'R_driver_ol_max': 55.0,     # Из даташита TJA1051
        'V_OL_max': 1.75,            # По ISO 11898-2
        'Iol_max': 0.07,             # 70 мА
        'Rin': 20000.0,              # 20 кОм
        'Cin': 20.0,                 # 20 пФ
        'bit_rate': 500000.0,        # 500 кбит/с
        'cable_length': 40.0,        # 40 метров
    }

    results = calculate_max_nodes(**config)

    print("\nРезультаты расчёта максимального количества узлов CAN/CAN FD:")
    print(f"1. По напряжению (DC VOL): {results['N_max_voltage']:.1f} узлов")
    print(f"2. По ёмкости (AC):       {results['N_max_capacitance']:.1f} узлов")
    print(f"3. Лимитирующий фактор:   {results['limiting_factor']}")
    print(f"4. Макс. теоретически:    {results['N_max_system']:.1f} узлов")
    print(f"5. Рекомендуется с запасом {int((1 - config['safety_margin'])*100)}%: {results['N_recommended']} узлов")
    print(f"\nДополнительные параметры:")
    print(f"- Общий ток шины:         {results['I_total']:.1f} мА (при {config['Iol_max']*1000:.0f} мА max)")
    print(f"- Общая ёмкость шины:     {results['C_bus_total']:.0f} пФ")
    print(f"- Проверка по току:       {'OK' if results['current_check'] else 'Превышен!'}")
