package ru.nsu.leontev.snakegame;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class MainFX extends Application {
    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource(("/ru/nsu/leontev/snakegame/fxml/menu.fxml")));
        primaryStage.setScene(new Scene(root, 960, 600));
        primaryStage.setResizable(false);
        primaryStage.show();
    }
}