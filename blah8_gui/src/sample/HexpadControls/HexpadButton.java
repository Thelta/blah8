package sample.HexpadControls;

public class HexpadButton
{

    private String buttonHexpadSymbol;
    private int buttonKeyCode;
    private String buttonPressedValue;

    public HexpadButton(String buttonSymbol, String buttonValue, int buttonKeyCode)
    {
        this.buttonHexpadSymbol = buttonSymbol;
        this.buttonKeyCode = buttonKeyCode;
        this.buttonPressedValue = buttonValue;
    }

    public String getButtonHexpadSymbol()
    {
        return buttonHexpadSymbol;
    }

    public int getButtonKeyCode()
    {
        return buttonKeyCode;
    }

    public void setButtonKeyCode(int buttonKeyCode)
    {
        this.buttonKeyCode = buttonKeyCode;
    }

    public String getButtonPressedValue()
    {
        return buttonPressedValue;
    }

    public void setButtonPressedValue(String buttonPressedValue)
    {
        this.buttonPressedValue = buttonPressedValue;
    }

    @Override
    public String toString()
    {
        return buttonHexpadSymbol;

    }
}
