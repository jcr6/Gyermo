# Troubleshooting



- Why MUST I type "sh Gyermo_LinuxBuild.sh" in stead of of "Gyermo_LinuxBuild.sh"? Is that 'x' attribute too much to ask for? 
  - It is when your main system is Windows, as it doesn't support that attribute. I'm about to switch to Linux soon, but until that time you must either attach that x yourself or just use the sh command.
- Why didn't you put WestWood PAK support in Gyermo?
  - For the same reasons I didn't put in by default in Jalondi, but required you to set a specific switch for that. WestWood PAK files are a downright disaster and easily cause conflicts with other formats. Westwood never took JCR6 readers that could also read their PAK files in mind, and why should they? If those guys knew I would try to support these files I'm sure they'd turn them into an even bigger disaster. XD
- Why was TAR so problematic in JCR6?
  - TAR was totally NOT designed at all for the purpose JCR6 was set up. Due to this I could never build in full TAR support from the start. More details can be found [in this document](https://github.com/Slyvina/JCR6/blob/main/Docs/TAR.md).
- Why do I get a cmd window in Windows, but nothing similar in Linux?
  - That has to do with the poor design of Windows and Gyermo was not the kind of tool in which I deemed it important get rid of that. If you start Gyermo from the terminal in Linux you will get the same crap on your terminal as you'd see in the cmd window in Windows, though. If you start Gyermo from a start menu or a file manager or any tool like it, you won't see the terminal pop up (at least most distros do not, as far as I know). Now if you'd port Gyermo to Mac and put it into a Mac Application Bundle you'd only not see the terminal, but you will still see all terminal crap if you run the binary file directly from the terminal. This too is due to better design choices.
- Why no Mac version of Gyermo?
  - Because it will require a new kind of MAGA (Make Apple Great Again) before I get back to ANY Apple system at all. From the moment I heard that Steve Jobs was terminally ill, I already feared Apple would go down the drain once he'd be dead, and so it happened. I really regret this decission as MacOS X was a very good OS, but Apple has really gone into a kind paranoid state, abusing the existence of malware and cybercrime (for which you were the least treatened as a Mac User, to make things even more ironic. Something in which the Mac community took pride and laughed at Windows Users about, so that this was not clearly seen for the abuse it really is, is actually beyond me) and me paying hundreds of dollars to Apple as a sign you can trust me, is not within my budget. And as a matter of fact, the fact that people pay money as a sign you can trust them, is actually a sign you can NOT trust them. Money is not a sign of trust. We all know that.
- Why only one file in Linux and a crapload of files in Linux
  - Because in Linux it's not customary to put all required depenencies in the tar files, and in Windows, it is. The interal assets have in Linux also been merged into the executable, while in Windows it hasn't and comes with a seperate .jcr file.
- What dependencies do I need in Linux?
  - SDL2, SDL2_Mixer an SDL2_Image. Installing those should get Gyermo to work.


