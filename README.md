# windows_audio_device_switcher
This program allows to switch the default audio device in windows via a console program. This can also easily be called via a windows shortcut or by programs like Logitech Gaming Software. The cool thing here is that the devices to switch to can be given by the name as displayed in the "Sounds" settings dialog.
This program was tested successfully under Windows 7, 8, 8.1 and 10.

Download: https://github.com/fivef/windows_audio_device_switcher/releases/download/v1.0/EndPointController.exe

Usage: 

1. Open windows command line (Ctrl+R, cmd, Enter)
2. Go to the path where EndPointController.exe is located.
3. Execute EndPointController.exe without arguments for usage infromation and a list of all available devices.
4. To switch to a specific device by name execute e.g.: EndPointController.exe "Logitech G930" (Use " if there are spaces in the name)
