// License:
// 
// Gyermo
// View Manager
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

#include <Slyvina.hpp>
#include <june19_core.hpp>
#include <SlyvQCol.hpp>
#include <june19_label.hpp>
#include <june19_scrollbar.hpp>
#include <Kitty_High.hpp>
#include "Gyermo_GUI.hpp"
#include "Gyermo_View.hpp"
#include <TQSA.hpp>
using namespace Slyvina::TQSE;
using namespace Slyvina::TQSA;

#undef GVTEXTDEBUG

using namespace Slyvina::TQSG;
using namespace Slyvina::Units;
using namespace Slyvina::Kitty;

using namespace Slyvina::June19;

namespace Slyvina {
	namespace JCR6 {
		namespace Gyermo {
#pragma region Data
			static std::map<String, String> _PicFormats{
				{"png","Portable Network Graphic"},
				{"jpg","Joint Point Expert Group"},
				{"jpeg","Joint Point Expert Group"},
				{"gif","Graphic interlaced file"},
				{"bmp","Bitmap"},
				{"tif","Tag Image File Format"},
				{"tiff","Tag Image File Format"},
				{"webp","Web Picture"}
			};
			static std::map<String, String> _AudioFormats{
				{"wav","Wave"},
				{"mp3","Moving Picture Experts Group - Audio Layer III"},
				{"ogg","Ogg"},
				{"flac","Free Lossless Audio Codec"}
			};
			TAudio _LoadedAudio{ nullptr };
			String _LoadedAudioCode{ "*" }, _RequestedAudioCode{ "" }, _AudioEntry{ "" };
			JT_Dir _AudioResource{ nullptr };

			class GVText;

			static int
				cw{ 0 },
				ch{ 0 },
				cx{ 0 },
				cy{ 0 },
				ctab{ 0 },
				csmy{ 0 }; // Scroll max y
			static Byte
				cr{ 255 },
				cg{ 255 },
				cb{ 255 };

			static String cWord{ "" };
			
			j19gadget* GVTextGadget{ nullptr };
			std::unique_ptr<GVText> First{ nullptr };
			GVText* Last{ nullptr };

			class GVText {
			private:
				bool IsFirst{ false };
			public:
				//std::unique_ptr<GVText> _Next{ nullptr };
				GVText* _Next{ nullptr };
				inline GVText* Next() { return _Next; }
				int x{ 0 }, y{ 0 };
				Byte R{ 0 }, G{ 0 }, B{ 0 };
				String Txt{ "" };
				GVText(String _txt, int _x, int _y, Byte _r=255u, Byte _g=255u, Byte _b=255u) {
					Txt = _txt;
					x = _x;
					y = _y;
					R = _r;
					G = _g;
					B = _b;
					csmy = std::max(csmy, _y - GVTextGadget->H());
					if (!First) {
						First = std::unique_ptr<GVText>(this);
						Last = this;
						IsFirst = false;
					} else {
						Last->_Next = this; //= std::unique_ptr<GVText>(this);
						Last = this;
					}
#ifdef GVTEXTDEBUG
					QCol->Doing("Created GVText", _txt + TrSPrintF("(%x,%x)", x, y));
#endif
				}
				~GVText() {
#ifdef GVTEXTDEBUG
					QCol->Doing("Disposed GVText", Txt + TrSPrintF("(%x,%x)", x, y));

#endif
					if (IsFirst) {
						auto nextvictim{ _Next };
						while (nextvictim) {
							auto victim{ nextvictim };
							nextvictim = victim->_Next;
							delete victim;
						}
					}
				}
			};
#pragma endregion

#pragma region Kitty
			static void View_Kitty_Write(_KittyOutput* SELF, String a) {
				ViewColor((ECol)SELF->ForegroundColor);
				ViewWrite(a);
			}
			static void View_Kitty_WriteLine(_KittyOutput* SELF, String a) { View_Kitty_Write(SELF,a + "\n"); }
#pragma endregion

			void ViewCLS() {				
				GVTextGadget->ScrollY = 0;
				First = nullptr;
				Last = nullptr;
				cWord = "";
				cx = 0;
				cy = 0;
			}

			static void ViewGadget(j19gadget* g, j19action) {
				static auto fnt{ j19gadget::GetDefaultFont() };
				for (auto gt = First.get(); gt; gt = gt->Next()) {
					int dy{ gt->y - (g->ScrollY*ch) }; if (dy<0 || dy + ch>g->H()) continue;
					int x{ gt->x + g->DrawX() };
					int y{ dy + g->DrawY() };
					SetColor(gt->R, gt->G, gt->B);
					SetAlpha(255);
					fnt->Text(gt->Txt,x,y);
				}
				if (g->DrawY() + g->H() > ScreenHeight() - 5) {
					g->H(g->H() - 1); QCol->Doing("Done", "View Gadget Height Correction!");
				}
				if (TQSE::MouseWheelY() != 0 && TQSE::MouseX() > g->DrawX() && TQSE::MouseY() > g->DrawY() && TQSE::MouseX() < g->DrawX() + g->W() && TQSE::MouseY() < g->DrawY() + g->H()) {
					g->SetScrollY(g, g->ScrollY - TQSE::MouseWheelY());
				}
			}

			static void ScrollGadget(j19gadget* g, int value) {
				g->ScrollY = std::max(0, std::min(value, csmy));
			}

			void ViewInit(j19gadget* mammie) {				
				GVTextGadget = CreatePanel(mammie->X(), mammie->Y(), mammie->W(), mammie->H(), mammie);
				GVTextGadget->BA = 0;
				GVTextGadget->CBDraw = ViewGadget;
				GVTextGadget->SetScrollY = ScrollGadget;
				AttachScrollV(GVTextGadget, true);
				auto con{ std::shared_ptr<_KittyOutput>(new _KittyOutput()) };
				con->abstract_Write = View_Kitty_Write;
				con->abstract_WriteLine = View_Kitty_WriteLine;
				_KittyHigh::Console(con);
			}


			static void WriteWord(Byte r=255,Byte g=255,Byte b=255) {
				if (!Trim(cWord).size()) return;
				static auto fnt{ j19gadget::GetDefaultFont() };
				auto w{ fnt->Width(cWord) };
				auto h{ fnt->Height(cWord) };
				cw = std::max(cw, fnt->Width("W"));
				ch = std::max(ch, h);
				ctab = cw * 5;
				if (!GVTextGadget) QCol->Error("INTERAL ERROR: GVTextGadget not defined!\nAs this can only happen thanks to a bug, please report this!");
				if (cx + cw > GVTextGadget->W()) {
					cy = cx == 0 ? cy : cy + ch;
					cx = 0;
				}
#ifdef GVTEXTDEBUG
				QCol->Doing("Write word", cWord);
#endif
				new GVText(cWord, cx, cy, r, g, b);
				cx += fnt->Width(cWord);
				cWord = "";
			}

			inline void QWW() { WriteWord(cr, cg, cb); }

			union c2b { char c; Byte b; char s[3]; };
			void ViewWrite(std::string a) {
#ifdef GVTEXTDEBUG
				QCol->Doing("ViewWrite", a);
#endif
				for (size_t p = 0; p < a.size(); p++) {
					c2b c; c.c = a[p];
					c.s[1] = 0;
#ifdef GVTEXTDEBUG
					printf("%x/%x: %02x\n %c (%s) -> %s\n", p, a.size(), c.b, c.c, c.s, cWord.c_str());
#endif
					switch (c.b) {
					case 0: goto out; // Should never happen, but just in case!
					case 8: WriteWord(cr, cg, cb); cx = std::max(0, cx - cw); break;
					case 7: cWord += "<BEL>"; break;
					case 9: WriteWord(cr, cg, cb); if (ctab) do {} while (++cx % ctab); break;
					case 10: QWW(); cx = 0; cy += ch; break;
					case 12: cWord += "<CLS>"; break;
					case 13: break; // IGNORE!
					case 32: QWW(); cx += cw; break;
					case 124: cWord += "|vl"; break;
					default:
						cWord += c.b >= 32 && c.b <= 126 ? c.s : cWord += TrSPrintF("<0x%02x>", c.b);
						break;
					}
				}
			out:
				if (cWord.size()) QWW();
			}

			bool IsBinary(Units::Bank B) {
				for (size_t p = 0; p < B->Size(); p++) {
					auto b{ B->PeekByte(p) };
					if (b < 32 && b != 13 && b != 9 && b != 10) return true;
					if (b > 127) return true;
				}
				return false;
			}

			void ViewPlainText(Units::Bank B) {
				ViewCLS();
				cr = 255;
				cg = 255;
				cb = 255;
				ViewWrite(B->ToString());
			}

			void ViewColor(ECol c) {
				auto gc{ MCol[c] };
				cr = gc.R;
				cg = gc.G;
				cb = gc.B;
			}

			void ViewHex(Units::Bank B) {
				ViewCLS();
				//ViewWriteLn("Hex view comes later!");
				ViewColor(ECol::BrightMagenta);
				ViewWrite(".........  ");
				ViewColor(ECol::White);
				for (Byte i = 0; i < 16; ++i) {
					if (i % 4 == 0) ViewWrite(" ");
					ViewWrite(TrSPrintF("%02x ", i));
				} ViewWrite("\n");
				char OV[17]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
				for (size_t p = 0; p < B->Size(); ++p) {
					auto ch{ B->PeekByte(p) };
					if (p % 16 == 0) { 
						if (p) { ViewColor(ECol::BrightGreen); ViewWrite("  "+(String)OV); for (Byte i = 0; i < 16; i++) OV[i] = 0; }
						ViewColor(ECol::White); ViewWrite(TrSPrintF("\n%09x ", p)); 
					}
					if (p % 4 == 0) ViewWrite(" ");
					ViewColor((p / 4) % 2 == 0 ? ECol::BrightCyan : ECol::Cyan);
					ViewWrite(TrSPrintF(" %02x", ch));
					OV[p % 16] = ch >= 32 && ch < 127 && ch!=124 ? (char)ch : '.';
				}
			}

			static void Loading() {
				Cls();
				SetColor(255, 255, 85);
				j19gadget::GetDefaultFont()->Text("Loading", ScreenWidth() / 2, ScreenHeight() / 2, Align::Center);
				Flip();
				QCol->Doing("Note", "Apparently this can take awhile");
			}

			static Bank LB(std::string entry) {
				auto B{ ReadFile(entry) };
				//auto R{ CreateXBank() };
				//while (!B->EndOfFile()) R->Write(B->ReadByte());
				auto R{ CreateBank(B->Size()) };
				for (size_t p = 0; p < B->Size(); p++) R->Poke(p, B->ReadByte());
				B->Close();
				return R;
			}
			static Bank GetB(JCR6::JT_Dir JD, std::string entry) {
				auto E{ JD ? JD->Entry(entry) : nullptr };
				auto Size{ JD ? E->RealSize() : (int)FileSize(entry) };
				QCol->Doing("Getting", entry);
				if (Size > 10000) Loading();
				return JD ? JD->B(entry) : LB(entry);
			}

			void ViewAudioCallBack(June19::j19gadget* g, June19::j19action) {
				if (g->Caption == "Stop") {
					Mix_HaltChannel(1);
					return;
				} 
				if (_LoadedAudioCode != _RequestedAudioCode || !_LoadedAudio) {
					_LoadedAudio = _AudioResource ? LoadAudio(_AudioResource, _AudioEntry) : LoadAudio(_AudioEntry);
					if (!_LoadedAudio) { Notify("Loading audio failed!"); return; }
					_LoadedAudioCode = _RequestedAudioCode;
				}
				if (!_LoadedAudio) { Notify("Audio is null"); return; }
				_LoadedAudio->ChPlay(1, g->Caption == "Loop" ? -1 : 0);
			}

			void ViewEntry(JCR6::JT_Dir JD, std::string entry) {
				auto E{ JD ? JD->Entry(entry) : nullptr };
				auto Size{ JD ? E->RealSize() : (int)FileSize(entry) };
				auto Ext{ ExtractExt(entry) };
				if ((!JD) && _JT_Dir::Recognize(entry) != "NONE") {
					QCol->Doing("View", "File recognized as JCR6 compatible");
					ViewCLS();
					cr = 0; cr = 180; cb = 255;
					ViewWriteLn("Recognized by JCR6 as: " + _JT_Dir::Recognize(entry));
					UI_ViewText->Visible = true;
					UI_ViewAudio->Visible = false;
					UI_ViewPicture->Visible = false;
					return;
				}
				KittyHigh KittyView{ nullptr };
				if (_KittyHigh::Langs.count("WF:" + StripDir(entry))) KittyView = _KittyHigh::Langs["WF:" + entry];
				if (_KittyHigh::Langs.count(Lower(Ext))) KittyView = _KittyHigh::Langs[Lower(Ext)];
				if (KittyView) {
					ViewCLS();
					ViewColor(ECol::Yellow); ViewWrite("Code: ");
					ViewColor(ECol::BrightCyan); ViewWrite(KittyView->Language + "\n\n");
					QCol->Doing("Kitty", KittyView->Language);
					KittyView->Show(JD ? JD->GetString(entry) : FLoadString(entry), true);
					UI_ViewText->Visible = true;
					UI_ViewAudio->Visible = false;
					UI_ViewPicture->Visible = false;
					return;
				}
				if (_PicFormats.count(Lower(Ext))) {
					auto img{ JD ? LoadImage(JD,entry) : LoadImage(entry) };
					UI_ViewPictureLabel->Caption = _PicFormats[Lower(Ext)] + "  " + (img ? TrSPrintF("%dx%d", img->Width(), img->Height()) : " LOADING FAILED!");
					UI_ViewPictureImage->Image(img);

					UI_ViewText->Visible = false;
					UI_ViewAudio->Visible = false;
					UI_ViewPicture->Visible = true;
					return;
				}
				if (_AudioFormats.count(Lower(Ext))) {
					_RequestedAudioCode = JD ? JD->Entry(entry)->MainFile + "::" + entry : "*FileSystem*::" + entry;
					_AudioEntry = entry;
					_AudioResource = JD;
					UI_ViewText->Visible = false;
					UI_ViewAudio->Visible = true;
					UI_ViewPicture->Visible = false;
					return;
				}
				// If nothing else works then do this.

				if (Size >= 500000000) {
					ViewCLS();
					ViewColor(ECol::Red);
					ViewWriteLn("File too big to view properly");
				} else {
					auto bnk{ GetB(JD, entry) };
					if (IsBinary(bnk)) {
						QCol->Doing("Hex", entry);;
						ViewHex(bnk);
					} else {
						QCol->Doing("Plain Text", entry);
						ViewPlainText(bnk);
					}
				}
				UI_ViewText->Visible = true;
				UI_ViewAudio->Visible = false;
				UI_ViewPicture->Visible = false;
			}

			String PicFormat(String f) {
				Trans2Lower(f);
				auto e{ ExtractExt(f) };
				return _PicFormats.count(e) ? _PicFormats[e] : "";
			}

			String AudioFormat(String f) {
				Trans2Lower(f);
				auto e{ ExtractExt(f) };
				return _AudioFormats.count(e) ? _AudioFormats[e] : "";
			}

		}
	}
}
