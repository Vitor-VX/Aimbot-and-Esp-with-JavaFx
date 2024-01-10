package com.victor.vx;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

public class EntryPoint extends Application {

    private static Stage stage;
    private static Scene scene;
    private static double xOffset = 0;
    private static double yOffset = 0;

    @Override
    public void start(Stage primaryStage) throws Exception {
        stage = primaryStage;
        setConfigsWindows("", false, true);
    }

    public static void switchScreen(String fxml) {
        try {
            Parent rootParent = FXMLLoader.load(Main.class.getResource("/fxml/" + fxml + ".fxml"));
            scene = new Scene(rootParent);

            rootParent.setOnMousePressed(event -> {
                xOffset = event.getSceneX();
                yOffset = event.getSceneY();
            });

            rootParent.setOnMouseDragged(event -> {
                stage.setX(event.getScreenX() - xOffset);
                stage.setY(event.getScreenY() - yOffset);
            });

            stage.setScene(scene);
        } catch (Exception e) {
            handleException("Erro ao tentar trocar para a tela \"" + fxml + "\"", e);
        }
    }

    public static void setConfigsWindows(String tittleWindow, boolean isMenuVisible, boolean isWindowVisible) {
        stage.setTitle(tittleWindow);

        if (!isMenuVisible) {
            stage.initStyle(StageStyle.UNDECORATED);
            stage.initStyle(StageStyle.TRANSPARENT); 
        }

        if (isWindowVisible) {
            if (scene == null) {
                switchScreen("Main");
            }
        }

        stage.setResizable(false);
        stage.show();
    }

    public static void handleException(String message, Exception e) {
        System.err.println(message + ": " + e.getMessage());
    }

    public static void main(String[] args) {
        launch(args);
    }
}
