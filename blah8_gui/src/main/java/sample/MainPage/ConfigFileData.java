package sample.MainPage;

import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.TextField;
import javafx.scene.paint.Color;
import sample.HexpadControls.HexpadControls;
import sample.Resolution.Resolution;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.StringJoiner;
import java.util.stream.Stream;

public class ConfigFileData
{
    private String gamePath;
    private int resWidth;
    private int resHeight;
    private int fullScreen;
    private String backgroundColor;
    private String objectColor;
    private String keyCodeString;
    private String buttonValueString;

    private boolean configExist;

    private static String FILENAME = "options.config";
    private static String CONFIG_FILE_DELIMITER = "\n";

    public ConfigFileData()
    {
        String[] arr = new String[2];

        configExist = false;

        Path configFile = Paths.get(FILENAME);
        if(configFile.toFile().exists())
        {
            configExist = true;

            try (Stream<String> stream = Files.lines(Paths.get(FILENAME)))
            {
                arr =  stream.toArray(String[]::new);
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }

            this.gamePath = arr[0];
            this.resWidth = Integer.decode(arr[1]);
            this.resHeight = Integer.decode(arr[2]);
            this.fullScreen = Integer.decode(arr[3]);
            this.objectColor = arr[4];
            this.backgroundColor = arr[5];
            this.keyCodeString = arr[6];
            this.buttonValueString = arr[7];
        }
    }

    public void setGamePath(String gamePath)
    {
        this.gamePath = gamePath;
    }

    public void setResWidth(int resWidth)
    {
        this.resWidth = resWidth;
    }

    public void setResHeight(int resHeight)
    {
        this.resHeight = resHeight;
    }

    public void setFullScreen(boolean fullScreen)
    {
        this.fullScreen = fullScreen ? 1 : 0;
    }

    public void setBackgroundColor(String backgroundColor)
    {
        this.backgroundColor = backgroundColor;
    }

    public void setObjectColor(String objectColor)
    {
        this.objectColor = objectColor;
    }

    public void setKeyCodeString(String keyCodeString)
    {
        this.keyCodeString = keyCodeString;
    }

    public void setButtonValueString(String buttonValueString)
    {
        this.buttonValueString = buttonValueString;
    }

    public void writeConfigFile()
    {
        StringJoiner joiner = new StringJoiner(CONFIG_FILE_DELIMITER);
        joiner.add(gamePath)
                .add(Integer.toString(resWidth))
                .add(Integer.toString(resHeight))
                .add(Integer.toString(fullScreen))
                .add(objectColor)
                .add(backgroundColor)
                .add(keyCodeString);

        ArrayList<String> configs = new ArrayList<>(2);
        configs.add(joiner.toString());
        configs.add(buttonValueString);

        Path configFile = Paths.get(FILENAME);
        try
        {
            Files.write(configFile, configs);
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    public void setDefaultValues(TextField filePathField, ChoiceBox<Resolution> resolutionBox,
                                 ColorPicker backgroundColorPicker, ColorPicker objectColorPicker,
                                 CheckBox fullscreenCheckBox, HexpadControls hexpadControls)
    {
        filePathField.setText(gamePath);
        resolutionBox.setValue(new Resolution(resWidth, resHeight));
        objectColorPicker.setValue(Color.web(objectColor));
        backgroundColorPicker.setValue(Color.web(backgroundColor));
        fullscreenCheckBox.setSelected(fullScreen == 1);

        String[] buttonValues = buttonValueString.split(HexpadControls.VALUE_DELIMITER);
        int[] keyCodes = Arrays.stream(keyCodeString.split(HexpadControls.KEYCODE_DELIMITER))
                .mapToInt(Integer::parseInt).toArray();
        hexpadControls.setConfigButtons(keyCodes, buttonValues);
    }

    public boolean isConfigExist()
    {
        return configExist;
    }


}
