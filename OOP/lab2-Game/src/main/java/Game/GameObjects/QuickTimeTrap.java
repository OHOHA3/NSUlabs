package Game.GameObjects;

import javax.swing.*;
import java.awt.*;

public class QuickTimeTrap implements DrawAble {
    Image firstPart;
    Image secondPart;
    private final int DRONEWIDTH = 80;
    private final int DRONEHIGH = 80;
    private final int RAYWIDTH = 20;
    private final int RAYHIGH = 540;
    int x;
    int time = 0;

    public QuickTimeTrap(String firstPartName, String secondPartName, int x) {
        firstPart = new ImageIcon(firstPartName).getImage();
        secondPart = new ImageIcon(secondPartName).getImage();
        this.x = x;
    }

    public boolean draw(Graphics g) {
        if (time < 80) {
            g.drawImage(firstPart, x - 3, 0, DRONEWIDTH, DRONEHIGH, null);
            g.drawImage(secondPart, x + 30, 60, RAYWIDTH, RAYHIGH, null);
            return true;
        }
        if (time < 160) {
            g.drawImage(firstPart, x - 3, 0, DRONEWIDTH, DRONEHIGH, null);
            g.drawImage(secondPart, x + 10, 60, RAYWIDTH * 3, RAYHIGH, null);
            return true;
        }
        return false;
    }

    public void move() {
        time++;
    }

    public boolean checkCollision(int playerX) {
        double distance = Math.abs(playerX - x);
        if (time > 80 && time < 160) {
            if (distance < 35) {
                return true;
            }
        }
        return false;
    }
}
