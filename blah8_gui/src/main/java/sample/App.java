package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeHookException;
import sample.MainPage.MainPageController;

import java.util.logging.Level;
import java.util.logging.LogManager;
import java.util.logging.Logger;

public class App extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {

        try {
            GlobalScreen.registerNativeHook();
            LogManager.getLogManager().reset();
            Logger logger = Logger.getLogger(GlobalScreen.class.getPackage().getName());
            logger.setLevel(Level.OFF);
        } catch (NativeHookException ex) {
            System.err.println("There was a problem registering the native hook.");
            System.err.println(ex.getMessage());
            System.exit(-1);
        }

        Font.loadFont(getClass().getResourceAsStream("/bitwise.ttf"), 25);

        FXMLLoader loader = new FXMLLoader(getClass().getResource("/sample.fxml"));
        Parent root = loader.load();
        MainPageController controller = loader.getController();
        controller.setMain(this);

        primaryStage.setTitle("blah8 Launcher");
        primaryStage.setResizable(false);
        primaryStage.setScene(new Scene(root, 380, 430));
        primaryStage.show();
    }

    @Override
    public void stop() {
        try {
            GlobalScreen.unregisterNativeHook();
        } catch (NativeHookException ex) {
            ex.printStackTrace();
        }
    }

    public static void main(String[] args) {
        launch(args);
    }

}
