package sample;

import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.util.StringJoiner;

public class Controller
{

    @FXML
    private Main main;

    @FXML
    private Button filePickButton;

    @FXML
    private TextField filePathField;

    @FXML
    private Button runEmulatorButton;

    @FXML
    private ColorPicker backgroundColorPicker;

    @FXML
    private ColorPicker objectColorPicker;

    @FXML
    private ChoiceBox<Resolution> resolutionBox;

    @FXML
    private CheckBox fullscreenCheckBox;

    public void setMain(Main main)
    {
        this.main = main;
    }

    @FXML
    private void initialize()
    {
        filePathField.setText(null);
        //get possible resolutions and set them at resolutionBox
        ResolutionService rs = new ResolutionService();
        ObservableList<Resolution> resolutions = rs.getObservableResolutions();
        resolutionBox.setItems(resolutions);
        resolutionBox.setValue(resolutions.get(resolutions.size() - 1));

        //set file chooser event
        filePickButton.setOnAction(event -> {
            Stage stage = Stage.class.cast(Control.class.cast(event.getSource()).getScene().getWindow());
            FileChooser fileChooser = new FileChooser();
            fileChooser.setTitle("Select chip8 game file.");
            File gameFile =  fileChooser.showOpenDialog(stage);
            if(gameFile != null)
            {
                filePathField.setText(gameFile.getAbsolutePath());
            }
        });

        runEmulatorButton.setOnAction(event -> {
            if(validateFilePath(filePathField.getText()))
            {
                System.out.println(getEmulatorCommand());
            }
            else
            {
                System.out.println("No");
            }

        });
    }

    private boolean validateFilePath(String path)
    {

        try
        {
            File file = new File(path);
            if(file.exists() && !file.isDirectory())
            {
                return true;
            }
        }
        catch (NullPointerException e)
        {
            return false;
        }

        return false;
    }

    private String getEmulatorCommand()
    {
        StringJoiner joiner = new StringJoiner(" ");
        Resolution resolution = resolutionBox.getValue();
        joiner.add(filePathField.getText())
                .add(Integer.toString(resolution.getWidth()))
                .add(Integer.toString(resolution.getHeight()))
                .add(Integer.toString(fullscreenCheckBox.isIndeterminate() ? 1 : 0))
                .add(backgroundColorPicker.getValue().toString())
                .add(objectColorPicker.getValue().toString());

        return joiner.toString();
    }

}
