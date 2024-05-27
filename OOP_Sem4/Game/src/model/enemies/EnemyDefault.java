package model.enemies;

import model.bullets.Bullet;
import model.bullets.BulletDefault;
import model.bullets.BulletDirection;

import java.util.Random;

public class EnemyDefault extends Enemy {
    public static final int enemyDefWidth = 45;
    public static final int enemyDefHeight = 34;
    private final Random random = new Random();

    public EnemyDefault(int x, int y, int freqModifier) {
        super(x, y, enemyDefWidth, enemyDefHeight, 1, 0, 1, freqModifier);
    }

    @Override
    public void increaseHealth() {}

    @Override
    public Bullet shoot() {
        int randomInt = random.nextInt(shootFrequencyModifier);
        if (randomInt < 2) return new BulletDefault(this, BulletDirection.DOWN);
        return null;
    }
}
