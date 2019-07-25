package sample.sample.HexpadControls;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.util.HashMap;
import java.util.HashSet;
import java.util.StringJoiner;

public class HexpadControls
{
    private HexpadButton[] hexpadButtons = {
            new HexpadButton("0", "1", 2),
                new HexpadButton("1", "2", 3),
                new HexpadButton("2", "3", 4),
                new HexpadButton("3", "4", 5),
                new HexpadButton("4", "Q", 16),
                new HexpadButton("5", "W", 17),
                new HexpadButton("6", "E", 18),
                new HexpadButton("7", "R", 19),
                new HexpadButton("8", "A", 30),
                new HexpadButton("9", "S", 31),
                new HexpadButton("A", "D", 32),
                new HexpadButton("B", "F", 33),
                new HexpadButton("C", "Z", 44),
                new HexpadButton("D", "X", 45),
                new HexpadButton("E", "C", 46),
                new HexpadButton("F", "V", 47)
    };

    private HashSet<Integer> usedKeyCodes;

    private HashMap<Integer, Integer> linux2USBMap;

    public static String KEYCODE_DELIMITER = ";";
    public static String VALUE_DELIMITER = "asdf";

    public HexpadControls()
    {
        assignKeycodeTransformerMap();

        usedKeyCodes = new HashSet<>();

        for(HexpadButton button : hexpadButtons)
        {
            button.setButtonKeyCode(linux2USBMap.get(button.getButtonKeyCode()));

            usedKeyCodes.add(button.getButtonKeyCode());
        }
    }

    public void setConfigButtons(int[] buttonKeycode, String[] buttonValue)
    {
        for(int i = 0; i < hexpadButtons.length; i++)
        {
            changeButton(i, buttonKeycode[i], buttonValue[i]);
        }

    }

    public boolean changeButton(int buttonIndex, int newKeyCode, String newButtonPressedValue )
    {
        if(usedKeyCodes.contains(linux2USBMap.get(newKeyCode)))
        {
            return false;
        }

        usedKeyCodes.remove(hexpadButtons[buttonIndex].getButtonKeyCode());

        hexpadButtons[buttonIndex].setButtonKeyCode(newKeyCode);
        hexpadButtons[buttonIndex].setButtonPressedValue(newButtonPressedValue);

        return true;
    }

    public String getButtonValue(int buttonIndex)
    {
        return hexpadButtons[buttonIndex].getButtonPressedValue();
    }

    public ObservableList<HexpadButton> getObservableHexpadButtons()
    {
        return FXCollections.observableArrayList(hexpadButtons);
    }

    private void assignKeycodeTransformerMap()
    {
        linux2USBMap = new HashMap<>();

        //<editor-fold desc="map assignments">
        linux2USBMap.put(30, 4);
        linux2USBMap.put(48, 5);
        linux2USBMap.put(46, 6);
        linux2USBMap.put(32, 7);
        linux2USBMap.put(18, 8);
        linux2USBMap.put(33, 9);
        linux2USBMap.put(34, 10);
        linux2USBMap.put(35, 11);
        linux2USBMap.put(23, 12);
        linux2USBMap.put(36, 13);
        linux2USBMap.put(37, 14);
        linux2USBMap.put(38, 15);
        linux2USBMap.put(50, 16);
        linux2USBMap.put(49, 17);
        linux2USBMap.put(24, 18);
        linux2USBMap.put(25, 19);
        linux2USBMap.put(16, 20);
        linux2USBMap.put(19, 21);
        linux2USBMap.put(31, 22);
        linux2USBMap.put(20, 23);
        linux2USBMap.put(22, 24);
        linux2USBMap.put(47, 25);
        linux2USBMap.put(17, 26);
        linux2USBMap.put(45, 27);
        linux2USBMap.put(21, 28);
        linux2USBMap.put(44, 29);
        linux2USBMap.put(2, 30);
        linux2USBMap.put(3, 31);
        linux2USBMap.put(4, 32);
        linux2USBMap.put(5, 33);
        linux2USBMap.put(6, 34);
        linux2USBMap.put(7, 35);
        linux2USBMap.put(8, 36);
        linux2USBMap.put(9, 37);
        linux2USBMap.put(10, 38);
        linux2USBMap.put(11, 39);
        linux2USBMap.put(28, 40);
        linux2USBMap.put(1, 41);
        linux2USBMap.put(14, 42);
        linux2USBMap.put(15, 43);
        linux2USBMap.put(57, 44);
        linux2USBMap.put(12, 45);
        linux2USBMap.put(13, 46);
        linux2USBMap.put(26, 47);
        linux2USBMap.put(27, 48);
        linux2USBMap.put(43, 50);
        linux2USBMap.put(39, 51);
        linux2USBMap.put(40, 52);
        linux2USBMap.put(41, 53);
        linux2USBMap.put(51, 54);
        linux2USBMap.put(52, 55);
        linux2USBMap.put(53, 56);
        linux2USBMap.put(58, 57);
        linux2USBMap.put(59, 58);
        linux2USBMap.put(60, 59);
        linux2USBMap.put(61, 60);
        linux2USBMap.put(62, 61);
        linux2USBMap.put(63, 62);
        linux2USBMap.put(64, 63);
        linux2USBMap.put(65, 64);
        linux2USBMap.put(66, 65);
        linux2USBMap.put(67, 66);
        linux2USBMap.put(68, 67);
        linux2USBMap.put(87, 68);
        linux2USBMap.put(88, 69);
        linux2USBMap.put(57399, 70);
        linux2USBMap.put(70, 71);
        linux2USBMap.put(3653, 72);
        linux2USBMap.put(57426, 73);
        linux2USBMap.put(57415, 74);
        linux2USBMap.put(57417, 75);
        linux2USBMap.put(57427, 76);
        linux2USBMap.put(57423, 77);
        linux2USBMap.put(57425, 78);
        linux2USBMap.put(57421, 79);
        linux2USBMap.put(57419, 80);
        linux2USBMap.put(57424, 81);
        linux2USBMap.put(57416, 82);
        linux2USBMap.put(69, 83);
        linux2USBMap.put(57397, 84);
        linux2USBMap.put(55, 85);
        linux2USBMap.put(74, 86);
        linux2USBMap.put(78, 87);
        linux2USBMap.put(57372, 88);
        linux2USBMap.put(79, 89);
        linux2USBMap.put(80, 90);
        linux2USBMap.put(81, 91);
        linux2USBMap.put(75, 92);
        linux2USBMap.put(76, 93);
        linux2USBMap.put(77, 94);
        linux2USBMap.put(71, 95);
        linux2USBMap.put(72, 96);
        linux2USBMap.put(73, 97);
        linux2USBMap.put(82, 98);
        linux2USBMap.put(83, 99);
        linux2USBMap.put(86, 100);
        linux2USBMap.put(57437, 101);
        linux2USBMap.put(91, 104);
        linux2USBMap.put(92, 105);
        linux2USBMap.put(93, 106);
        linux2USBMap.put(99, 107);
        linux2USBMap.put(100, 108);
        linux2USBMap.put(101, 109);
        linux2USBMap.put(102, 110);
        linux2USBMap.put(103, 111);
        linux2USBMap.put(104, 112);
        linux2USBMap.put(105, 113);
        linux2USBMap.put(106, 114);
        linux2USBMap.put(107, 115);
        linux2USBMap.put(57403, 117);
        linux2USBMap.put(57352, 122);
        linux2USBMap.put(57367, 123);
        linux2USBMap.put(57368, 124);
        linux2USBMap.put(57354, 125);
        linux2USBMap.put(57376, 127);
        linux2USBMap.put(57392, 128);
        linux2USBMap.put(57390, 129);
        linux2USBMap.put(125, 137);
        linux2USBMap.put(29, 224);
        linux2USBMap.put(42, 225);
        linux2USBMap.put(56, 226);
        linux2USBMap.put(57435, 227);
        linux2USBMap.put(57373, 228);
        linux2USBMap.put(54, 229);
        linux2USBMap.put(57400, 230);
        linux2USBMap.put(57436, 231);
        //</editor-fold>
    }

    public String getConfigKeycodeString()
    {
        StringJoiner joiner = new StringJoiner(KEYCODE_DELIMITER);

        for(HexpadButton button: hexpadButtons)
        {
            joiner.add(Integer.toString(button.getButtonKeyCode()));
        }

        return joiner.toString();
    }

    public String getConfigValueString()
    {
        StringJoiner joiner = new StringJoiner(VALUE_DELIMITER);

        for(HexpadButton button : hexpadButtons)
        {
            joiner.add(button.getButtonPressedValue());
        }

        return joiner.toString();
    }



}
