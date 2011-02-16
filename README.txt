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

