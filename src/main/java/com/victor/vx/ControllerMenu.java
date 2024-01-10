package com.victor.vx;

import javafx.concurrent.Task;
import javafx.fxml.FXML;
import javafx.scene.control.Button;

public class ControllerMenu {

    private LibraryFunctions libraryFunctions = LibraryFunctions.INSTANCE;

    @FXML
    private Button ativarFuncoes;
    @FXML
    private Button aimbotAutomatic;
    @FXML
    private Button aimbotFromShot;
    @FXML
    private Button norecoil;
    @FXML
    private Button balletInfity;
    @FXML
    private Button espActivate;
    @FXML
    private Button close;
    @FXML
    private Button espCima;
    @FXML
    private Button espBaixo;

    @FXML
    public void handleAtivarFuncoes() {
        handleFeature(ativarFuncoes, libraryFunctions.StopHacking());

        Task<Void> task = new Task<Void>() {
            @Override
            protected Void call() throws Exception {
                libraryFunctions.ModFunctions();
                return null;
            }
        };

        new Thread(task).start();
    }

    @FXML
    public void handleAimbotAutomatic() {
        handleFeature(aimbotAutomatic, libraryFunctions.StopAimbotAutomatic());
    }

    @FXML
    public void handleAimbotFromShot() {
        handleFeature(aimbotFromShot, libraryFunctions.StopAimbotFromShot());
    }

    @FXML
    public void handleNoRecoil() {
        handleFeature(norecoil, libraryFunctions.StopNoRecoil());
    }

    @FXML
    public void handleBalletInfity() {
        handleFeature(balletInfity, libraryFunctions.StopBalletInfity());
    }

    @FXML
    public void handleEspAtivar() {
        handleFeature(espActivate, libraryFunctions.EspLinhaActivate());
    }

    @FXML
    public void handleEspCima() {
        handleFeature(espCima, libraryFunctions.EspType(1));
    }

    @FXML
    public void handleEspBaixo() {
        handleFeature(espBaixo, libraryFunctions.EspType(2));
    }

    private void handleFeature(Button button, Object stopParameter) {
        if (stopParameter instanceof Boolean) {
            boolean stopValue = (Boolean) stopParameter;
            if (stopValue) {
                button.setStyle("-fx-background-color: #00ff00;-fx-text-fill: #000000;");
            } else {
                button.setStyle("-fx-background-color: null;");
            }
        } else {
            button.setStyle("-fx-background-color: null;");
        }
    }
}