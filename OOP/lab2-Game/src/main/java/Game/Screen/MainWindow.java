package Game.Screen;

import javax.swing.*;
import java.awt.*;

public class MainWindow extends JFrame {
    public MainWindow() {
        setTitle("Game");
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setResizable(false);
        Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
        Scale.getInstance().setSkale(size.height);
        int startWidth = 960;
        int startHeight = 540;
        setSize((int) (startWidth * Scale.getInstance().getScale()), (int) (startHeight * Scale.getInstance().getScale()));
        setLocationRelativeTo(null);
        add(new GameField());
        setVisible(true);
    }

    public static void main(String[] args) {
        new MainWindow();
    }
}