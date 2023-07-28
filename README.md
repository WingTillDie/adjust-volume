# toggle-volume
Adjust Windows sound volume via C++  
The program utilizes the Windows Core Audio API to interact with the audio endpoint and adjust the volume level.

## Usage
```bash
# Usage: ./adjust-volume.exe [volume] 

# Get current sound volume
./adjust-volume.exe
# Output: Current sound volume: 0.1

# Set current sound volume to 0.2
./adjust-volume.exe .2
```


``` bash
./toggle-volume.exe
# if sound_level != 0:
#     set_sound_level(0)
# else:
#     set_sound_level(1)
```

## Example applications
./toggle-volume.exe
* Use with Windows Clock UWP app (the alarm volume depends on system sound volume, you might want to maximize it)
* Avoids accidental unmute by setting sound volume to the lowest level
