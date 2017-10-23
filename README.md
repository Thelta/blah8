# blah8
A chip8 interpreter written in C.

To run a game, first run blah8_gui.jar. Select game file, change options you want to change then hit "Start Emulator" button.

## Emulator
Emulator consists from two parts: Interpreter and Handler.

### Interpreter
To write interpreter I used [Cowgod's Chip-8 reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) . It pretty much covers everything 
you need to know about chip8. I used function pointers instead of swich-case for instruction handling. While it still isn't pretty good solution because
to run an instruction, it may call two functions. While this would be prevented using a hash map, I **really** didn't want to write (or copy) hash map,
I still think it isn't that bad as switch-cases.


### Handler
For window handling, rendering and input handling it uses SDL library. There are few optimizations for drawback call I learnt from [building Minecraft clone] 
(https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/). While this probably wasn't necessary, I already had to change few things from rendering code
so It wouldn't hurt. To produce sound, I didn't need to use any library, because chip8 builds only one sound, so I used printf("%c", 7) trick. This makes os to
produce error sound. So far so good.

I could talk a lot about input handling, but I will talk about that at launcher part.

##Launcher
It is written in Java using JavaFX. The reason I used Java because It was universal and it had vast libraries. In Java Swing, you can cull resolutions
so I didn't have to write platform specific code.

After a day, launcher was finished, I thought. Then I remembered one of the biggest reasons I wanted to write a launcher for this was custom button layout.
After thinking a while for how to handle this, I knew I was in trouble. JavaFX's event system didn't support scancodes so a non-US keyboard would have trouble
when a special character is pressed. To handle this I used @kwhat's [jnativehook](https://github.com/kwhat/jnativehook) library. While it gave me scancodes, it gave me linux scancodes. However SDL
uses scancodes specified by USB organization. To resolve this, I used [Chromium's keycode map](https://chromium.googlesource.com/chromium/chromium/+/master/ui/base/keycodes/usb_keycode_map.h).
