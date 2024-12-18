// License:
// 
// Gyermo
// View Manager (header)
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
// Version: 24.11.25 I
// End License
#pragma once
#include <string>

#include <june19_core.hpp>
#include "Gyermo_Colors.hpp"

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {

			enum class ViewType { Text, Code, Audio, Picture, Binary };

			void ViewCLS();
			void ViewInit(Slyvina::June19::j19gadget* mammie);
			void ViewWrite(std::string a);
			inline void ViewWriteLn(std::string a) { ViewWrite(a + "\n"); }
			bool IsBinary(Units::Bank B);

			void ViewPlainText(Units::Bank B);
			void ViewColor(ECol c);

			void ViewEntry(JCR6::JT_Dir JD, std::string entry);

			void ViewAudioCallBack(June19::j19gadget*, June19::j19action);

			String PicFormat(String f);
			String AudioFormat(String f);

		}
	}
}
