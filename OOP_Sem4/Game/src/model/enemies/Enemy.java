package model.enemies;

import model.Ship;

public abstract class Enemy extends Ship {
    // Модификатор частоты выстрелов для врагов
    protected final int shootFrequencyModifier;

    protected Enemy(int x, int y, int width, int height,
                    int speedX, int speedY, int maxHealth, int freqModifier) {
        // Вызов конструктора суперкласса Ship с указанием всех параметров корабля
        super(x, y, width, height, speedX, speedY, maxHealth);
        shootFrequencyModifier = freqModifier;
    }
}
