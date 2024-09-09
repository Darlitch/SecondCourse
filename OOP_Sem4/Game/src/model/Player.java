package model;

import model.bullets.Bullet;
import model.bullets.BulletDefault;
import model.bullets.BulletDirection;

public class Player extends Ship {
    public static final int playerWidth = 60;
    public static final int playerHeight = 40;
    public static final int playerSpeed = 4;
    // Таймаут между выстрелами игрока
    private final int shootTimeout;
    // Время последнего выстрела игрока
    private long lastShotTime;

    public Player(int x, int y, int shootTimeout) {
        super(x, y, playerWidth, playerHeight,
                0, 0, 3);
        this.shootTimeout = shootTimeout;
    }

    @Override
    public Bullet shoot() {
        long currTime = System.currentTimeMillis();
        if (currTime - lastShotTime > shootTimeout) {
            lastShotTime = currTime;
            return new BulletDefault(this, BulletDirection.UP);
        }
        return null;
    }
}
