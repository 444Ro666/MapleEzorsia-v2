# MapleEzorsia

This DLL modifies addresses in a v83 MapleStory client to change the game window and canvas resolution.  
Some in-game component boundaries are also modified to scale with the custom resolution.  
This code can only be compiled on the x86 platform.

---
### Default config.ini
The default `config.ini` file isn't included or generated but is optional.
Simply make a file called `config.ini` in your game folder (where the .dll is) and the program will try to use it.  
```
[general]
width=1280
height=720
```

### How to use
In your v83 MapleStory directory,  
1. Rename file `nmconew.dll` to `nmconew2.dll`  
2. insert [this DLL file](https://github.com/izarooni/MapleEzorsia/releases/latest) in that directory  
3. Run your client!  
