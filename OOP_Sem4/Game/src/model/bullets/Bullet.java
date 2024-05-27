package model.bullets;

import model.Entity;
import model.Ship;

public abstract class Bullet extends Entity {
    protected Bullet(Ship shooter, int width, int height, int speed, BulletDirection direct) {
        // Вызов конструктора суперкласса Entity с указанием ширины и высоты снаряда
        super(width, height);
        // Расчет начальной позиции снаряда относительно положения стреляющего корабля
        int bulletX = shooter.getX() + shooter.getWidth() / 2 - width / 2;
        int bulletY, speedY;
        // Определение направления полета снаряда и вычисление начальной скорости по оси Y
        if (direct == BulletDirection.UP) {
            // Если снаряд летит вверх, то его начальная координата Y будет выше стреляющего корабля
            bulletY = shooter.getY() - height;
            speedY = -speed;
        } else {
            bulletY = shooter.getY() + shooter.getHeight();
            speedY = speed;
        }
        // Установка начальной позиции снаряда
        setX(bulletX);
        setY(bulletY);
        // Установка начальной скорости снаряда по оси Y
        setSpeedY(speedY);
    }
}
