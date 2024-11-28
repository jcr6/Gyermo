// License:
// 
// Gyermo
// Main
// 
// 
// 
// 	(c) Jeroen P. Broks, 2024
// 
// 		This program is free software: you can redistribute it and/or modify
// 		it under the terms of the GNU General Public License as published by
// 		the Free Software Foundation, either version 3 of the License, or
// 		(at your option) any later version.
// 
// 		This program is distributed in the hope that it will be useful,
// 		but WITHOUT ANY WARRANTY; without even the implied warranty of
// 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// 		GNU General Public License for more details.
// 		You should have received a copy of the GNU General Public License
// 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// 	Please note that some references to data like pictures or audio, do not automatically
// 	fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 24.11.28 II
// End License

#include <Slyvina.hpp>
#ifdef SlyvLinux
#include <SDL2/SDL_main.h>
#else
#include <SDL_main.h>
#endif
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>
#include <SlyvStream.hpp>
#include <JCR6_JQL.hpp>
#include <JCR6_QuakePAK.hpp>
#include <JCR6_RealDir.hpp>
#include <JCR6_WAD.hpp>
#include <JCR6_TAR.hpp>
#include <JCR6_zlib.hpp>
#include <High_Headers.hpp>
#include "Gyermo_GUI.hpp"
#include "Gyermo_Assets.hpp"
#include "Gyermo_ReadJCR.hpp"

using namespace Slyvina;
using namespace Slyvina::Units;
using namespace Slyvina::JCR6;
using namespace Slyvina::JCR6::Gyermo;


int main(int argc, char** args) {
	QCol->LMagenta("Gyermo\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	QCol->Yellow("(c) 2024-" + Right(__DATE__, 4) + ", Jeroen P. Broks\n\n");
	auto startres{ argc >= 2 ? String(args[1]) : CurrentDir() };
	InitJQL();
	InitQuake();
	InitWAD();
	init_zlib();
	InitTAR(); 
	Kitty::KittyHigh_ALL();
	JCR6_InitRealDir();
	Asset_Init(args[0]);
	UI_Init(); Renew(startres);
	UI_Run();
	UI_Done();
}
