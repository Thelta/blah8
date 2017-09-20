package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.text.Font;
import javafx.stage.Stage;

public class Main extends Application
{
    @Override
    public void start(Stage primaryStage) throws Exception
    {
        Font.loadFont(getClass().getResource("bitwise.ttf").toExternalForm(), 25);

        FXMLLoader loader = new FXMLLoader(getClass().getResource("sample.fxml"));
        Parent root = loader.load();
        Controller controller = loader.getController();
        controller.setMain(this);

        primaryStage.setTitle("blah8 Launcher");
        primaryStage.setResizable(false);
        primaryStage.setScene(new Scene(root, 330, 400));
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
