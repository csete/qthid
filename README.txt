1. INTRODUCTION

Qthid is a Funcube Dongle control application with Qt user interface. It is an
evolution of the qthid002 package by Howard Long G6LVB.

The Funcube Dongle (http://www.funcubedongle.com/) is a small software defined
radio receiver for 64 MHz - 1.7 GHz. It fits into a computer's USB port and 
uses USB audio to transfer complex I/Q data to SDR applications. The control 
API for setting frequency, gain, filter, etc. uses the USB HID standard, thus 
the Funcube Dongle does not require any specific hardware drivers as long as 
the host operating system supports USB audio and HID.

Qthid uses the HIDAPI cross platform library by Signal 11 Software 
(http://www.signal11.us/oss/hidapi/). All the required files are bundled with 
qthid and no installation of the library is required.

The current version 3.1 implements the full API available with the 18f firmware.
It should cover all the functionality provided by the "fully functional windows
frontend". Note that the majority of this full API is only available since
firmware 18f and qthid 3.0 wil lnot wiork with earlier firmware. The application
will simply fail to detect any FCd with earlier firmwares.

You can use qthid 2.2 to upgrade your firmware to 18f or later.


2. INSTALLATION AND USAGE

2.1 Linux

You can use the precompiled binaries (32 or 64 bit) but you need the Qt runtime
libraries installed. On Ubuntu these are called libqtcore4 and libqtgui4 and
their dependencies. You can simply install everything Qt version 4 (not version 3!).
Qt 4.7 has been tested, Qt 4.6 will probably also work.

In order to use the Funcube Dongle as regular user, you need to create a udev rule.
Create a udev file /etc/udev/rules.d/20-fcd.rules containing:

  SUBSYTEMS=="usb" ATTRS{idVendor}=="04d8" ATTRS{idProduct}=="fb56" MODE:="0666" SYMLINK+="FCD"
  
(hint: open a terminal and type: sudo gedit /etc/udev/rules.d/20-fcd.rules)

Reload the rules using "sudo udevadm control --reload-rules"
Qthid should now detect your FCD when you plug it in.


2.2 Mac OS X (10.6+)

The Mac OS X bundle contains all the necessary Qt libraries and no additional
installation or configuration is required. Unlike Linux, the Funcube Dongle will
just work.


2.3 Windows

The windows package is self containing and does not require and Qt libs to be
installed separately. It is known to work on Windows XP, Vista and 7.


2.4 Building from source

- Install Qt Creator 2.0.1 or newer and Qt SDK 4.7. On recent linux it is normally
  sufficient to select Qt Creator and the required Qt libraries will be installed as
  dependencies. On Mac and Windows you need to download the full Qt SDK. On Windows
  you also need the MS Windows SDK.
- Open the qthid.pro file in Qt Creator. It will say something about setting up a target;
  say OK or Finish or Done (depends on version and OS).
- You can now build and execute the project.

If you want to try the latest experimental code checkout the 'next' branch, but
check first that it is indeed ahead of the master branch. You can do that on the
network chart: https://github.com/csete/qthid/network


3. License

Qthid is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

