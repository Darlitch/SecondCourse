package model.bullets;

import model.Ship;

public class BulletDefault extends Bullet {
    public BulletDefault(Ship shooter, BulletDirection direct) {
        super(shooter, 10, 10, 10, direct);
    }
}
