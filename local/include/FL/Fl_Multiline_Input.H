//
// "$Id: Fl_Multiline_Input.H,v 1.4.2.2 2000/06/05 21:20:27 mike Exp $"
//
// Multiline input header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2000 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef Fl_Multiline_Input_H
#define Fl_Multiline_Input_H

#include "Fl_Input.H"

class Fl_Multiline_Input : public Fl_Input {
public:
    Fl_Multiline_Input(int x,int y,int w,int h,const char *l = 0)
	: Fl_Input(x,y,w,h,l) {type(FL_MULTILINE_INPUT);}
};

#endif

//
// End of "$Id: Fl_Multiline_Input.H,v 1.4.2.2 2000/06/05 21:20:27 mike Exp $".
//
