package model;

import model.Entity;
import model.bullets.Bullet;

public abstract class Ship extends Entity {
    protected int health;
    protected final int maxHealth;

    protected Ship(int x, int y, int width, int height, int speedX, int speedY, int maxHealth) {
        super(x, y, width, height, speedX, speedY);
        this.health = maxHealth;
        this.maxHealth = maxHealth;
    }

    public void increaseHealth() {
        if (health < maxHealth) health++;
    }

    public void decreaseHealth() {
        if (health > 0) health--;
    }

    public int getHealth() {
        return health;
    }

    public abstract Bullet shoot();
}