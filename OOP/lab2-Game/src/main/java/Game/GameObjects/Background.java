package Game.GameObjects;

import Game.Screen.Scale;

import javax.swing.*;
import java.awt.*;

public class Background implements DrawAble {
    Image image;
    double scale = Scale.getInstance().getScale();
    private final int WIDTH = (int)(960*scale);
    private final int HIGH = (int)(540*scale);

    public Background(String imageName) {
        image = new ImageIcon(imageName).getImage();
    }

    public boolean draw(Graphics g) {
        g.drawImage(image, 0, 0, WIDTH, HIGH, null);
        return true;
    }
}
