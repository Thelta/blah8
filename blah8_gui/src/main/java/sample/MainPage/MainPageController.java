package sample.MainPage;

import javafx.application.Platform;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import org.jnativehook.GlobalScreen;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import sample.HexpadControls.HexpadButton;
import sample.HexpadControls.HexpadControls;
import sample.App;
import sample.Resolution.Resolution;
import sample.Resolution.ResolutionService;

import java.io.File;
import java.io.IOException;

public class MainPageController implements NativeKeyListener
{

    @FXML
    private App main;

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

    @FXML
    private Button buttonChanger;

    @FXML
    private ChoiceBox<HexpadButton> buttonSelector;

    private HexpadControls hexpadControls;

    private ConfigFileData configFileData;

    private static String EMULATOR_NAME = "blah8";

    public void setMain(App main)
    {
        this.main = main;
    }

    @FXML
    private void initialize()
    {
        hexpadControls = new HexpadControls();

        //Init ConfigFileData and if there is a config.file set values to that.
        configFileData = new ConfigFileData();
        if(configFileData.isConfigExist())
        {
            configFileData.setDefaultValues(filePathField, resolutionBox, backgroundColorPicker,
                    objectColorPicker, fullscreenCheckBox, hexpadControls);
        }

        //get possible resolutions and set them at resolutionBox
        ResolutionService rs = new ResolutionService();
        ObservableList<Resolution> resolutions = rs.getObservableResolutions();
        resolutionBox.setItems(resolutions);
        resolutionBox.setValue(resolutions.get(resolutions.size() - 1));

        ObservableList<HexpadButton> observableHexpadButtons= hexpadControls.getObservableHexpadButtons();
        buttonSelector.setItems(observableHexpadButtons);
        buttonSelector.getSelectionModel().selectFirst();
        changeButtonChangerValue();
        buttonSelector.setOnAction(event -> {
            changeButtonChangerValue();
        });


        buttonChanger.setOnAction(event -> {
            GlobalScreen.addNativeKeyListener(this);
        });

        //set file chooser event
        filePickButton.setOnAction(event -> {
            Stage stage = Stage.class.cast(Control.class.cast(event.getSource()).getScene().getWindow());
            FileChooser fileChooser = new FileChooser();
            fileChooser.setInitialDirectory(new File("./"));
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
                setConfigFileData();
                configFileData.writeConfigFile();

                try
                {
                    Runtime.getRuntime().exec("./" + EMULATOR_NAME);
                }
                catch (IOException e)
                {
                    Alert alert = new Alert(Alert.AlertType.ERROR);
                    alert.setTitle("Can't Run Emulator.");
                    alert.setContentText("Can't find emulator.Please put blah8 executable to same folder in ui executable.");
                    alert.show();
                }
            }
            else
            {
                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setTitle("Can't Run Emulator.");
                alert.setContentText("File does not exist.");
                alert.show();
            }

        });
    }


    private void changeButtonChangerValue()
    {
        int buttonIndex = buttonSelector.getSelectionModel().getSelectedIndex();
        buttonChanger.setText(hexpadControls.getButtonValue(buttonIndex));

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

    private void setConfigFileData()
    {
        configFileData.setGamePath(filePathField.getText());
        configFileData.setBackgroundColor(backgroundColorPicker.getValue().toString());
        configFileData.setObjectColor(objectColorPicker.getValue().toString());
        configFileData.setFullScreen(fullscreenCheckBox.isSelected());
        configFileData.setResWidth(resolutionBox.getValue().getWidth());
        configFileData.setResHeight(resolutionBox.getValue().getHeight());
        configFileData.setKeyCodeString(hexpadControls.getConfigKeycodeString());
        configFileData.setButtonValueString(hexpadControls.getConfigValueString());
    }

    @Override
    public void nativeKeyTyped(NativeKeyEvent nativeKeyEvent)
    {

    }

    @Override
    public void nativeKeyPressed(NativeKeyEvent nativeKeyEvent)
    {
        int buttonIndex = buttonSelector.getSelectionModel().getSelectedIndex();
        boolean isFailed;
        isFailed = hexpadControls.changeButton(buttonIndex,
                                                nativeKeyEvent.getKeyCode(),
                                                nativeKeyEvent.getKeyText(nativeKeyEvent.getKeyCode()));
        GlobalScreen.removeNativeKeyListener(this);

        Platform.runLater(new Runnable()
        {
            @Override
            public void run()
            {
                buttonSelector.requestFocus();
                changeButtonChangerValue();

                if (!isFailed)
                {
                    Alert alert = new Alert(Alert.AlertType.ERROR);
                    alert.setTitle("About Key Change");
                    alert.setContentText("That key is assigned to another button.");
                    alert.show();
                }

            }
        });
    }

    @Override
    public void nativeKeyReleased(NativeKeyEvent nativeKeyEvent)
    {
    }
}
