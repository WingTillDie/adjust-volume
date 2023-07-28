# toggle-volume
Toggle Windows sound volume via C++  
The program utilizes the Windows Core Audio API to interact with the audio endpoint and adjust the volume level.

## Usage
```
./toggle-volume.exe
```  
Running the program will reduce the sound volume to the lowest level

If the sound volume is already in the lowest level, it will increase the sound volume to the highest level

## Example applications
* Use with Windows Clock UWP app (the alarm volume depends on system sound volume, you might want to maximize it)
* Avoids accidental unmute by setting sound volume to the lowest level
