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
// Version: 24.11.26 VI
// End License
#include <map>
#include <june19.hpp>
#include <SlyvSharedArray.hpp>
#include "Gyermo_GUI.hpp"
#include "Gyermo_Assets.hpp"
#include "Gyermo_Config.hpp"
#include "Gyermo_ReadJCR.hpp"
#include "Gyermo_View.hpp"
#include <SlyvVolumes.hpp>

using namespace Slyvina::TQSG;
using namespace Slyvina::TQSE;
using namespace Slyvina::Units;
using namespace Slyvina::June19;

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {
			static bool UI_KeepRunning{ true };

			static void UI_Debug(j19gadget* g, j19action a) {
				std::cout << "DEBUG GADGET: " << (int)g->GetKind() << "; (" << g->X() << "," << g->Y() << "); " << g->W() << "x" << g->H() << "; Kids:" << g->NumKids() << "\n";
			}


			static String UI_CurrentTab{ "Data" };
			static std::map<String,j19gadget*> UI_TabPanels{};
			static std::map<String, j19gadget*> UI_NavRadio{ {"Volumes",nullptr},{"Used",nullptr},{"Favorites",nullptr} };
			static std::map<String, j19gadget*> UI_NavPanels{ };
		
			std::map<String, j19gadget*>
				UI_DataFields{},
				UI_BlockFields{};
			static TImageFont UI_Font{ nullptr };
			static j19gadget
				* UI_Work{ nullptr },
				* UI_Background{ nullptr },
				* UI_Header{ nullptr },
				* UI_Nav{ nullptr },
				* UI_Left{ nullptr },
				* UI_TabData{ nullptr },
				* UI_TabBlock{ nullptr },
				* UI_BlckNoBlock{ nullptr },
				* UI_TabView{ nullptr },
				* UI_AudioPlay{ nullptr },
				* UI_AudioLoop{ nullptr },
				* UI_AudioStop{ nullptr },
				* UI_NavUsedClear{ nullptr },
				* UI_NavFavAdd{ nullptr },
				* UI_NavFavRemove{ nullptr };
			j19gadget
				* UI_NavUsed{ nullptr },
				* UI_NavFav{ nullptr },
				* UI_Right{ nullptr },
				* UI_Resource{ nullptr },
				* UI_Directory{ nullptr },
				* UI_FileList{ nullptr },
				* UI_DataEntry{ nullptr },
				* UI_DataMain{ nullptr },
				* UI_DataType{ nullptr },
				* UI_DataSize{ nullptr },
				* UI_DataRatio{ nullptr },
				* UI_DataAliasLabel{ nullptr },
				* UI_DataAlias{ nullptr },
				* UI_DataOtherLabel{ nullptr },
				* UI_DataOther{ nullptr },
				* UI_DataResourceType{ nullptr },
				* UI_BlckGroup{ nullptr },
				* UI_BlckEntry{ nullptr },
				* UI_BlckID{ nullptr },
				* UI_BlckEntries{ nullptr },
				* UI_BlckEntryOffset{ nullptr },
				* UI_BlckRatio{ nullptr },
				* UI_BlckOther{ nullptr },
				* UI_ViewEntry{ nullptr },
				* UI_ViewText{ nullptr },
				* UI_ViewAudio{ nullptr },
				* UI_ViewPicture{ nullptr },
				* UI_ViewPictureImage{ nullptr },
				* UI_ViewPictureLabel{ nullptr },
				* UI_Extract{ nullptr },
				* UI_ExtractAll{ nullptr };

			static UniqueArray<j19gadget*> UI_Tabs{};

			static void DrawRight(j19gadget*, j19action) {
				for (size_t i = 1; i < UI_Tabs.size(); ++i) UI_Tabs[i]->X(UI_Tabs[i - 1]->X() + UI_Tabs[i - 1]->W());
			}
			static void AutoNoBlock(j19gadget*, j19action) {
				UI_BlckNoBlock->Visible = !UI_BlckGroup->Visible;
			}
			static void DrawTab(j19gadget* g, j19action) {
				auto Panel{ UI_TabPanels[g->Caption] };
				Panel->Visible = g->Caption == UI_CurrentTab; 
				Panel->BA = 255;
				Panel->X(0);
				Panel->Y(g->Y() + g->H());
				Panel->H(UI_Right->H() - (Panel->Y()+5));
			}
			static void ActTab(j19gadget* g, j19action) {
				for (size_t i = 0; i < UI_Tabs.size(); ++i) ColorGadgetReverse(UI_Tabs[i], "Tab_" + UI_Tabs[i]->Caption);
				ColorGadget(g, "Tab_" + g->Caption);
				UI_CurrentTab = g->Caption;
			}
			static void AttachImage(j19gadget* p, String ImageFile) {
				if (Assets()->EntryExists(ImageFile)) {
					QCol->Doing("Loading", ImageFile);
					auto Pic{ LoadImage(Assets(),ImageFile) };
					int W{ (int)floor((double)Pic->Width() * ((double)p->H() / (double)Pic->Height())) };
					QCol->Doing("Size", TrSPrintF("%dx%d", W, p->H()));
					auto Gad{ CreatePicture(p->W() - W,0,W,p->H(),p,Pic_FullStretch) };
					Gad->Image(Pic);
				} else {
					QCol->Doing("Skipped", ImageFile);
				}

			}

			static void DrawNavRadio(j19gadget* me, j19action) {
				UI_NavPanels[me->Caption]->Visible = me->checked;
				ColorGadget(
					me,
					me->checked ? "Nav_Checked" : "Nav_UnChecked",
					me->checked ? "White" : "Grey",
					"Black"
				);
			}

			static void NavSelectVolume(j19gadget* vl, j19action) {
				if (vl->SelectedItem() < 0) return;
				auto vn{ vl->ItemText() };
				if (!vn.size()) return;
				auto gt{ AVolPath(vn + ":") };
				Renew(gt);
			}

			static void NavUsedClearDraw(j19gadget* b, j19action) {
				auto mammie{ b->GetParent() };
				b->X(0);
				b->Y(mammie->H() - b->H());
				b->Enabled = UI_NavUsed->NumItems() > 0;
				UI_NavUsed->H((mammie->H() - UI_NavUsed->Y()) - b->H());
				if (mammie->DrawY() + mammie->H() > ScreenHeight() - 3) mammie->H(mammie->H() - 1);
			}

			static void DrawFav(j19gadget* lb, j19action) {
				auto mammie{ lb->GetParent() };
				if (mammie->DrawY() + mammie->H() > ScreenHeight() - 5) mammie->H(mammie->H() - 1);
				UI_NavFavAdd->Y(mammie->H() - UI_NavFavAdd->H());
				UI_NavFavRemove->Y(UI_NavFavAdd->Y());
				UI_NavFavRemove->X(UI_NavFavAdd->X()+UI_NavFavAdd->W() + 2);
				lb->H(UI_NavFavAdd->Y());
				UI_NavFavAdd->Enabled = UI_Resource->Caption[0] != '*';
				UI_NavFavRemove->Enabled = lb->SelectedItem() >= 0;			
			}

			static void Act_AddFav(j19gadget*, j19action) {
				if (CurrentJCRFile() == "") return;
				CFGV("Favorites", StripDir(CurrentJCRFile()), CurrentJCRFile());
				UpdateFavorites(UI_NavFav);
			}

			static void Draw_Extract(j19gadget* g, j19action) {
				UI_ExtractAll->X(UI_Extract->W() + 5);
				g->Y(UI_FileList->Y() + UI_FileList->H() + 2);
				g->Enabled = g->Caption=="Extract" ? CurrentJCRFile().size() && UI_FileList->ItemText().size() && (!Suffixed(UI_FileList->ItemText(),"/")) : CurrentJCRFile().size();
			}

#define DataTab 150
#define NavWidth 300
#define DataField(gadget,Caption) ColorGadget(CreateLabel(Caption,2,y,DataTab,HeadHeight,CPan),"Tab_Data"); gadget=CreateLabel("--",DataTab,y,CPan->W(),HeadHeight,CPan); ColorGadget(gadget,"FLD_DATA","Yellow","Black"); y+=HeadHeight
#define BlckField(gadget,Caption) ColorGadget(CreateLabel(Caption,2,y,DataTab,HeadHeight,UI_BlckGroup),"Tab_Block"); gadget=CreateLabel("--",DataTab,y,CPan->W(),HeadHeight,UI_BlckGroup); ColorGadget(gadget,"FLD_BLOCK","180,255,0","Black"); y+=HeadHeight

			void UI_Init() {				
				// Main
				const int HeadHeight{ 16 };
				const int Tab{ 150 };
				Graphics((int)floor(DesktopWidth() * .95), (int)floor(DesktopHeight() * .80), "Gyermo");
				QCol->Doing("Font", "Fonts/DOS.jfbf");
				UI_Font = LoadImageFont(Assets(), "Fonts/DOS.jfbf");
				j19gadget::SetDefaultFont(UI_Font);
				UI_Work = WorkScreen();
				UI_Work->Visible = true;
				UI_Background = CreatePicture(0, 0, UI_Work->W(), UI_Work->H(),UI_Work, Pic_FullStretch);
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

				// Nav Panel - General
				UI_Nav = CreateGroup(0, HeadHeight * 3, NavWidth, UI_Work->H() - (HeadHeight * 4), UI_Work);
				UI_NavPanels["Volumes"] = CreateListBox(0, HeadHeight * UI_NavRadio.size(), UI_Nav->W(), UI_Nav->H()-(HeadHeight*UI_NavRadio.size()), UI_Nav);
				for (auto uinp : UI_NavRadio) {
					auto& uin{ uinp.first };
					UI_NavRadio[uin] = CreateRadioButton(uin, 0, 0 , UI_Nav->W(), HeadHeight, UI_Nav);
					UI_NavPanels[uin] = UI_NavPanels[uin] ? UI_NavPanels[uin] : CreateGroup(0, HeadHeight * UI_NavRadio.size(), UI_Nav->W(), UI_Nav->H() - (HeadHeight * UI_NavRadio.size()), UI_Nav);
					UI_NavRadio[uin]->CBDraw = DrawNavRadio;
				}
				UI_NavRadio["Volumes"]->Y(0); UI_NavRadio["Volumes"]->checked = true;
				UI_NavRadio["Used"]->Y(HeadHeight);
				UI_NavRadio["Favorites"]->Y(HeadHeight * 2);
				// Nav Panel - Volumes
				ColorGadget(UI_NavPanels["Volumes"], "Nav_Volumes", "BrightBlue", "Blue");
				QCol->Doing("Scanning", "Volumes");
				auto V{ Volumes() };
				UI_NavRadio["Volumes"]->ClearItems();
				for (auto vol : *V) UI_NavPanels["Volumes"]->AddItem(vol.first);
				UI_NavPanels["Volumes"]->CBAction = NavSelectVolume;
				// Nav Panel - Used
				UI_NavUsed = CreateListBox(0, 0, UI_NavPanels["Used"]->W(), 0, UI_NavPanels["Used"]);
				UI_NavUsedClear = CreateButton("Clear", 0, 0, UI_NavPanels["Used"]);
				UI_NavUsedClear->CBDraw = NavUsedClearDraw;
				ColorGadget(UI_NavUsed, "Nav_Used", "BrightGreen", "Green");
				ColorGadget(UI_NavUsedClear, "Clear", "Yellow", "Red");
				UI_NavUsed->HData = "Used";
				UpdateUsed(UI_NavUsed);
				UI_NavUsed->CBAction = Act_UFP;
				UI_NavUsedClear->CBAction = ClearUsed;
				// Nav Panel - Favorites
				UI_NavFav = CreateListBox(0, 0, UI_NavPanels["Used"]->W(), 0, UI_NavPanels["Favorites"]);
				UI_NavFavAdd = CreateButton("+", 0, 0, UI_NavPanels["Favorites"]);
				UI_NavFavRemove = CreateButton("-", 0, 0, UI_NavPanels["Favorites"]);
				UI_NavFav->CBDraw = DrawFav;
				UI_NavFav->CBAction = Act_UFP;
				UI_NavFav->HData = "Favorites";
				ColorGadget(UI_NavFav, "FAV.LIST", "BrightMagenta", "Magenta");
				ColorGadget(UI_NavFavAdd, "FAV.Add", "BrightGreen", "Green");
				ColorGadget(UI_NavFavRemove, "FAV.Remove", "Pink", "Red");
				UI_NavFavAdd->CBAction = Act_AddFav;
				UI_NavFavRemove->CBAction = RemoveFavorite;
				UpdateFavorites(UI_NavFav);

				// Left Panel
				UI_Left = CreateGroup(NavWidth, HeadHeight * 3, (UI_Work->W() / 2)-NavWidth, UI_Work->H() - (HeadHeight * 3), UI_Work);
				UI_Right = CreateGroup(UI_Work->W() / 2, HeadHeight * 3, UI_Work->W() / 2, UI_Work->H() - (HeadHeight * 4), UI_Work);
				UI_FileList = CreateListBox(5, 5, UI_Left->W() - 35, UI_Left->H() - 100, UI_Left);
				UI_FileList->CBAction = Act_FileList;
				ColorGadget(UI_FileList, "FileList", "White", "BrightBlue");
				UI_Extract = CreateButton("Extract", 0, 0, UI_Left);
				UI_ExtractAll = CreateButton("Extract All", 0, 0, UI_Left);
				UI_Extract->CBAction = ExtractButton;
				UI_ExtractAll->CBAction = ExtractAllButton;

				ColorGadget(UI_Extract, "Extract", "Yellow", "Brown");
				ColorGadget(UI_ExtractAll, "Extract", "Yellow", "Brown");
				UI_Extract->CBDraw = Draw_Extract;
				UI_ExtractAll->CBDraw = Draw_Extract;

				// Right Panel
				UI_TabData = CreateButton("Data", 0, 0, UI_Right);
				UI_TabData->Enabled = true;
				ColorGadgetReverse(UI_TabData, "Tab_Data", "255,180,0", "25,18,0");
				UI_TabBlock = CreateButton("Block", 0, 0, UI_Right);
				ColorGadgetReverse(UI_TabBlock, "Tab_Block", "0,255,0", "0,25,0");
				UI_TabView = CreateButton("View", 0, 0, UI_Right);
				ColorGadgetReverse(UI_TabView, "Tab_View", "Grey", "Black");
				j19gadget* _tabs[3]{ UI_TabData,UI_TabBlock,UI_TabView };
				UI_Tabs = UniqueArray<j19gadget*>(3,_tabs);
				UI_Right->CBDraw = DrawRight;
				for (size_t i = 0; i < UI_Tabs.size(); ++i) {
					auto NewPanel = CreatePanel(0, 0, UI_Right->W() - 5, 0, UI_Right);
					ColorGadget(NewPanel, "Tab_" + UI_Tabs[i]->Caption);
					UI_TabPanels[UI_Tabs[i]->Caption] = NewPanel;
					UI_Tabs[i]->CBDraw = DrawTab;
					UI_Tabs[i]->CBAction = ActTab;
					DrawTab(UI_Tabs[i], j19action::Activate);
				}
				ActTab(UI_TabData, j19action::Activate);

				// Tab: Data
				auto CPan{ UI_TabPanels["Data"] };
				AttachImage(CPan, "GFX/Crystal.png");				
				auto y{ 2 };
				DataField(UI_DataEntry, "Entry:"); UI_DataFields["$__Entry"] = UI_DataEntry;
				DataField(UI_DataMain, "Main File:");
				DataField(UI_DataType, "Type:");
				DataField(UI_DataSize, "Size:"); UI_DataFields["%__Size"] = UI_DataSize;
				DataField(UI_DataFields["%__CSize"], "Compressed size:");
				DataField(UI_DataRatio, "Ratio:");
				DataField(UI_DataFields["$__Storage"], "Storage:");
				DataField(UI_DataFields["%__Block"], "Block:");
				DataField(UI_DataFields["%__Offset"], "Offset:");
				DataField(UI_DataFields["%__CHMOD"], "CHMOD:");
				DataField(UI_DataFields["$__JCR6FOR"], "JCR6 Library:");
				DataField(UI_DataFields["$__OS"], "Platform:");
				DataField(UI_DataFields["$__Author"], "Author:");
				DataField(UI_DataFields["$__Notes"], "Notes:");
				DataField(UI_DataFields["$__MD5HASH"], "MD5");
				UI_DataAliasLabel = CreateLabel("Aliases:", 2, y, DataTab, HeadHeight, CPan);
				UI_DataAlias = CreateListBox(DataTab, y, CPan->W() - (DataTab + 100), HeadHeight * 4, CPan);
				ColorGadget(UI_DataAliasLabel, "Tab_Data");
				ColorGadget(UI_DataAlias, "Alias", "Yellow", "Brown");
				y += (HeadHeight * 4)+2;
				UI_DataOtherLabel = CreateLabel("Other Data:", 2, y, DataTab, HeadHeight, CPan);
				UI_DataOther = CreateListBox(DataTab, y, CPan->W() - (DataTab + 100), HeadHeight * 4, CPan);
				ColorGadget(UI_DataOtherLabel, "Tab_Data");
				ColorGadget(UI_DataOther, "Other", "Pink", "Red");
				y += (HeadHeight * 4) + 2;
				DataField(UI_DataResourceType, "Resource Type:");

				// Tab: Block
				CPan = UI_TabPanels["Block"];
				CPan->CBDraw = AutoNoBlock;
				AttachImage(CPan, "GFX/Wendicka.png");
				UI_BlckNoBlock = CreateLabel("This entry is not part of a block", 0, 0, 0, 0, CPan); ColorGadget(UI_BlckNoBlock, "Tab_Block");
				UI_BlckNoBlock->Visible = false;
				UI_BlckGroup = CreateGroup(0, 0, CPan->W(), CPan->H(), CPan);
				y = 2;
				BlckField(UI_BlckEntry, "Entry:");
				BlckField(UI_BlckID, "Block ID:");
				//printf("DEBUG BLCKID: %d:> (%d,%d); FG:%02x%02x%02x;FA:%02x\n", UI_BlckID->ID(), UI_BlckID->X(), UI_BlckID->Y(), UI_BlckID->FR, UI_BlckID->FG, UI_BlckID->FB, UI_BlckID->FA);
				ColorGadget(CreateLabel("Entries:", 2, y, DataTab, HeadHeight, UI_BlckGroup), "Tab_Block");
				UI_BlckEntries = CreateListBox(DataTab, y,  CPan->W() - (DataTab + 200), HeadHeight * 4, UI_BlckGroup);
				ColorGadget(UI_BlckEntries, "BLOCK_ENTRIES", "BrightCyan", "Cyan");
				y += HeadHeight * 4;
				BlckField(UI_BlckEntryOffset, "Entry Offset:");
				BlckField(UI_BlockFields["%__Offset"], "Block Offset:");
				BlckField(UI_BlockFields["%__Size"], "Block Size:");
				BlckField(UI_BlockFields["%__CSize"], "Block Comp. Size:");
				BlckField(UI_BlockFields["$__Storage"], "Block Storage:");
				BlckField(UI_BlckRatio, "Block Ratio:");
				BlckField(UI_BlockFields["$__JCR6FOR"], "JCR6 Library:");
				BlckField(UI_BlockFields["$__MD5HASH"], "MD5:");
				ColorGadget(CreateLabel("Other Data:", 2, y, DataTab, HeadHeight, UI_BlckGroup), "Tab_Block");
				UI_BlckOther = CreateListBox(DataTab, y, CPan->W() - (DataTab + 200), HeadHeight * 4, UI_BlckGroup);
				ColorGadget(UI_BlckOther, "Other");
				CPan = UI_TabPanels["View"];
				ColorGadget(CreateLabel("Entry:", 2, 2, DataTab, HeadHeight, CPan), "Tab_View");
				UI_ViewEntry = CreateLabel("--", DataTab, 2, DataTab, HeadHeight, CPan);
				ColorGadget(UI_ViewEntry, "Fld_View", "White", "Black");
				UI_ViewText = CreatePanel(0, HeadHeight, CPan->W(), CPan->H() - HeadHeight, CPan);
				ViewInit(UI_ViewText);
				UI_ViewPicture = CreatePanel(0, HeadHeight, CPan->W(), CPan->H() - HeadHeight, CPan);
				UI_ViewPictureImage = CreatePicture(0, 0, CPan->W(), CPan->H() - 20, UI_ViewPicture,Pic_Fit);
				UI_ViewPictureLabel = CreateLabel("--", 0, 0, CPan->W(), HeadHeight, UI_ViewPicture);
				ColorGadget(UI_ViewPictureLabel, "Tab_View");
				UI_ViewPictureLabel->BA = 25;
				UI_ViewAudio = CreatePanel(0, HeadHeight, CPan->W(), CPan->H() - HeadHeight, CPan);
				UI_AudioPlay = CreateButton("Play", 25, 25, UI_ViewAudio);
				UI_AudioLoop = CreateButton("Loop", 25, 50, UI_ViewAudio);
				UI_AudioStop = CreateButton("Stop", 25, 75, UI_ViewAudio);
				UI_AudioPlay->SetForeground(0, 255, 0, 255);
				UI_AudioPlay->SetBackground(0, 25, 0, 255);
				UI_AudioLoop->SetForeground(255, 180, 0, 255);
				UI_AudioLoop->SetBackground(25, 18, 0, 255);
				UI_AudioStop->SetForeground(255, 0, 0, 255);
				UI_AudioStop->SetBackground(25, 0, 0, 255);
				UI_AudioPlay->CBAction = ViewAudioCallBack;
				UI_AudioLoop->CBAction = ViewAudioCallBack;
				UI_AudioStop->CBAction = ViewAudioCallBack;
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
			void UI_Done() {
				QCol->Doing("Releasing", "June19");
				FreeJune19();
			}
		}
	}
}
