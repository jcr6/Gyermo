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
// Version: 24.11.28 I
// End License

#include <JCR6_TAR.hpp>
#include <SlyvDirry.hpp>
#include <SlyvGINIE.hpp>
#include <SlyvQCol.hpp>
#include <june19_core.hpp>
#include "Gyermo_Config.hpp"
#include "Gyermo_GUI.hpp"

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
				TAR_ReturnWhatIHave = Upper(_Config->NewValue("TAR", "RETURN_WHAT_I_HAVE", "TRUE")) == "TRUE";
				TAR_ThoroughCheck = Upper(_Config->NewValue("TAR", "THOROUGH_CHECK", "FALSE")) == "TRUE";
				TAR_SymLinking = Upper(_Config->NewValue("TAR", "SYMLINKING", "FALSE")) == "TRUE";
			}

			void ClearUsed(June19::j19gadget* lb,June19::j19action) {
				_Config->Kill("Used");
				if (UI_NavUsed) UI_NavUsed->ClearItems();
			}

			std::string ConfigFile() {
				return Dirry("$AppSupport$/Gyermo.ini");
			}

			void UpdateUsed(June19::j19gadget* up) {
				auto nl{ _Config->List("Used","Used") };
				up->ClearItems();
				for (int p = (int)nl->size()-1; p >= 0; p--) { 
					auto& sr{ (*nl)[p] };
					if (_Config->HasValue("Used", sr)) up->AddItem((*nl)[p]); 
				}
			}

			void UpdateFavorites(June19::j19gadget* fp) {
				auto fl{ _Config->Values("Favorites") };
				fp->ClearItems();
				for (auto ft : *fl) {
					auto file{ _Config->Value("Favorites",ft) };
					auto sfil{ StripDir(file) };					
					if (!FileExists(file)) {
						_Config->Kill("Favorites", ft,GINIE_Kill::Values);
						QCol->Warn("Removed non-existent file '" + file + "' from the favorite list");
					} else {
						fp->AddItem(sfil);
					}
				}
			}

			void AddUsed(June19::j19gadget* up, std::string res) {
				auto lu{ _Config->List("Used","Used") };
				auto sr{ StripDir(res) };
				_Config->Value("Used",sr, res);
				if (lu->size() && (*lu)[lu->size() - 1] == res) return; 
				auto nl{ NewVecString() };
				for (auto o : *lu) {
					if (o != sr) nl->push_back(o);
				}
				nl->push_back(sr);
				if (nl->size() > std::max(15, _Config->NewValue("Used", "*Max", 30))) nl->erase(nl->begin());
				_Config->ReplaceList("Used", "Used", nl);;
				UpdateUsed(up);
			}

			String CFGV(std::string c, std::string v) {
				return _Config->Value(c, v);
			}

			void CFGV(std::string c, std::string v, std::string newv) {
				_Config->Value(c, v, newv);
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

			void RemoveFavorite(std::string f) {
				auto victim{ StripDir(f) };
				QCol->Doing("Removing", f, ""); QCol->Yellow(" from favorites\n");
				_Config->Kill("Favorites",victim,GINIE_Kill::Values );
				UpdateFavorites(UI_NavFav);
			}

			void RemoveFavorite(June19::j19gadget* g, June19::j19action) {
				if (UI_NavFav->SelectedItem() >= 0 && UI_NavFav->ItemText().size()) RemoveFavorite(UI_NavFav->ItemText());
			}
			
		}
	}
}
