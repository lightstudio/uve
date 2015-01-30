#include "pch.h"
//#include "NativeScript.h"
//using namespace UVEngineNative;
//
//static NativeScript::FuncLUT func_lut[] = {
//    {"zenkakko",		&NativeScript::zenkakkoCommand},
//
//    {"yesnobox",		&NativeScript::yesnoboxCommand},
//
//    {"windoweffect",	&NativeScript::effectCommand},
//    {"windowchip",		&NativeScript::windowchipCommand},
//    {"windowback",		&NativeScript::windowbackCommand},
//    {"wavestop",		&NativeScript::wavestopCommand},
//    {"waveloop",		&NativeScript::waveCommand},
//    {"wave",			&NativeScript::waveCommand},
//    {"waittimer",		&NativeScript::waittimerCommand},
//    {"wait",			&NativeScript::waitCommand},
//
//    {"vsp2",			&NativeScript::vspCommand},
//    {"vsp",				&NativeScript::vspCommand},
//    {"voicevol",		&NativeScript::voicevolCommand},
//    {"versionstr",		&NativeScript::versionstrCommand},
//
//    {"usewheel",		&NativeScript::usewheelCommand},
//    {"useescspc",		&NativeScript::useescspcCommand},
//    {"underline",		&NativeScript::underlineCommand},
//
//    {"trap",			&NativeScript::trapCommand},
//    {"transmode",		&NativeScript::transmodeCommand},
//    {"transbtn",		&NativeScript::transbtnCommand},
//    {"time",			&NativeScript::timeCommand},
//    {"textspeeddefault",&NativeScript::textspeeddefaultCommand},
//    {"textspeed",		&NativeScript::textspeedCommand},
//    {"textshow",		&NativeScript::textshowCommand},
//    {"texton",			&NativeScript::textonCommand},
//    {"textoff",			&NativeScript::textoffCommand},
//    {"texthide",		&NativeScript::texthideCommand},
//    {"textgosub",		&NativeScript::textgosubCommand},
//    {"textclear",		&NativeScript::textclearCommand},
//    {"textbtnwait",		&NativeScript::btnwaitCommand},
//    {"texec",			&NativeScript::texecCommand},
//    {"tateyoko",		&NativeScript::tateyokoCommand},
//    {"tan",				&NativeScript::tanCommand},
//    {"tal",				&NativeScript::talCommand},
//    {"tablegoto",		&NativeScript::tablegotoCommand},
//
//    {"systemcall",		&NativeScript::systemcallCommand},
//    {"sub",				&NativeScript::subCommand},
//    {"strsph",			&NativeScript::strspCommand},
//    {"strsp",			&NativeScript::strspCommand},
//    {"stralias",		&NativeScript::straliasCommand},
//    {"stop",			&NativeScript::stopCommand},
//    {"sp_rgb_gradation",&NativeScript::sp_rgb_gradationCommand},
//    {"spstr",			&NativeScript::spstrCommand},
//    {"spreload",		&NativeScript::spreloadCommand},
//    {"splitstring",		&NativeScript::splitCommand},
//    {"split",			&NativeScript::splitCommand},
//    {"spi",				&NativeScript::soundpressplginCommand},
//    {"spclclk",			&NativeScript::spclclkCommand},
//    {"spbtn",			&NativeScript::spbtnCommand},
//    {"soundpressplgin",	&NativeScript::soundpressplginCommand},
//    {"skipoff",			&NativeScript::skipoffCommand},
//    {"skip",			&NativeScript::skipCommand},
//    {"sin",				&NativeScript::sinCommand},
//    {"shadedistance",	&NativeScript::shadedistanceCommand},
//    {"sevol",			&NativeScript::sevolCommand},
//    {"setwindow3",		&NativeScript::setwindow3Command},
//    {"setwindow2",		&NativeScript::setwindow2Command},
//    {"setwindow",		&NativeScript::setwindowCommand},
//    {"setkinsoku",		&NativeScript::setkinsokuCommand},
//    {"setcursor",		&NativeScript::setcursorCommand},
//    {"selnum",			&NativeScript::selectCommand},
//    {"selgosub",		&NativeScript::selectCommand},
//    {"selectvoice",		&NativeScript::selectvoiceCommand},
//    {"selectcolor",		&NativeScript::selectcolorCommand},
//    {"selectbtnwait",	&NativeScript::btnwaitCommand},
//    {"select",			&NativeScript::selectCommand},
//    {"savetime",		&NativeScript::savetimeCommand},
//    {"savescreenshot2",	&NativeScript::savescreenshotCommand},
//    {"savescreenshot",	&NativeScript::savescreenshotCommand},
//    {"saveon",			&NativeScript::saveonCommand},
//    {"saveoff",			&NativeScript::saveoffCommand},
//    {"savenumber",		&NativeScript::savenumberCommand},
//    {"savename",		&NativeScript::savenameCommand},
//    {"savegame2",		&NativeScript::savegameCommand},
//    {"savegame",		&NativeScript::savegameCommand},
//    {"savefileexist",	&NativeScript::savefileexistCommand},
//    {"sar",				&NativeScript::nsaCommand},
//
//    {"rubyon",			&NativeScript::rubyonCommand},
//    {"rubyoff",			&NativeScript::rubyoffCommand},
//    {"roff",			&NativeScript::roffCommand},
//    {"rnd2",			&NativeScript::rndCommand},
//    {"rnd",				&NativeScript::rndCommand},
//    {"rmode",			&NativeScript::rmodeCommand},
//    {"rmenu",			&NativeScript::rmenuCommand},
//    {"return",			&NativeScript::returnCommand},
//    {"resettimer",		&NativeScript::resettimerCommand},
//    {"reset",			&NativeScript::resetCommand},
//    {"repaint",			&NativeScript::repaintCommand},
//
//    {"quakey",			&NativeScript::quakeCommand},
//    {"quakex",			&NativeScript::quakeCommand},
//    {"quake",			&NativeScript::quakeCommand},
//
//    {"puttext",			&NativeScript::puttextCommand},
//    {"prnumclear",		&NativeScript::prnumclearCommand},
//    {"prnum",			&NativeScript::prnumCommand},
//    {"print",			&NativeScript::printCommand},
//    {"pretextgosub",	&NativeScript::pretextgosubCommand},
//    {"playstop",		&NativeScript::playstopCommand},
//    {"playonce",		&NativeScript::playCommand},
//    {"play",			&NativeScript::playCommand},
//    {"pagetag",			&NativeScript::pagetagCommand},
//
//    {"okcancelbox",		&NativeScript::yesnoboxCommand},
//    {"ofscpy",			&NativeScript::ofscopyCommand},
//    {"ofscopy",			&NativeScript::ofscopyCommand},
//
//    {"numalias",		&NativeScript::numaliasCommand},
//    {"nsadir",			&NativeScript::nsadirCommand},
//    {"nsa",				&NativeScript::nsaCommand},
//    {"notif",			&NativeScript::ifCommand},
//    {"next",			&NativeScript::nextCommand},
//    {"nsa",				&NativeScript::arcCommand},
//    {"ns3",				&NativeScript::nsaCommand},
//    {"ns2",				&NativeScript::nsaCommand},
//    {"nega",			&NativeScript::negaCommand},
//
//    {"mul",				&NativeScript::mulCommand},
//    {"msp2",			&NativeScript::mspCommand},
//    {"msp",				&NativeScript::mspCommand},
//    {"mpegplay",		&NativeScript::mpegplayCommand},
//    {"mp3vol",			&NativeScript::mp3volCommand},
//    {"mp3stop",			&NativeScript::mp3stopCommand},
//    {"mp3save",			&NativeScript::mp3Command},
//    {"mp3loop",			&NativeScript::mp3Command},
//    {"mp3fadeout",		&NativeScript::mp3fadeoutCommand},
//    {"mp3fadein",		&NativeScript::mp3fadeinCommand},
//    {"mp3",				&NativeScript::mp3Command},
//    {"movl",			&NativeScript::movCommand},
//    {"movie",			&NativeScript::movieCommand},
//    {"movemousecursor",	&NativeScript::movemousecursorCommand},
//    {"mov9",			&NativeScript::movCommand},
//    {"mov8",			&NativeScript::movCommand},
//    {"mov7",			&NativeScript::movCommand},
//    {"mov6",			&NativeScript::movCommand},
//    {"mov5",			&NativeScript::movCommand},
//    {"mov4",			&NativeScript::movCommand},
//    {"mov3",			&NativeScript::movCommand},
//    {"mov10",			&NativeScript::movCommand},
//    {"mov",				&NativeScript::movCommand},
//    {"monocro",			&NativeScript::monocroCommand},
//    {"mode_saya",		&NativeScript::mode_sayaCommand},
//    {"mode_ext",		&NativeScript::mode_extCommand},
//    {"mod",				&NativeScript::modCommand},
//    {"mid",				&NativeScript::midCommand},
//    {"menu_window",		&NativeScript::menu_windowCommand},
//    {"menu_full",		&NativeScript::menu_fullCommand},
//    {"menu_automode",	&NativeScript::menu_automodeCommand},
//    {"menusetwindow",	&NativeScript::menusetwindowCommand},
//    {"menuselectvoice",	&NativeScript::menuselectvoiceCommand},
//    {"menuselectcolor",	&NativeScript::menuselectcolorCommand},
//    {"maxkaisoupage",	&NativeScript::maxkaisoupageCommand},
//
//    {"luasub",			&NativeScript::luasubCommand},
//    {"luacall",			&NativeScript::luacallCommand},
//    {"lsph2sub",		&NativeScript::lsp2Command},
//    {"lsph2add",		&NativeScript::lsp2Command},
//    {"lsph2",			&NativeScript::lsp2Command},
//    {"lsph",			&NativeScript::lspCommand},
//    {"lsp2sub",			&NativeScript::lsp2Command},
//    {"lsp2add",			&NativeScript::lsp2Command},
//    {"lsp2",			&NativeScript::lsp2Command},
//    {"lsp",				&NativeScript::lspCommand},
//    {"lr_trap",			&NativeScript::trapCommand},
//    {"lrclick",			&NativeScript::clickCommand},
//    {"loopbgmstop",		&NativeScript::loopbgmstopCommand},
//    {"loopbgm",			&NativeScript::loopbgmCommand},
//    {"lookbacksp",		&NativeScript::lookbackspCommand},
//    {"lookbackflush",	&NativeScript::lookbackflushCommand},
//    {"lookbackcolor",	&NativeScript::lookbackcolorCommand},
//    {"lookbackbutton",	&NativeScript::lookbackbuttonCommand},
//    {"logsp2",			&NativeScript::logspCommand},
//    {"logsp",			&NativeScript::logspCommand},
//    {"locate",			&NativeScript::locateCommand},
//    {"loadgosub",		&NativeScript::loadgosubCommand},
//    {"loadgame",		&NativeScript::loadgameCommand},
//    {"linepage2",		&NativeScript::linepageCommand},
//    {"linepage",		&NativeScript::linepageCommand},
//    {"len",				&NativeScript::lenCommand},
//    {"ld",				&NativeScript::ldCommand},
//    {"labellog",		&NativeScript::labellogCommand},
//
//    {"kinsoku",			&NativeScript::kinsokuCommand},
//
//    {"jumpf",			&NativeScript::jumpfCommand},
//    {"jumpb",			&NativeScript::jumpbCommand},
//
//    {"kidokuskip",		&NativeScript::kidokuskipCommand},
//    {"kidokumode",		&NativeScript::kidokumodeCommand},
//
//    {"itoa2",			&NativeScript::itoaCommand},
//    {"itoa",			&NativeScript::itoaCommand},
//    {"isskip",			&NativeScript::isskipCommand},
//    {"ispage",			&NativeScript::ispageCommand},
//    {"isfull",			&NativeScript::isfullCommand},
//    {"isdown",			&NativeScript::isdownCommand},
//    {"intlimit",		&NativeScript::intlimitCommand},
//    {"input",			&NativeScript::inputCommand},
//    {"indent",			&NativeScript::indentCommand},
//    {"inc",				&NativeScript::incCommand},
//    {"if",				&NativeScript::ifCommand},
//
//    {"humanz",			&NativeScript::humanzCommand},
//    {"humanorder",		&NativeScript::humanorderCommand},
//
//    {"goto",			&NativeScript::gotoCommand},
//    {"gosub",			&NativeScript::gosubCommand},
//    {"globalon",		&NativeScript::globalonCommand},
//    {"getzxc",			&NativeScript::getzxcCommand},
//    {"getvoicevol",		&NativeScript::getvoicevolCommand},
//    {"getversion",		&NativeScript::getversionCommand},
//    {"gettimer",		&NativeScript::gettimerCommand},
//    {"gettext",			&NativeScript::gettextCommand},
//    {"gettaglog",		&NativeScript::gettaglogCommand},
//    {"gettag",			&NativeScript::gettagCommand},
//    {"gettab",			&NativeScript::gettabCommand},
//    {"getspsize",		&NativeScript::getspsizeCommand},
//    {"getsppos",		&NativeScript::getspposCommand},
//    {"getspmode",		&NativeScript::getspmodeCommand},
//    {"getsevol",		&NativeScript::getsevolCommand},
//    {"getscreenshot",	&NativeScript::getscreenshotCommand},
//    {"getsavestr",		&NativeScript::getsavestrCommand},
//    {"getret",			&NativeScript::getretCommand},
//    {"getreg",			&NativeScript::getregCommand},
//    {"getparam2",		&NativeScript::getparamCommand},
//    {"getparam",		&NativeScript::getparamCommand},
//    {"getpageup",		&NativeScript::getpageupCommand},
//    {"getpage",			&NativeScript::getpageCommand},
//    {"getmp3vol",		&NativeScript::getmp3volCommand},
//    {"getmousepos",		&NativeScript::getmouseposCommand},
//    {"getmouseover",	&NativeScript::getmouseoverCommand},
//    {"getlog",			&NativeScript::getlogCommand},
//    {"getinsert",		&NativeScript::getinsertCommand},
//    {"getfunction",		&NativeScript::getfunctionCommand},
//    {"getenter",		&NativeScript::getenterCommand},
//    {"getcursorpos2",	&NativeScript::getcursorpos2Command},
//    {"getcursorpos",	&NativeScript::getcursorposCommand},
//    {"getcursor",		&NativeScript::getcursorCommand},
//    {"getcselstr",		&NativeScript::getcselstrCommand},
//    {"getcselnum",		&NativeScript::getcselnumCommand},
//    {"getbtntimer",		&NativeScript::gettimerCommand},
//    {"getbgmvol",		&NativeScript::getmp3volCommand},
//    {"game",			&NativeScript::gameCommand},
//
//    {"for",				&NativeScript::forCommand},
//    {"filelog",			&NativeScript::filelogCommand},
//    {"fileexist",		&NativeScript::fileexistCommand},
//
//    {"existspbtn",		&NativeScript::spbtnCommand},
//    {"exec_dll",		&NativeScript::exec_dllCommand},
//    {"exbtn_d",			&NativeScript::exbtnCommand},
//    {"exbtn",			&NativeScript::exbtnCommand},
//    {"erasetextwindow",	&NativeScript::erasetextwindowCommand},
//    {"english",			&NativeScript::englishCommand},
//    {"end",				&NativeScript::endCommand},
//    {"effectcut",		&NativeScript::effectcutCommand},
//    {"effectblank",		&NativeScript::effectblankCommand},
//    {"effect",			&NativeScript::effectCommand},
//
//    {"dwavestop",		&NativeScript::dwavestopCommand},
//    {"dwaveplayloop",	&NativeScript::dwaveCommand},
//    {"dwaveplay",		&NativeScript::dwaveCommand},
//    {"dwaveloop",		&NativeScript::dwaveCommand},
//    {"dwaveload",		&NativeScript::dwaveCommand},
//    {"dwave",			&NativeScript::dwaveCommand},
//    {"drawtext",		&NativeScript::drawtextCommand},
//    {"drawsp3",			&NativeScript::drawsp3Command},
//    {"drawsp2",			&NativeScript::drawsp2Command},
//    {"drawsp",			&NativeScript::drawspCommand},
//    {"drawfill",		&NativeScript::drawfillCommand},
//    {"drawclear",		&NativeScript::drawclearCommand},
//    {"drawbg2",			&NativeScript::drawbg2Command},
//    {"drawbg",			&NativeScript::drawbgCommand},
//    {"draw",			&NativeScript::drawCommand},
//    {"div",				&NativeScript::divCommand},
//    {"dim",				&NativeScript::dimCommand},
//    {"delay",			&NativeScript::delayCommand},
//    {"defvoicevol",		&NativeScript::defvoicevolCommand},
//    {"defsub",			&NativeScript::defsubCommand},
//    {"defsevol",		&NativeScript::defsevolCommand},
//    {"defmp3vol",		&NativeScript::defmp3volCommand},
//    {"definereset",		&NativeScript::defineresetCommand},
//    {"defaultspeed",	&NativeScript::defaultspeedCommand},
//    {"defaultfont",		&NativeScript::defaultfontCommand},
//    {"dec",				&NativeScript::decCommand},
//    {"date",			&NativeScript::dateCommand},
//
//    {"csp2",			&NativeScript::cspCommand},
//    {"csp",				&NativeScript::cspCommand},
//    {"cselgoto",		&NativeScript::cselgotoCommand},
//    {"cselbtn",			&NativeScript::cselbtnCommand},
//    {"csel",			&NativeScript::selectCommand},
//    {"cos",				&NativeScript::cosCommand},
//    {"cmp",				&NativeScript::cmpCommand},
//    {"clickvoice",		&NativeScript::clickvoiceCommand},
//    {"clickstr",		&NativeScript::clickstrCommand},
//    {"click",			&NativeScript::clickCommand},
//    {"cl",				&NativeScript::clCommand},
//    {"chvol",			&NativeScript::chvolCommand},
//    {"checkpage",		&NativeScript::checkpageCommand},
//    {"cellcheckspbtn",	&NativeScript::spbtnCommand},
//    {"cellcheckexbtn",	&NativeScript::exbtnCommand},
//    {"cell",			&NativeScript::cellCommand},
//    {"caption",			&NativeScript::captionCommand},
//
//    {"btrans",			&NativeScript::transbtnCommand},
//    {"btnwait2",		&NativeScript::btnwaitCommand},
//    {"btnwait",			&NativeScript::btnwaitCommand},
//    {"btntime2",		&NativeScript::btntimeCommand},
//    {"btntime",			&NativeScript::btntimeCommand},
//    {"btndown",			&NativeScript::btndownCommand},
//    {"btndef",			&NativeScript::btndefCommand},
//    {"btn",				&NativeScript::btnCommand},
//    {"btime",			&NativeScript::btntimeCommand},
//    {"bsp",				&NativeScript::bspCommand},
//    {"break",			&NativeScript::breakCommand},
//    {"br",				&NativeScript::brCommand},
//    {"blt",				&NativeScript::bltCommand},
//    {"bgmvol",			&NativeScript::mp3volCommand},
//    {"bgmstop",			&NativeScript::mp3stopCommand},
//    {"bgmonce",			&NativeScript::mp3Command}, 
//    {"bgmfadeout",		&NativeScript::mp3fadeoutCommand},
//    {"bgmfadein",		&NativeScript::mp3fadeinCommand},
//    {"bgm",				&NativeScript::mp3Command}, 
//    {"bgcpy",			&NativeScript::bgcopyCommand},
//    {"bgcopy",			&NativeScript::bgcopyCommand},
//    {"bg",				&NativeScript::bgCommand},
//    {"bexec",			&NativeScript::btnwaitCommand},
//    {"bdown",			&NativeScript::btndownCommand},
//    {"bdef",			&NativeScript::exbtnCommand},
//    {"bcursor",			&NativeScript::getcursorCommand},
//    {"bclear",			&NativeScript::btndefCommand},
//    {"barclear",		&NativeScript::barclearCommand},
//    {"bar",				&NativeScript::barCommand},
//
//    {"avi",				&NativeScript::aviCommand},
//    {"automode_time",	&NativeScript::automode_timeCommand},
//    {"automode",		&NativeScript::mode_extCommand},
//    {"autoclick",		&NativeScript::autoclickCommand},
//    {"atoi",			&NativeScript::atoiCommand},
//    {"arc",				&NativeScript::arcCommand},
//    {"amsp2",			&NativeScript::amspCommand},
//    {"amsp",			&NativeScript::amspCommand},
//    {"allspresume",		&NativeScript::allspresumeCommand},
//    {"allsphide",		&NativeScript::allsphideCommand},
//    {"allsp2resume",	&NativeScript::allsp2resumeCommand},
//    {"allsp2hide",		&NativeScript::allsp2hideCommand},
//    {"addkinsoku",		&NativeScript::addkinsokuCommand},
//    {"add",				&NativeScript::addCommand},
//    {"abssetcursor",	&NativeScript::setcursorCommand},
//
//    {"", NULL}
//};
//
//void NativeScript::makeFuncLUT()
//{
//    for (int i='z'-'a' ; i>=0 ; i--)
//        func_hash[i].func = NULL;
//
//    int idx = 0;
//    while (func_lut[idx].method){
//        int j = func_lut[idx].command[0]-'a';
//        if (func_hash[j].func == NULL) func_hash[j].func = func_lut+idx;
//        func_hash[j].num = func_lut+idx - func_hash[j].func + 1;
//        idx++;
//    }
//}
