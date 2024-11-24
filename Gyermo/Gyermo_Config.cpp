// License:
// 
// Gyermo
// Configuration
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
// Version: 24.11.24
// End License
#include "Gyermo_Config.hpp"
#include <SlyvDirry.hpp>
#include <SlyvGINIE.hpp>
#include <SlyvQCol.hpp>
using namespace Slyvina::Units;
namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {
			static GINIE _Config{ nullptr };

			static void LoadMe() {
				if (!_Config) {
					QCol->Doing("Reading", ConfigFile());
					_Config = LoadOptGINIE(ConfigFile(), ConfigFile(), "Gyermo configuration\n(c) Jeroen P. Broks");
				}
			}

			std::string ConfigFile() {
				return Dirry("$AppSupport$/Gyermo.ini");
			}

			TCol GetCol(String ColStr) {
				if (ColSM.count(ColStr)) return MCol[ColSM[ColStr]];
				if (IndexOf(ColStr, ',') >= 0) {
					auto SC{ Split(ColStr,',') };
					if (SC->size() != 3) { QCol->Error("Color syntax error: " + ColStr); exit(2); }
					TCol ret{ (Byte)ToInt((*SC)[0]), (Byte)ToInt((*SC)[1]), (Byte)ToInt((*SC)[2]) };
					return ret;
				}
				ECol gc{ (ECol)(ToInt(ColStr) % 16) };
				return MCol[gc];
				return TCol();
			}

			void ColorGadget(June19::j19gadget* g, String Tag, String DefaultColorF, String DefaultColorB) {
				LoadMe();
				auto
					FG{ GetCol(_Config->NewValue("Colors::Foreground",Tag,DefaultColorF)) },
					BG{ GetCol(_Config->NewValue("Colors::Background",Tag,DefaultColorB)) };
				//QCol->Reset();  printf("Color Gadget: F:%02x%02x%02x B:%02x%02x%02x\n", FG.R, FG.G, FG.B, BG.R, BG.G, BG.B);
				g->FR = FG.R;
				g->FG = FG.G;
				g->FB = FG.B;
				g->BR = BG.R;
				g->BG = BG.G;
				g->BB = BG.B;
			}

			void ColorGadgetReverse(June19::j19gadget* g, String Tag, String DefaultColorF, String DefaultColorB) {
				LoadMe();
				auto
					FG{ GetCol(_Config->NewValue("Colors::Foreground",Tag,DefaultColorF)) },
					BG{ GetCol(_Config->NewValue("Colors::Background",Tag,DefaultColorB)) };
				//QCol->Reset();  printf("Color Gadget: F:%02x%02x%02x B:%02x%02x%02x Reversed\n", FG.R, FG.G, FG.B, BG.R, BG.G, BG.B);
				g->BR = FG.R;
				g->BG = FG.G;
				g->BB = FG.B;
				g->FR = BG.R;
				g->FG = BG.G;
				g->FB = BG.B;
			}
			
		}
	}
}
