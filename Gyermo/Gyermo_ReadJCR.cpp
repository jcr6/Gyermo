// License:
// 
// Gyermo
// JCR6 analysis
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

#include "Gyermo_ReadJCR.hpp"
#include "Gyermo_GUI.hpp"
#include <JCR6_Core.hpp>
#include <SlyvQCol.hpp>
#include <SlyvDir.hpp>
#include <TQSE.hpp>
using namespace Slyvina::TQSE;
using namespace Slyvina::Units;
using namespace Slyvina::June19;

namespace Slyvina { 
	namespace JCR6 { 
		namespace Gyermo {
			static std::string __CurrentPath;
			static std::string __CurrentJCRFile;
			static JT_Dir __CurrentJCR{ nullptr };

			void Renew() {
				UI_FileList->ClearItems();
				UI_FileList->AddItem("../");
				if (__CurrentJCRFile.size()) {
					std::vector<std::string> dirs{  };
					auto EL{ __CurrentJCR->Entries() };
					auto PWDS{ Split(__CurrentPath,'/') };
					UI_DataResourceType->Caption = _JT_Dir::Recognize(__CurrentJCRFile);
					if (!EL) { Notify("Error! Entry list turned out to be a NULL pointer!"); return; }
					for (auto E : *EL) {
						//if (!VectorContains(dirs, ExtractDir(E->Name()))) dirs.push_back(ExtractDir(E->Name()));
						auto p{ ExtractDir(Upper(E->Name())) };
						while (p.size()) {
							VectorAddUnique(dirs, p);
							if (IndexOf(p, '/') < 0) break;
							p = ExtractDir(p);
						}
					}
					for (auto d : dirs) {
						if (ExtractDir(Upper(d)) == Upper(__CurrentPath)) UI_FileList->AddItem(StripDir(d) + "/");
						/*
						if (IndexOf(d, '/') >= 0) {
							auto s{ Split(ExtractDir(d), '/') };
							//printf("Split %s:%d    (Must be %s:%d)\n", d.c_str(), s->size(), __CurrentPath.c_str(), PWDS->size());
							//if (s->size() == PWDS->size()) {
							//	auto j{ StringJoin("/",*s,PWDS->size()) };
							//	if(j==__CurrentPath)UI_FileList->AddItem(StripDir(d) + "/");								
							//}							
							for (size_t l = 0; l < s->size(); l++) {
								auto j{ StringJoin("/",*s,l) };
								auto js{ StripDir(j) };
								printf("j=%s; js=%s;\n",j.c_str(),js.c_str());
								if (ExtractDir(j) == __CurrentPath) UI_FileList->AddItem(js + "/");
							}

						} else if (__CurrentPath=="") {
							UI_FileList->AddItem(d + "/");
						}
						//*/
					}
					for (auto E : *EL) {
						auto d{ E->Name() };
						if (ExtractDir(Upper(d)) == Upper(__CurrentPath)) {
							//QCol->Doing("Goedgekeurd", d);
							UI_FileList->AddItem(StripDir(d));
						} 
						//else { QCol->Doing("Afgekeurd", d); }

					}
					UI_Resource->Caption = __CurrentJCRFile;
					UI_Directory->Caption = __CurrentPath;
				} else {
					auto dirs{ FileList(__CurrentPath,DirWant::Directories) };
					auto files{ FileList(__CurrentPath) };
					UI_DataResourceType->Caption = "File system";
					for (auto d : *dirs) UI_FileList->AddItem(d+"/");
					for (auto f : *files) UI_FileList->AddItem(f);
					UI_Resource->Caption = "* File System *";
					UI_Directory->Caption = __CurrentPath;
					UI_BlckGroup->Visible = false;
					UI_DataOther->Visible = false;
					UI_DataOtherLabel->Visible = false;
					UI_DataAlias->Visible = false;
					UI_DataAliasLabel->Visible = false;
				}
				UI_FileList->CBAction(UI_FileList, j19action::Select);
			}

			void Renew(std::string Path) {
				if (__CurrentJCRFile.size()) {
					Renew(__CurrentJCRFile, Path);
					return;
				}
				if (IsDir(Path)) {
					__CurrentPath = ChReplace(Path,'\\','/');
					Renew();
				} else if (IsFile(Path)) {
					if (_JT_Dir::Recognize(Path) != "NONE") Renew(Path, "");					
					return;
				} else {
					Notify("No way to access: " + Path);
					exit(404);
				}
			}
			void Renew(std::string JCRFile, std::string Path) {
				if (JCRFile == "") { __CurrentJCRFile = ""; Renew(Path); return; }
				QCol->Doing("Checking", JCRFile, " "); QCol->LMagenta(Path+"\n");
				__CurrentJCR = __CurrentJCR && __CurrentJCRFile == JCRFile ? __CurrentJCR : JCR6_Dir(JCRFile);
				__CurrentJCRFile = JCRFile;
				__CurrentPath = Path;
				Renew();
			}

			std::string GRJType(std::string f) {
				if (Suffixed(f, "/")) return "Directory";
				if (!__CurrentJCRFile.size()) {
					auto R{ _JT_Dir::Recognize(f) };
					if (R != "NONE") return R == "JCR6" ? "JCR6 resource" : R + " (Readable by JCR6)";
				}
				return "File";
			}


			void Act_FileList(June19::j19gadget* f, June19::j19action a) {
				UI_Right->Visible = f->SelectedItem() >= 0;
				if (!UI_Right->Visible) return;
				auto fi{ f->ItemText() };
				auto ff{ __CurrentPath + (__CurrentPath.size() ? "/" : "") + fi };
				switch (a) {				
				case j19action::DoubleClick:
					if (Suffixed(fi, "/")) {
						if (fi == "../") {
							if (__CurrentPath == "" || __CurrentPath == "/") {
								if (__CurrentJCRFile.size()) {
									Renew("", ExtractDir(__CurrentJCRFile));
									return;
								}
							} else {
								__CurrentPath = ExtractDir(__CurrentPath);
								Renew();
								return;

							}						
						} else {
							__CurrentPath += (__CurrentPath.size() ? "/" : "") + fi.substr(0, fi.size() - 1);
							Renew();
							return;
						}
					} else if ((!__CurrentJCRFile.size()) && _JT_Dir::Recognize(ff) != "NONE") {
						Renew(ff, "");
						return;
					}
					// Fallthrough!
				case j19action::Select:
					for (auto u : UI_DataFields) { u.second->Caption = "--"; }
					if (__CurrentJCRFile.size()) {
						if (Suffixed(ff, "/")) {
							UI_DataEntry->Caption = ff;
							UI_DataType->Caption = "Directory";
						} else {
							auto be{ __CurrentJCR->Entries() };
							auto e{ __CurrentJCR->Entry(ff) };
							UI_DataOther->ClearItems();
							for (auto ad : e->_ConfigBool) { if (UI_DataFields.count("&" + ad.first)) UI_DataFields["&" + ad.first]->Caption = boolstring(ad.second); else UI_DataOther->AddItem("bool   " + ad.first + " = " + boolstring(ad.second)); }
							for (auto ad : e->_ConfigInt) { if (UI_DataFields.count("%" + ad.first)) UI_DataFields["%" + ad.first]->Caption = std::to_string(ad.second); else UI_DataOther->AddItem("int    " + ad.first + " = " + std::to_string(ad.second)); }
							for (auto ad : e->_ConfigString) { if (UI_DataFields.count("$" + ad.first)) UI_DataFields["$" + ad.first]->Caption = ad.second; else UI_DataOther->AddItem("string " + ad.first + " = " + ad.second); }
							UI_DataOther->Visible = UI_DataOther->NumItems() > 0;
							UI_DataOtherLabel->Visible = UI_DataOther->Visible;
							UI_DataMain->Caption = e->MainFile;
							UI_DataAlias->ClearItems();
							for (auto ae : *be) {
								if (
									ae->Name() != e->Name() &&
									ae->RealSize() == e->RealSize() &&
									ae->CompressedSize() == e->CompressedSize() &&
									ae->Offset() == e->Offset() &&
									ae->MainFile == e->MainFile &&
									ae->Block() == e->Block()
									) UI_DataAlias->AddItem(ae->Name());
							}
							UI_DataAlias->Visible = UI_DataAlias->NumItems() > 0;
							UI_DataAliasLabel->Visible = UI_DataAlias->Visible;
							if (e->Block()) {
								UI_DataFields["%__CSize"]->Caption = "N/A";
								UI_DataRatio->Caption = "N/A";
								UI_BlckGroup->Visible = true;
								//for (auto k : __CurrentJCR->Blocks) QCol->Doing("Block:", k.first, TrSPrintF("\t%d\n", k.second->ID)); // debug
								auto b{ __CurrentJCR->Blocks[std::to_string(e->Block()) + ":" + e->MainFile] };
								UI_BlckEntry->Caption = e->Name();
								UI_BlckID->Caption = std::to_string(e->Block());
								UI_BlckEntries->ClearItems();
								for (auto eb : *be) if (eb->Block() == e->Block() && eb->MainFile == e->MainFile) UI_BlckEntries->AddItem(eb->Name());
								UI_BlckEntryOffset->Caption = TrSPrintF("%x (%d)", e->Offset(), e->Offset());
								UI_BlckOther->ClearItems();
								for (auto ad : b->dataBool) { if (UI_BlockFields.count("&" + ad.first)) UI_BlockFields["&" + ad.first]->Caption = boolstring(ad.second); else UI_BlckOther->AddItem("bool   " + ad.first + " = " + boolstring(ad.second)); }
								for (auto ad : b->dataInt) { if (UI_BlockFields.count("%" + ad.first)) UI_BlockFields["%" + ad.first]->Caption = std::to_string(ad.second); else UI_BlckOther->AddItem("int    " + ad.first + " = " + std::to_string(ad.second)); }
								for (auto ad : b->dataString) { if (UI_BlockFields.count("$" + ad.first)) UI_BlockFields["$" + ad.first]->Caption = ad.second; else UI_BlckOther->AddItem("string " + ad.first + " = " + ad.second); }
								UI_BlckOther->Visible = UI_BlckOther->NumItems() > 0;
								UI_BlckRatio->Caption = b->dataString["__Storage"] == "Store" ? "N/A" : TrSPrintF("%5.1f%%", ((double)b->CompressedSize() / (double)b->RealSize()) * 100);
							} else {
								UI_BlckGroup->Visible = false;
							}
						}
					} else {
						auto fsize = FileSize(ff);
						UI_DataEntry->Caption = ff;
						UI_DataSize->Caption = Suffixed(ff, "/") ? "N/A" : (fsize < 10000 ? TrSPrintF("%d bytes", (int)fsize) : (fsize < 10000000 ? TrSPrintF("%d KB", fsize / 1000) : TrSPrintF("%d MB", fsize / 1000000)));
						UI_DataType->Caption = GRJType(ff);
						UI_BlckGroup->Visible = false;
					}
				}
			}
		}
	} 
}
