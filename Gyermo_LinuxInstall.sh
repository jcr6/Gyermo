# License:
# 
# Gyermo
# Linux Install Script
# 
# 
# 
# 	(c) Jeroen P. Broks, 2024
# 
# 		This program is free software: you can redistribute it and/or modify
# 		it under the terms of the GNU General Public License as published by
# 		the Free Software Foundation, either version 3 of the License, or
# 		(at your option) any later version.
# 
# 		This program is distributed in the hope that it will be useful,
# 		but WITHOUT ANY WARRANTY; without even the implied warranty of
# 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# 		GNU General Public License for more details.
# 		You should have received a copy of the GNU General Public License
# 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
# 	Please note that some references to data like pictures or audio, do not automatically
# 	fall under this licenses. Mostly this is noted in the respective files.
# 
# Version: 24.11.28
# End License
echo "Gyermo Installation Script for Linux!"
echo "(c) Jeroen P. Broks"

if [ -e Exe/Linux/Gyermo ]; then
   if [ -e Exe/Linux/Gyermo.jcr ]; then
	echo "Installing Gyermo!"
	#echo "The system may now ask for your password. Don't worry, this is just done by Linux itself!"
	echo "If the system says 'permission denied' or anything similar, please run this script again with 'sudo'!"
	echo "Creating /usr/bin/gyermo"
	cat Exe/Linux/Gyermo Exe/Linux/Gyermo.jcr > /usr/bin/gyermo
	echo "Configuring permissions"
	chmod +x /usr/bin/gyermo
	echo "Ok"
   else
        echo "Gyermo.jcr was not present. The Jalondi script was not properly executed."
        echo "Try Gyermo_LinuxBuild.sh first!"
   fi
else
   echo "Executable for Gyermo doesn't exist! Clearly Gyermo was not yet properly compiled."
   echo "Try Gyermo_LinuxBuild.sh first!"
fi
