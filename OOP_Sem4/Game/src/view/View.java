package view;

import model.GameState;
import controller.GameController;
import model.ModelListener;
import model.Player;
import model.Ship;
import model.bullets.Bullet;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class View extends JFrame implements ModelListener {
    private final GameController gameController;
    private final JLabel playerLabel;
    private final JLabel healthLabel;
    private final JLabel scoreLabel;
    private final JPanel enemiesPanel;
    private final JPanel bulletsPanel;

    private final ImageIcon enemyIcon;

    public View(GameController gameController) {
        super("Тут должно быть название");

        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setResizable(false);
        setLayout(null);
        setVisible(true);

        Insets border = getInsets(); // Получаем границы окна
        setSize(GameController.fieldWidth + border.left + border.right,
                GameController.fieldHeight + border.top + border.bottom);
        setLocationRelativeTo(null);

        getContentPane().setBackground(new Color(234, 234, 238));

        this.gameController = gameController;
        gameController.setModelListener(this);
        addKeyListener(gameController);

        getContentPane().setBackground(new Color(12, 12, 19));

        playerLabel = createPlayer();
        healthLabel = createHealth();
        scoreLabel = createScore();
        enemiesPanel = createPanel();
        bulletsPanel = createPanel();

        enemyIcon = setIcon("/enemy.png");
        placeEnemies();
        // Добавляем элементы интерфейса на панель содержимого окна
        add(playerLabel);
        add(enemiesPanel);
        add(bulletsPanel);
        add(healthLabel);
        add(scoreLabel);
        repaint();
    }

    private JLabel createPlayer() {
        ImageIcon playerIcon = setIcon("/player.png");
        JLabel label = new JLabel(playerIcon);
        Player player = gameController.getPlayer();

        label.setBounds(player.getX(), player.getY(),
                player.getWidth(), player.getHeight());
        return label;
    }

    private JLabel createHealth() {
        JLabel health = new JLabel();
        health.setText("Health: " + gameController.getPlayer().getHealth());
        health.setFont(new Font("Times New Roman", Font.BOLD, 20));
        health.setForeground(new Color(168, 228, 160)); // "бабушкины яблоки" :)
        health.setBounds(50, 500, 100, 30);
        return health;
    }

    private JLabel createScore() {
        JLabel score = new JLabel();
        score.setText("Health: " + gameController.getPlayer().getHealth());
        score.setFont(new Font("Times New Roman", Font.BOLD, 15));
        score.setForeground(new Color(125, 143, 232));
        score.setBounds(50, 520, 100, 30);
        return score;
    }

    private JPanel createPanel() {
        JPanel panel = new JPanel(null);
        panel.setBounds(0, 0, GameController.fieldWidth, GameController.fieldHeight);
        panel.setOpaque(false);
        return panel;
    }

    private void placeEnemies() {
        List<Ship> shipsCopy = new ArrayList<>(gameController.getShips());
        for (Ship ship : shipsCopy) {
            if (ship == gameController.getPlayer()) continue;
            // Создаем новый JLabel с иконкой врага
            JLabel enemyLabel = new JLabel(enemyIcon);
            enemyLabel.setBounds(ship.getX(), ship.getY(),
                    ship.getWidth(), ship.getHeight());
            enemiesPanel.add(enemyLabel);
        }
    }

    private void updateEnemies() {
        enemiesPanel.removeAll();
        placeEnemies();
    }

    private void placeBullets() {
        List<Bullet> bulletsCopy = new ArrayList<>(gameController.getBullets());
        for (Bullet bullet : bulletsCopy) {
            JLabel bulletLabel = new JLabel();
            bulletLabel.setBounds(bullet.getX(), bullet.getY(),
                    bullet.getWidth(), bullet.getHeight());
            bulletLabel.setBackground(Color.WHITE);
            bulletLabel.setOpaque(true);

            bulletsPanel.add(bulletLabel);
        }
    }

    private void updateBullets() {
        bulletsPanel.removeAll();
        placeBullets();
    }

    private ImageIcon setIcon(String filename) {
        java.net.URL image = getClass().getResource(filename);
        if (image != null)
            return new ImageIcon(image);
        return null;
    }

    private void screenGameOver() {
        SwingUtilities.invokeLater(() -> {
            getContentPane().removeAll();

            JLabel gameOver = new JLabel();
            gameOver.setText("GAME OVER!");
            gameOver.setFont(new Font("Courier New", Font.BOLD, 40));
            gameOver.setForeground(Color.WHITE);
            gameOver.setBounds(400, 100, 500, 50);
            add(gameOver);

            JLabel finalScore = new JLabel();
            finalScore.setText("Your score: " + gameController.getScore());
            finalScore.setFont(new Font("Courier New", Font.ITALIC, 20));
            finalScore.setForeground(Color.WHITE);
            finalScore.setBounds(420, 150, 500, 50);
            add(finalScore);

            repaint();
        });
    }

    @Override
    public void modelChange() {
        if (gameController.getGameState() == GameState.DEAD) {
            screenGameOver();
            return;
        }

        SwingUtilities.invokeLater(() -> {
            Player player = gameController.getPlayer();
            playerLabel.setLocation(player.getX(), player.getY());
            updateEnemies();
            updateBullets();

            scoreLabel.setText("Score: " + gameController.getScore());
            healthLabel.setText("Health: " + player.getHealth());

            repaint();
        });
    }
}
