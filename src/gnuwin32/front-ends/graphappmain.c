/*CXXR $Id$
 *CXXR
 *CXXR This file is part of CXXR, a project to refactor the R interpreter
 *CXXR into C++.  It may consist in whole or in part of program code and
 *CXXR documentation taken from the R project itself, incorporated into
 *CXXR CXXR (and possibly MODIFIED) under the terms of the GNU General Public
 *CXXR Licence.
 *CXXR 
 *CXXR CXXR is Copyright (C) 2008-10 Andrew R. Runnalls, subject to such other
 *CXXR copyrights and copyright restrictions as may be stated below.
 *CXXR 
 *CXXR CXXR is not part of the R project, and bugs and other issues should
 *CXXR not be reported via r-bugs or other R project channels; instead refer
 *CXXR to the CXXR website.
 *CXXR */


#include <windows.h>

/* The mingw-runtime startup code has _argc and _argv as visible
   symbols, as do the MS compilers.

   The mingw-w64-crt is different, but this file is not used on
   MinGW-w64
*/

extern void 
GA_startgraphapp(HINSTANCE Instance, HINSTANCE PrevInstance, int CmdShow);

#ifdef WIN64
int PASCAL
WinMain (HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine,
	 int CmdShow)
{
    extern int __argc;
    extern char **__argv;
    extern void AppMain(int argc, char **argv);

    GA_startgraphapp(Instance, PrevInstance, CmdShow);
    AppMain(__argc, __argv);

    return 0;
}
#else
int PASCAL
WinMain (HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine,
	 int CmdShow)
{
    extern int _argc;
    extern char **_argv;
    extern void AppMain(int argc, char **argv);

    GA_startgraphapp(Instance, PrevInstance, CmdShow);
    AppMain(_argc, _argv);

    return 0;
}
#endif
