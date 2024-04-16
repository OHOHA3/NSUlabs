package Game.Screen;

import Game.Engine.Engine;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class GameField extends JPanel implements ActionListener {
    private Timer timer;
    Engine engine;

    public GameField() {
        engine = new Engine(this);
        timer = new Timer(10, this);
        timer.start();
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        engine.render(g);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        repaint();
        engine.update();
    }
}
