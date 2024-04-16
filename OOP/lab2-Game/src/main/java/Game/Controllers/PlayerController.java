package Game.Controllers;

import Game.GameObjects.Player;

import java.awt.event.KeyEvent;

public class PlayerController {
    public void checkMove(KeyEvent e, Player player) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_A || key == KeyEvent.VK_LEFT) {
            player.setLeft(true);
        }
        if (key == KeyEvent.VK_D || key == KeyEvent.VK_RIGHT) {
            player.setRight(true);
        }
        if (key == KeyEvent.VK_W || key == KeyEvent.VK_UP || key == KeyEvent.VK_SPACE) {
            player.setJump(true);
        }
        if (key == KeyEvent.VK_S || key == KeyEvent.VK_DOWN) {
            player.setSeat(true);
        }
        if (key == KeyEvent.VK_Q) {
            if (!player.isRightDash()) {
                player.setLeftDash(true);
            }
        }
        if (key == KeyEvent.VK_E) {
            if (!player.isLeftDash()) {
                player.setRightDash(true);
            }
        }
    }

    public void checkStop(KeyEvent e, Player player) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_A || key == KeyEvent.VK_LEFT) {
            player.setLeft(false);
        }
        if (key == KeyEvent.VK_D || key == KeyEvent.VK_RIGHT) {
            player.setRight(false);
        }
        if (key == KeyEvent.VK_W || key == KeyEvent.VK_UP || key == KeyEvent.VK_SPACE) {
            player.setJump(false);
        }
        if (key == KeyEvent.VK_S || key == KeyEvent.VK_DOWN) {
            player.setSeat(false);
        }
    }

    public void move(Player player) {
        player.move();
    }
}
