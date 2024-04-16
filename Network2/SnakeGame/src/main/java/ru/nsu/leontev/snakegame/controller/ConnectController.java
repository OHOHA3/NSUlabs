package ru.nsu.leontev.snakegame.controller;

import javafx.application.Platform;
import javafx.beans.property.ListProperty;
import javafx.beans.property.SimpleListProperty;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ListView;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import ru.nsu.leontev.snakegame.model.events.EventListener;
import ru.nsu.leontev.snakegame.model.GameModel;
import ru.nsu.leontev.snakegame.model.announcement.IpAddress;
import ru.nsu.leontev.snakegame.model.SnakesProto.GameMessage.*;


import java.io.IOException;
import java.net.InetAddress;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.ResourceBundle;

public class ConnectController implements Initializable, EventListener {
    private GameModel gameModel = GameModel.getInstance();

    private Alert alert;

    private ListProperty<Text> availableGamesListProperty = new SimpleListProperty<>();

    @FXML
    private ListView<Text> availableGamesListView;

    public ConnectController() throws IOException {
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        gameModel.getEventManager().subscribe(this, "availableGames", "message");

        availableGamesListView.itemsProperty().bind(availableGamesListProperty);

        alert = new Alert(Alert.AlertType.WARNING);
        alert.setHeaderText(null);
        alert.setTitle("Warning");
    }

    @Override
    public void update(String eventType, Map<IpAddress, AnnouncementMsg> availableGames) {
        Platform.runLater(() -> {
            refreshGamesListView(availableGames);
        });
    }

    @Override
    public void update(String eventType, String message) {
        Platform.runLater(() -> {
            alert.setContentText(message);
            alert.showAndWait();
        });
    }

    private void refreshGamesListView(Map<IpAddress, AnnouncementMsg> availableGames) {
        List<Text> gamesTextList = new ArrayList<>();

        for (Map.Entry<IpAddress, AnnouncementMsg> game : availableGames.entrySet()) {
            Text gameInfoText = new Text(game.getValue().getPlayers().getPlayers(0).getIpAddress() + " " +
                    game.getValue().getPlayers().getPlayers(0).getPort() + " " +
                    game.getValue().getPlayers().getPlayers(0).getName() +
                    game.getValue().getPlayers().getPlayersList().size() + " " +
                    game.getValue().getConfig().getWidth() + "x" +
                    game.getValue().getConfig().getHeight() + " " +
                    game.getValue().getConfig().getFoodStatic() + "+" +
                    game.getValue().getConfig().getFoodPerPlayer() + "x");

            gameInfoText.setFill(Color.rgb(66, 100, 139));
            gamesTextList.add(gameInfoText);
        }
        availableGamesListProperty.setValue(FXCollections.observableArrayList(gamesTextList));
    }

    public void listElementClicked(MouseEvent event) throws IOException, InterruptedException {
        String gameIpAddress = availableGamesListView.getSelectionModel().getSelectedItem().getText().split(" ")[0];
        String gamePort = availableGamesListView.getSelectionModel().getSelectedItem().getText().split(" ")[1];

        gameModel.stopReceivingAnnouncementMessage();

        gameModel.sendMessage(InetAddress.getByName(gameIpAddress), Integer.parseInt(gamePort),
                gameModel.createJoinMsg());

        gameModel.getEventManager().unsubscribe(this, "availableGames", "message");

        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/ru/nsu/leontev/snakegame/fxml/game.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage) ((Node) event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }

    public void backButtonPressed(MouseEvent event) throws IOException {
        gameModel.stopReceivingAnnouncementMessage();
        gameModel.getAvailableGames().clear();

        Parent gameViewParent = FXMLLoader.load(getClass().getResource("/ru/nsu/leontev/snakegame/fxml/menu.fxml"));

        Scene gameViewScene = new Scene(gameViewParent);

        Stage window = (Stage) ((Node) event.getSource()).getScene().getWindow();

        window.setScene(gameViewScene);
        window.show();
    }
}
