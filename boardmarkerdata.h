/****************************************************************************
 *                                                                          *
 * QtVR--Physics-base inverse kinematics and inverse dynamics               *
 * Copyright (c) 2013 Joseph Cooper                                         *
 *                                                                          *
 * This software is provided 'as-is', without any express or implied        *
 * warranty. In no event will the authors be held liable for any damages    *
 * arising from the use of this software.                                   *
 *                                                                          *
 * Permission is granted to anyone to use this software for any purpose,    *
 * including commercial applications, and to alter it and redistribute it   *
 * freely, subject to the following restrictions:                           *
 *                                                                          *
 *  1. The origin of this software must not be misrepresented; you must not *
 *  claim that you wrote the original software. If you use this software    *
 *  in a product, an acknowledgment in the product documentation would be   *
 *  appreciated but is not required.                                        *
 *                                                                          *
 *  2. Altered source versions must be plainly marked as such, and must not *
 *  be misrepresented as being the original software.                       *
 *                                                                          *
 *  3. This notice may not be removed or altered from any source            *
 *  distribution.                                                           *
 ****************************************************************************/
#ifndef BOARDMARKERDATA_H
#define BOARDMARKERDATA_H

#include <QObject>
#include <ode/ode.h>
#include <vector>
using std::vector;

class CapBody;

#define MARKERS 50
#define BOARDS 2
#define BPT 5
struct BoardMarkerFrame {
  double time;
  double markers[MARKERS][4]; //50 markers: x,y,z,c
  double boards[BOARDS][BPT][4]; //2 boards, 4 corners, x,y,z,v
  BoardMarkerFrame()
  {
    time=0;
    memset(markers,0,sizeof(double)*MARKERS*4);
    memset(boards,0,sizeof(double)*BOARDS*BPT*4);
  }
};

/**
  This class can loads data captured with
  the Phasespace and the balance board.
  You can advance through
  the data at a given rate or keep things
  constant or jump to a given frame.
  */
class BoardMarkerData : public QObject
{
    Q_OBJECT
public:
    explicit BoardMarkerData(dWorldID world,dSpaceID space,QObject *parent = 0);

  /// We set the markers to be where the data frame
  /// says they should be.  We set their velocity to
  /// match where they will be with the next step.
  void step();

  // Need to check on dimensions

  void setStepSize(int stepSize) {framesPerStep=stepSize;}
  /// This allows us to set the body's velocities correctly.
  void setStepTime(double stepTime) {timePerStep = stepTime;}

  /// For rendering purposes.
  dSpaceID getSpace() { return space; }

  int size();

signals:
  void frameChanged(int);

public slots:

  void setPaused(bool);
  void setSingleStep();
  void setFrame(int frame);

  void changeBodyConnect(int mark,int body);
  void changeBodyLink(int mark,bool link);
  void changeLinkPos(int mark,double xx,double yy,double zz);
protected:
  void loadData();

public:
  int framesPerStep;
  double timePerStep;
  int currentFrame;
  /// If the object is paused, the markers have constant velocity.
  bool paused;
  bool singleStep;
  dSpaceID space;
  dWorldID world;

  dGeomID* geom;
  dBodyID* body;
  dJointID* joint;
  dJointGroupID jointGroup;
  bool* tryLink;

  BoardMarkerFrame* cFrame;
  BoardMarkerFrame* nFrame;

  int markCnt;
  CapBody* cBody;


  // Data Structure
  vector<BoardMarkerFrame> data;


};

#endif // BOARDMARKERDATA_H
