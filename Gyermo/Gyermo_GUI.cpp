// License:
// 
// Gyermo
// GUI
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
// Version: 24.11.23
// End License
#include <june19.hpp>
#include "Gyermo_GUI.hpp"
#include "Gyermo_Assets.hpp"
#include "Gyermo_Config.hpp"
using namespace Slyvina::TQSG;
using namespace Slyvina::TQSE;
using namespace Slyvina::June19;

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {
			static bool UI_KeepRunning{ true };

			static void UI_Debug(j19gadget* g, j19action a) {
				std::cout << "DEBUG GADGET: " << (int)g->GetKind() << "; (" << g->X() << "," << g->Y() << "); " << g->W() << "x" << g->H() << "; Kids:" << g->NumKids() << "\n";
			}


			static TImageFont UI_Font{ nullptr };
			static j19gadget
				* UI_Work{ nullptr },
				* UI_Background{ nullptr },
				* UI_Header{ nullptr },
				* UI_Left{ nullptr },
				* UI_Right{ nullptr },
				* UI_Resource{ nullptr },
				* UI_Directory{ nullptr },
				* UI_FileList{ nullptr };
				

			void UI_Init() {				
				const int HeadHeight{ 15 };
				const int Tab{ 150 };
				Graphics((int)floor(DesktopWidth() * .95), (int)floor(DesktopHeight() * .80), "Gyermo");
				QCol->Doing("Font", "Fonts/DOS.jfbf");
				UI_Font = LoadImageFont(Assets(), "Fonts/DOS.jfbf");
				j19gadget::SetDefaultFont(UI_Font);
				UI_Work = WorkScreen();
				UI_Work->Visible = true;
				UI_Background = CreatePicture(0, 0, UI_Work->W(), UI_Work->H(),UI_Work);
				UI_Background->Image(LoadImage(Assets(), "GFX/Fractal.png"));
				UI_Background->SetForeground(255, 255, 255, 255);
				UI_Header = CreateLabel("Gyermo - (c) Jeroen P. Broks", 0, 0, UI_Work->W(), HeadHeight, UI_Background);
				UI_Header->Visible = true;
				ColorGadget(UI_Header, "Header", "White", "Red");
				UI_Header->BA = 255;
				//UI_Header->CBDraw = UI_Debug;
				//UI_Work->CBDraw = UI_Debug;
				//UI_Left->CBDraw = UI_Debug;
				ColorGadget(CreateLabel("Resource:", 0, HeadHeight, 5, HeadHeight, UI_Work), "Text", "Yellow", "Yellow");
				ColorGadget(CreateLabel("Directory:", 0, HeadHeight*2, 5, HeadHeight, UI_Work), "Text", "Yellow", "Yellow");
				UI_Resource = CreateLabel("*Resource", Tab, HeadHeight, UI_Work->W() - (Tab + 5), HeadHeight, UI_Work);
				UI_Directory = CreateLabel("*Directory", Tab, HeadHeight * 2, UI_Work->W() - (Tab + 5), HeadHeight, UI_Work);
				ColorGadget(UI_Resource, "FIELD", "BrightBlue", "BrightBlue");
				ColorGadget(UI_Directory, "FIELD", "BrightBlue", "BrightBlue");
				UI_Left = CreateGroup(0, HeadHeight * 3, UI_Work->W() / 2, UI_Work->H() - (HeadHeight * 3), UI_Work);
				UI_Right = CreateGroup(0, HeadHeight * 3, UI_Work->W() / 2, UI_Work->H() - (HeadHeight * 3), UI_Work);
				UI_FileList = CreateListBox(5, 5, UI_Left->W() - 35, UI_Left->H() - 100, UI_Left);
				ColorGadget(UI_FileList, "FileList", "White", "BrightBlue");
			}


			void UI_Run() {
				do {
					Cls();
					Poll();
					UI_Work->Draw();
					//std::cout << LabelError() << "\n"; // debug
					Flip();
					if (AppTerminate()) UI_KeepRunning = false;
				} while (UI_KeepRunning);
				FreeJune19();
			}
		}
	}
}
