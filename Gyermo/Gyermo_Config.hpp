// License:
// 
// Gyermo
// Configuration (header)
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
// Version: 24.11.26
// End License

#pragma once
#include <string>
#include <june19_core.hpp>
#include "Gyermo_Colors.hpp"

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {
			std::string ConfigFile();
			void ClearUsed(June19::j19gadget* lb, June19::j19action);
			inline void ClearUsed(June19::j19gadget* lb) { ClearUsed(lb, June19::j19action::Unknown); }
			inline void ClearUsed() { ClearUsed(nullptr, June19::j19action::Unknown); }
			void UpdateUsed(June19::j19gadget* up);
			void AddUsed(June19::j19gadget* up, std::string res);
			TCol GetCol(String ColStr);
			void ColorGadget(June19::j19gadget* g,String Tag,String DefaultColorF="White",String DefaultColorB="DarkGray");
			void ColorGadgetReverse(June19::j19gadget* g, String Tag, String DefaultColorF = "White", String DefaultColorB = "DarkGray");
		}
	}
}
