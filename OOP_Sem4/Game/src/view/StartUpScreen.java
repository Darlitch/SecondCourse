package view;

import model.GameDifficulty;
import controller.GameController;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class StartUpScreen extends JFrame implements ActionListener {
    public static final int START_SCREEN_WIDTH = 600;
    public static final int START_SCREEN_HEIGHT = 300;

    public StartUpScreen() {
        super("AntonLebedevGame(лень придумать название)");

        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setResizable(false);
        // Включаем, чтобы задавать размер каждого объекта вручную
        setLayout(null);

        setSize(START_SCREEN_WIDTH, START_SCREEN_HEIGHT);
        setLocationRelativeTo(null); // центрируем относительно экрана

        getContentPane().setBackground(new Color(12, 12, 19));

        setUpButtons();

        setVisible(true);
    }

    private void setUpButtons() {
        JButton level1 = new JButton("LEVEL 1");
        level1.setBounds(250, 50, 100, 30);
        level1.setFocusable(false);
        level1.addActionListener(this);
        level1.setActionCommand("level1");

        JButton level2 = new JButton("LEVEL 2");
        level2.setBounds(250, 100, 100, 30);
        level2.setFocusable(false);
        level2.addActionListener(this);
        level2.setActionCommand("level2");

        JButton level3 = new JButton("LEVEL 3");
        level3.setBounds(250, 150, 100, 30);
        level3.setFocusable(false);
        level3.addActionListener(this);
        level3.setActionCommand("level3");

        getContentPane().add(level1);
        getContentPane().add(level2);
        getContentPane().add(level3);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        GameDifficulty difficulty = switch (e.getActionCommand()) {
            case "level1" -> GameDifficulty.LEVEL1;
            case "level2" -> GameDifficulty.LEVEL2;
            default -> GameDifficulty.LEVEL3;
        };

        GameController gameController = new GameController(difficulty);
        //запускаем игру
        SwingUtilities.invokeLater(() -> {
            View view = new View(gameController);
            dispose();
        });
    }
}
