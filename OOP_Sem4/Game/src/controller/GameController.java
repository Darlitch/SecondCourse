package controller;

import model.*;
import model.bullets.Bullet;
import model.enemies.EnemyDefault;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;
import java.util.List;

public class GameController implements KeyListener, TickerListener {
    private int score = 0;
    public static final int fieldWidth = 1000;
    public static final int fieldHeight = 600;

    private final Player player;
    private List<Ship> ships;
    private List<Bullet> bullets;
    private GameState gameState = GameState.RUN; // Состояние игры
    private final GameDifficulty difficulty;

    private ModelListener modelListener;
    private final Ticker ticker; // Таймер

    public GameController(GameDifficulty difficulty) {
        this.difficulty = difficulty;
        //интервал между выстрелами игрока
        int shootTimeout = switch (difficulty) {
            case LEVEL1 -> 700;
            case LEVEL2 -> 900;
            case LEVEL3 -> 1100;
        };

        player = new Player(fieldWidth / 2 - Player.playerWidth / 2,
                fieldHeight - Player.playerHeight, shootTimeout);

        ships = new ArrayList<>();
        bullets = new ArrayList<>();
        spawnEnemies();
        ships.add(player);

        ticker = new Ticker();
        ticker.setTickerListener(this);
        ticker.start();
    }

    public void spawnEnemies() {
        int offset = 25;
        int gap = 15;
        int enemyWidth = EnemyDefault.enemyDefWidth + gap;

        int shootFreqModifier = switch (difficulty) {
            case LEVEL1 -> 1500;
            case LEVEL2 -> 800;
            case LEVEL3 -> 400;
        };
        // Создание врагов в ряд по краям игрового поля
        for (int i = offset; i < (fieldWidth - offset); i += enemyWidth) {
            ships.add(new EnemyDefault(i, 10, shootFreqModifier));
        }
    }
    // Метод для обновления состояния игры
    public void update() {
        if (gameState == GameState.DEAD) {
            notifyListener(); // уведомляем слушателя
            return;
        }

        checkCollisions(); // Проверка столкновений

        if (canMove(player)) player.move(); // перемещаем игрока, если это возможно
        int speedModifier = moveEnemies() ? 1 : -1; // направление движения врагов
        // Перемешение врагов и выстрелы
        for (Ship ship : ships) {
            if (ship == player) continue;
            ship.setSpeedX(ship.getSpeedX() * speedModifier);
            ship.move();

            Bullet bullet = ship.shoot();
            if (bullet != null) {
                bullets.add(bullet);
            }
        }
        // Если все уничтожены, то восстанавливаем ХП и спавним новых
        if (ships.size() == 1) {
            spawnEnemies();
            player.increaseHealth();
        }
        notifyListener(); // уведомляем слушателя
    }

    private void checkCollisions() {
        List<Bullet> bulletsCopy = new ArrayList<>(bullets);
        List<Ship> shipsCopy = new ArrayList<>(ships);

        for (Bullet bullet : bullets) {
            for (Ship ship : ships) {
                if (!bulletsCopy.contains(bullet) || !ships.contains(ship) ||
                        !bullet.collide(ship)) continue;
                ship.decreaseHealth();

                bulletsCopy.remove(bullet);
                if (ship.getHealth() == 0) {
                    if (ship == player) {
                        gameState = GameState.DEAD;
                        return;
                    }
                    shipsCopy.remove(ship);
                    score += switch (difficulty) {
                        case LEVEL1 -> 10;
                        case LEVEL2 -> 15;
                        case LEVEL3 -> 20;
                    };
                }
            }
            moveBullet(bullet, bulletsCopy);
        }
        ships = shipsCopy;
        bullets = bulletsCopy;
    }

    private boolean canMove(Ship ship) {
        int newX = ship.getX() + ship.getSpeedX();
        int newY = ship.getY() + ship.getSpeedY();
        return 0 <= newX && newX + ship.getWidth() <= fieldWidth
                && 0 <= newY && newY + ship.getHeight() <= fieldHeight;
    }

    private boolean moveEnemies() {
        boolean allCanMove = true;
        for (Ship ship : ships) {
            if (ship == player) continue;
            allCanMove = allCanMove && canMove(ship);
        }
        return allCanMove;
    }

    private void moveBullet(Bullet bullet, List<Bullet> bulletsList) {
        bullet.move();
        int bottomY = bullet.getY() + bullet.getHeight();
        if (bottomY < 0 || bullet.getY() >= fieldHeight)
            bulletsList.remove(bullet);
    }

    public Player getPlayer() {
        return player;
    }

    public GameState getGameState() {
        return gameState;
    }

    public List<Ship> getShips() {
        return ships;
    }

    public List<Bullet> getBullets() {
        return bullets;
    }

    public int getScore() {
        return score;
    }

    public void setModelListener(ModelListener modelListener) {
        this.modelListener = modelListener;
    }

    private void notifyListener() {
        if (modelListener != null)
            modelListener.modelChange();
    }

    public void playerShoot() {
        Bullet newBullet = player.shoot();
        if (newBullet == null) return;
        bullets.add(newBullet);
    }

    @Override
    public void keyTyped(KeyEvent e) {}

    @Override
    public void keyPressed(KeyEvent e) {
        switch (e.getKeyCode()) {
            case KeyEvent.VK_RIGHT:
                player.setSpeedX(Player.playerSpeed);
                break;
            case KeyEvent.VK_LEFT:
                player.setSpeedX(-Player.playerSpeed);
                break;
            case KeyEvent.VK_SPACE:
                playerShoot();
                break;
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_LEFT ||
                e.getKeyCode() == KeyEvent.VK_RIGHT)
            player.setSpeedX(0);
    }

    @Override
    public void ticked() {
        update();
    }
}