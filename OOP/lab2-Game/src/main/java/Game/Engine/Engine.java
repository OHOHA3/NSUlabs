package Game.Engine;

import Game.Controllers.*;
import Game.GameObjects.*;
import Game.Painters.Painter;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;

public class Engine extends JPanel {
    Background background;
    Player player;
    ArrayList<FlyingTrap> flyingTraps;
    ArrayList<HomingTrap> homingTraps;
    ArrayList<HorisontalTrap> horisontalTraps;
    ArrayList<MovingBack> movingBacks;
    ArrayList<QuickTimeTrap> quickTimeTraps;

    Game.Painters.Painter painter;

    PlayerController playerController;
    MovingBackController movingBackController;
    HorisontalTrapController horisontalTrapController;
    FlyingTrapController flyingTrapController;
    QuickTimeTrapController quickTimeTrapController;
    HomingTrapController homingTrapController;
    Instant startGameTime;
    GameStatus gameStatus;
    long minutes;
    int seconds;
    int stopFlag;
    int endCounter;
    JButton button1;
    JButton button2;
    JPanel panel;

    public Engine(JPanel panel) {
        panel.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                playerController.checkMove(e, player);
            }

            public void keyReleased(KeyEvent e) {
                playerController.checkStop(e, player);
            }
        });
        this.panel = panel;
        panel.setFocusable(true);
        panel.setLayout(null);
        painter = new Painter();
        playerController = new PlayerController();
        movingBackController = new MovingBackController("textures/boingRight.png", "textures/boingLeft.png");
        horisontalTrapController = new HorisontalTrapController("textures/horisontRight.png", "textures/horisontLeft.png");
        flyingTrapController = new FlyingTrapController("textures/bombRight.png", "textures/bombLeft.png", "textures/bombDown.png");
        quickTimeTrapController = new QuickTimeTrapController("textures/drone.png", "textures/ray.png");
        homingTrapController = new HomingTrapController("textures/homingTrap.png");
        gameStatus = GameStatus.MENU;
    }

    public void update() {
        switch (gameStatus) {
            case MENU:
                button1 = new JButton();
                button1.setText("START");
                button1.setSize(160, 90);
                button1.setLocation(400, 100);
                button1.addActionListener(e -> {
                    panel.removeAll();
                    gameStatus = GameStatus.INITGAME;
                });
                panel.add(button1);
                button2 = new JButton();
                button2.setText("QUIT");
                button2.setSize(160, 90);
                button2.setLocation(400, 250);
                button2.addActionListener(e -> System.exit(0));
                panel.add(button2);
                break;
            case INITGAME:
                background = new Background("textures/background.png");
                player = new Player("textures/player.png");
                flyingTraps = new ArrayList<>();
                homingTraps = new ArrayList<>();
                horisontalTraps = new ArrayList<>();
                movingBacks = new ArrayList<>();
                quickTimeTraps = new ArrayList<>();
                startGameTime = Instant.now();
                gameStatus = GameStatus.GAME;
                stopFlag = 0;
                endCounter = 0;
                break;
            case GAME:
                playerController.move(player);
                movingBackController.create(movingBacks);
                movingBackController.move(movingBacks);
                horisontalTrapController.create(horisontalTraps);
                horisontalTrapController.move(horisontalTraps);
                flyingTrapController.create(flyingTraps, player.getX());
                flyingTrapController.move(flyingTraps);
                quickTimeTrapController.create(quickTimeTraps);
                quickTimeTrapController.move(quickTimeTraps);
                homingTrapController.create(homingTraps);
                homingTrapController.move(homingTraps, player.getX(), player.getY());
                if (flyingTrapController.checkCollisions(flyingTraps, player.getX(), player.getY())) {
                    gameStatus = GameStatus.ENDSCREEN;
                }
                if (homingTrapController.checkCollisions(homingTraps, player.getX(), player.getY())) {
                    gameStatus = GameStatus.ENDSCREEN;
                }
                if (horisontalTrapController.checkCollisions(horisontalTraps, player.getX(), player.getY())) {
                    gameStatus = GameStatus.ENDSCREEN;
                }
                if (quickTimeTrapController.checkCollisions(quickTimeTraps, player.getX())) {
                    gameStatus = GameStatus.ENDSCREEN;
                }
                break;
            case ENDSCREEN:
                if (stopFlag == 0) {
                    Instant endGameTime = Instant.now();
                    minutes = Duration.between(startGameTime, endGameTime).toMinutes();
                    seconds = Duration.between(startGameTime, endGameTime).toSecondsPart();
                    stopFlag++;
                }
                endCounter++;
                if (endCounter > 250) {
                    gameStatus = GameStatus.MENU;
                }
                break;
        }
    }

    public void render(Graphics g) {
        switch (gameStatus) {
            case MENU:
                panel.setBackground(Color.black);
                break;
            case GAME:
                painter.draw(background, g);
                painter.draw(player, g);
                painter.draw(movingBacks, g);
                painter.draw(homingTraps, g);
                painter.draw(quickTimeTraps, g);
                painter.draw(flyingTraps, g);
                painter.draw(horisontalTraps, g);
                break;
            case ENDSCREEN:
                painter.drawEndScreen(g, minutes, seconds);
                break;
        }
    }
}
