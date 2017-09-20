package sample;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.awt.*;
import java.util.ArrayList;

public class ResolutionService
{
    private ArrayList<Resolution> resolutions;

    public ResolutionService()
    {
        resolutions = new ArrayList<>();

        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        GraphicsDevice gd = ge.getDefaultScreenDevice();
        DisplayMode[] modes = gd.getDisplayModes();

        DisplayMode prevMode = null;
        for(DisplayMode dm : modes)
        {
            if(!dm.equals(prevMode))
            {
                resolutions.add(new Resolution(dm.getWidth(), dm.getHeight()));
                prevMode = dm;
            }
        }
    }

    public ObservableList<Resolution> getObservableResolutions()
    {
        return FXCollections.observableArrayList(resolutions);
    }


}
