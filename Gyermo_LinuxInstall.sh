echo "Gyermo Installation Script for Linux!"
echo "(c) Jeroen P. Broks"

if [ -e Exe/Linux/Gyermo ]; then
   if [ -e Exe/Linux/Gyermo.jcr ]; then
	echo "Installing Gyermo!"
	echo "The system may now ask for your password. Don't worry, this is just done by Linux itself!"
	sudo cat Exe/Linux/Gyermo Exe/Linux/Gyrmo.jcr > usr/bin/gyermo
   else
        echo "Gyermo.jcr was not present. The Jalondi script was not properly executed."
        echo "Try Gyermo_LinuxBuild.sh first!"
   fi
else
   echo "Executable for Gyermo doesn't exist! Clearly Gyermo was not yet properly compiled."
   echo "Try Gyermo_LinuxBuild.sh first!"
fi
