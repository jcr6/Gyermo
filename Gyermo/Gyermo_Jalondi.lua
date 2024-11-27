-- License:
-- 
-- Gyermo
-- Jalondi Asset Pack Script
-- 
-- 
-- 
-- 	(c) Jeroen P. Broks, 2024
-- 
-- 		This program is free software: you can redistribute it and/or modify
-- 		it under the terms of the GNU General Public License as published by
-- 		the Free Software Foundation, either version 3 of the License, or
-- 		(at your option) any later version.
-- 
-- 		This program is distributed in the hope that it will be useful,
-- 		but WITHOUT ANY WARRANTY; without even the implied warranty of
-- 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- 		GNU General Public License for more details.
-- 		You should have received a copy of the GNU General Public License
-- 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
-- 
-- 	Please note that some references to data like pictures or audio, do not automatically
-- 	fall under this licenses. Mostly this is noted in the respective files.
-- 
-- Version: 24.11.27
-- End License
Jalondi.Doing("Scripting","Jalondi")
Jalondi.Doing("Platform",Jalondi.Platform)

if Jalondi.Platform=="Windows" then
	BaseDir="/Projects"
elseif Jalondi.Platform=="Linux" then
	BaseDir="../../../.." -- May need to be changed later, but for now it will do!
	--error("Linux Scripting not yet properly set up")
else
	error("Unknown platform")
end

Jalondi.Add(BaseDir.."/Fonts/DosFont/DosFont.jfbf","Fonts/DOS.jfbf","zlib","","")
Jalondi.Alias("Fonts/DOS.jfbf/124.png","Fonts/DOS.jfbf/118.108.png")
Jalondi.Add("Assets","","zlib")
Jalondi.Start("../Exe/"..Jalondi.Platform.."/Gyermo.jcr")
