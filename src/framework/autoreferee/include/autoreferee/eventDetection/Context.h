//
// Created by rolf on 10-05-20.
//

#ifndef SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_CONTEXT_H_
#define SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_CONTEXT_H_
#include <field/FieldState.h>
#include <world/WorldState.h>
#include <deque>
#include <referee/GameState.h>

struct Context{
  FieldState geometry;
  std::deque<WorldState> worldHistory;
  RefereeState referee;
};
#endif //SOCCER_SRC_FRAMEWORK_AUTOREFEREE_EVENTDETECTION_CONTEXT_H_
