/***************************************************************************
                                graphictext.h
                             -------------------
    begin                : Mon Nov 24 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GRAPHICTEXT_H
#define GRAPHICTEXT_H

#include "painting.h"


/**
  *@author Michael Margraf
  */

class GraphicText : public Painting  {
public:
  GraphicText();
  ~GraphicText();

  void paintScheme(QPainter*);
  void getCenter(int&, int&);
  void setCenter(int, int, bool relative=false);

  GraphicText* newOne();
  bool load(const QString&);
  QString save();
  void paint(ViewPainter*);
  void MouseMoving(QPainter*, int, int, int, int, QPainter*, int, int, bool);
  bool MousePressing();
  bool getSelected(int, int);
  void Bounding(int&, int&, int&, int&);

  void rotate();
  void mirrorX();
  void mirrorY();
  bool Dialog();

  QColor   Color;
  QFont    Font;
  QString  Text;
  int      Angle;
};

#endif
