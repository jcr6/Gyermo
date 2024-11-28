# How to build Gyèrmo

Gyèrmo is set up to run in the Slyvina Framework. That being said you will need a dedicated folder for Slyvina.
This folder should have a Tools, Libs and Apps folder.
Gyèrmo will have to be located in Slyvina/Apps/Gyermo
The Slyvina libraries required: Units, Kitty, JCR6 and TQSL.

In future versions of this document I will put some time in having all this set up correctly.
When it comes to 3rd party dependencies: zlib and SDL2.
I will also go more into the deep about those two later.

# Windows

In Windows, the best you can do is to open the Gyermo.sln file in Visual Studio and just compile Gyermo there.
Please note, PowerShell must be able to find Jalondi.exe from the [Jalondi](https://github.com/jcr6/Jalondi) repository, as Jalondi need to package the assets Gyermo needs.
In the Gyermo folder, the executable, all required .dll files and Gyermo.jcr will after building be in Exe/Windows


# Linux

Please note that for the building scripts in Linux, you will need Jalondi, GCC, Python3 and SCons. CLick here to find [Jalondi](https://github.com/jcr6/Jalondi), and GCC, Python3 and SCons should be available on any regular package manager for Linux.
The best way to go is to run: Gyermo_BuildLinux.sh in order to compile.
Running Gyermo_InstallLinux.sh will install Gyermo into usr/bin. Please note that Linux will ask for your password in order to allow this.

Please note that Jalondi must be present in any directory in your PATH environment file. The best way to go is to put the "jalondi" executable file into your usr/bin file in order to accomplish this.
