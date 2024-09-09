package view;

import javax.swing.*;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            StartUpScreen start = new StartUpScreen();
        });
    }
}
