package model.enemies;

import model.Ship;

public abstract class Enemy extends Ship {
    protected final int shootFrequencyModifier;

    protected Enemy(int x, int y, int width, int height,
                    int speedX, int speedY, int maxHealth, int freqModifier) {
        super(x, y, width, height, speedX, speedY, maxHealth);
        shootFrequencyModifier = freqModifier;
    }
}
