package ru.nsu.leontev.snakegame.controller;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;
import ru.nsu.leontev.snakegame.model.GameModel;

import java.io.IOException;

public class MenuController {
    private GameModel gameModel = GameModel.getInstance();

    public MenuController() throws IOException {
    }

    public void newGameButtonPressed(MouseEvent event) throws IOException {
        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/ru/nsu/leontev/snakegame/fxml/new_game.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage) ((Node) event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void connectButtonPressed(MouseEvent event) throws IOException {
        gameModel.receiveAnnouncementMessages();

        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/ru/nsu/leontev/snakegame/fxml/connect.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage) ((Node) event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void optionsButtonPressed(MouseEvent event) throws IOException {
        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/ru/nsu/leontev/snakegame/fxml/options.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage) ((Node) event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void exitButtonPressed(MouseEvent event) {
        Stage window = (Stage) ((Node) event.getSource()).getScene().getWindow();

        window.close();
    }

}
