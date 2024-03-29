//
// "$Id: Fl_Radio_Round_Button.H,v 1.4.2.2 2000/06/05 21:20:28 mike Exp $"
//
// Radio round button header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Radio_Round_Button_H
#define Fl_Radio_Round_Button_H

#include "Fl_Round_Button.H"

class Fl_Radio_Round_Button : public Fl_Round_Button {
public:
    Fl_Radio_Round_Button(int x,int y,int w,int h,const char *l=0)
	: Fl_Round_Button(x,y,w,h,l) {type(FL_RADIO_BUTTON);}
};

#endif

//
// End of "$Id: Fl_Radio_Round_Button.H,v 1.4.2.2 2000/06/05 21:20:28 mike Exp $".
//
