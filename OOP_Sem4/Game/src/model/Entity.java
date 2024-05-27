package model;

public abstract class Entity {
    protected int speedX;
    protected int speedY;
    protected int x;
    protected int y;
    protected int width;
    protected int height;

    protected Entity(int x, int y, int width, int height, int speedX, int speedY) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.speedX = speedX;
        this.speedY = speedY;
    }

    protected Entity(int width, int height) {
        this.width = width;
        this.height = height;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y) {
        this.y = y;
    }

    public int getHeight() {
        return height;
    }

    public int getWidth() {
        return width;
    }

    public int getSpeedX() {
        return speedX;
    }

    public int getSpeedY() {
        return speedY;
    }

    public void setSpeedX(int speedX) {
        this.speedX = speedX;
    }

    public void setSpeedY(int speedY) {
        this.speedY = speedY;
    }

    public void move() {
        x += speedX;
        y += speedY;
    }
    //Метод для проверки столкновения с другим энтити
    public boolean collide(Entity other) {
        int rightX = x + width;
        int bottomY = y + height;
        int otherRightX = other.x + other.width;
        int otherBottomY = other.y + other.height;
        return rightX > other.x && otherRightX > x &&
                bottomY > other.y && otherBottomY > y;
    }
}