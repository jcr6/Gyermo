// License:
// 
// Gyermo
// GUI (header)
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
#pragma once
#include <Slyvina.hpp>
#include <june19_core.hpp>

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {
			void UI_Init();
			void UI_Run();
			void UI_Done();

			extern June19::j19gadget
				* UI_DataEntry,
				* UI_DataMain,
				* UI_DataType,
				* UI_DataSize,
				* UI_DataRatio,
				* UI_DataAliasLabel,
				* UI_DataAlias,
				* UI_DataOtherLabel,
				* UI_DataOther,
				* UI_DataResourceType;

			extern std::map<String, June19::j19gadget*> UI_DataFields;
		}
	}
}
