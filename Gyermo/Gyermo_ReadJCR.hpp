// License:
// 
// Gyermo
// JCR6 Analysis (header)
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
// Version: 24.11.26 III
// End License
#pragma once
#include <string>
#include <june19_core.hpp>


namespace Slyvina{ 
	namespace JCR6 { 
		namespace Gyermo{ 
			void Renew();
			void Renew(std::string Path);
			void Renew(std::string JCRFile,std::string Path);
			String CurrentJCRFile();

			std::string GRJType(std::string f);

			void Act_FileList(June19::j19gadget*, June19::j19action);
			void Act_UFP(June19::j19gadget* g, June19::j19action);
			void Extract(std::string src, std::string tgt);
			void ExtractButton(June19::j19gadget*, June19::j19action);
			void ExtractAllButton(June19::j19gadget*, June19::j19action);
		} 
	} 
}
