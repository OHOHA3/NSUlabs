package Game.GameObjects;


import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;

public class Player implements DrawAble {
    private int x;
    private int y;
    int tick = 0;
    final private int width = 80;
    private int height = 80;
    int jumpHeight = 0;
    int jumpDirection = 0;
    Image image;
    boolean left = false;
    boolean right = false;
    boolean jump = false;
    boolean leftDash = false;
    boolean rightDash = false;
    boolean seat = false;
    int seatFlag = 0;

    public Player(String imageName) {
        x = 450;
        y = 400;
        image = new ImageIcon(imageName).getImage();
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public boolean isLeftDash() {
        return leftDash;
    }

    public boolean isRightDash() {
        return rightDash;
    }

    public void setLeft(boolean left) {
        this.left = left;
    }

    public void setRight(boolean right) {
        this.right = right;
    }

    public void setJump(boolean jump) {
        this.jump = jump;
    }

    public void setLeftDash(boolean leftDash) {
        this.leftDash = leftDash;
    }

    public void setRightDash(boolean rightDash) {
        this.rightDash = rightDash;
    }

    public void setSeat(boolean seat) {
        this.seat = seat;
    }

    public boolean draw(Graphics g) {
        g.drawImage(image, x, y, width, height, null);
        return true;
    }

    public void move() {
        if (seat) {
            if (seatFlag == 0) {
                seatFlag++;
                height /= 2;
                y += height;
            }
        } else {
            if (seatFlag == 1) {
                seatFlag--;
                y -= height;
                height *= 2;
            }
            if (leftDash) {
                if (tick < 12) {
                    tick++;
                    if (x > 0) {
                        x -= 10;
                    }
                } else {
                    tick = 0;
                    leftDash = false;
                }
            } else if (rightDash) {
                if (tick < 12) {
                    tick++;
                    if (x < 960 - 80) {
                        x += 10;
                    }
                } else {
                    tick = 0;
                    rightDash = false;
                }
            } else {
                if (left) {
                    if (x > 0) {
                        x -= 5;
                    }
                }
                if (right) {
                    if (x < 960 - 80) {
                        x += 5;
                    }
                }
            }
        }
        if (jump) {
            if (jumpDirection == 0) {
                jumpDirection = 1;
            }
        }
        if (jumpDirection == 1) {
            if (jumpHeight < height * 2) {
                y -= 7;
                jumpHeight += 7;
            } else {
                jumpDirection = -1;
            }
        }
        if (jumpDirection == -1) {
            if (jumpHeight > 0) {
                y += 7;
                jumpHeight -= 7;
            } else {
                jumpDirection = 0;
            }
        }

    }

}
