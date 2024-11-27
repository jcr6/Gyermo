// License:
// 
// Gyermo
// Asset Manager
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
// Version: 24.11.27 V
// End License
#include "Gyermo_Assets.hpp"

#include <JCR6_Core.hpp>
#include <SlyvQCol.hpp>
#include <SlyvStream.hpp>
using namespace Slyvina::Units;

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {

			static String ResFile{};
			static JT_Dir Res{ nullptr };

			void Asset_Init(String Exe) {
				//std::cout << "JCR6 DEBUG: " << Exe << " recognized as " << _JT_Dir::Recognize(Exe);
#ifdef SlyvLinux
				ResFile = _JT_Dir::Recognize("/proc/self/exe")!="NONE" ? ResFile = "/proc/self/exe" : (_JT_Dir::Recognize(Exe) != "NONE" ? ResFile = Exe : ResFile = StripExt(Exe) + ".jcr");
#else
				ResFile = _JT_Dir::Recognize(Exe) != "NONE" ? ResFile = Exe : ResFile = StripExt(Exe) + ".jcr";
#endif
				//std::cout << " -> resfile is now: " << ResFile << "\n";
			}
			JT_Dir Assets() {
				if (!Res) {
					QCol->Doing("Reading", ResFile);
					if (!FileExists(ResFile)) { QCol->Error(ResFile + " not found"); exit(404); }				
					Res = JCR6_Dir(ResFile);
					if (Last()->Error) {
						QCol->Error("Error reading main resource");
						QCol->Doing("- Error", Last()->ErrorMessage);
						QCol->Doing("- Entry", Last()->Entry);
						QCol->Doing("- Main", Last()->MainFile);
					}
					if (!Res) { QCol->Error("Reading the main resource failed for unknown reasons"); exit(500); }
				}
				return Res;
			}
		}
	}
}
