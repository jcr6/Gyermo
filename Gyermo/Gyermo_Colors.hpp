// License:
// 
// Gyermo
// Colors
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
#include <Slyvina.hpp>
#include <SlyvQCol.hpp>

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {
			struct TCol { Byte R{ 0 }, G{ 0 }, B{ 0 }; };

			enum class ECol {
				Black,
				Blue,
				Green,
				Cyan,
				Red,
				Magenta,
				Brown,
				BrightGrey,
				DarkGrey,
				BrightBlue,
				BrightGreen,
				BrightCyan,
				BrightRed,
				BrightMagenta,
				Yellow,
				White,
				Pink = 12,
				Grey = 7
			};

			static std::map<ECol, TCol> MCol{
				{ECol::Black,{0,0,0}},
				{ECol::Blue,{0,0,170}},
				{ECol::Green,{0,170,0}},
				{ECol::Cyan,{0,170,170}},
				{ECol::Magenta,{170,0,170}},
				{ECol::Red,{170,0,0}},
				{ECol::Brown,{170,85,0}},
				{ECol::BrightGrey,{170,170,170}},
				{ECol::DarkGrey,{85,85,85}},
				{ECol::BrightBlue,{85,85,255}},
				{ECol::BrightGreen,{85,255,85}},
				{ECol::BrightCyan,{85,255,255}},
				{ECol::BrightRed,{255,85,85}},
				{ECol::BrightMagenta,{255,85,255}},
				{ECol::Yellow,{255,255,85}},
				{ECol::White,{255,255,255}}
			};

			static std::map<String, ECol> ColSM{
				{"Black",ECol::Black},
				{"Blue",ECol::Blue},
				{"Green",ECol::Green},
				{"Cyan",ECol::Cyan},
				{"Red",ECol::Red},
				{"Magenta",ECol::Magenta},
				{"Brown",ECol::Brown},
				{"BrightGrey",ECol::BrightGrey},
				{"DarkGrey",ECol::DarkGrey},
				{"BrightBlue",ECol::BrightBlue},
				{"BrightGreen",ECol::BrightGreen},
				{"BrightCyan",ECol::BrightCyan},
				{"BrightRed",ECol::BrightRed},
				{"BrightMagenta",ECol::BrightMagenta},
				{"Yellow",ECol::Yellow},
				{"White",ECol::White},
				{"Pink",ECol::Pink},
				{"Grey",ECol::Grey}
			};

			inline TCol SCol(String N) {
				if (!ColSM.count(N)) { QCol->Error("Unknown color name: " + N); exit(1); }
				return MCol[ColSM[N]];
			}
		}
	}
}
