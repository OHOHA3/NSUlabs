package Game.Painters;

import Game.GameObjects.DrawAble;

import java.awt.*;
import java.util.ArrayList;

public class Painter {
    public void draw(DrawAble model, Graphics g) {
        model.draw(g);
    }

    public void draw(ArrayList<? extends DrawAble> models, Graphics g) {
        int length = models.size();
        int i = 0;
        while (i < length) {
            if (!models.get(i).draw(g)) {
                models.remove(i);
                length--;
                i--;
            }
            i++;
        }
    }

    public void drawEndScreen(Graphics g, long minutes, int seconds) {
        String str = "You've survived " + minutes +
                " minutes " + seconds + " seconds";
        Font f = new Font("TimesRoman", Font.BOLD, 25);
        g.setColor(Color.white);
        g.setFont(f);
        g.drawString(str, 260, 240);
    }
}
