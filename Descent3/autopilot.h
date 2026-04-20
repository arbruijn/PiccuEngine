#ifndef AUTOPILOT_H_
#define AUTOPILOT_H_

struct game_controls;

bool AutopilotIsActive();
void AutopilotSetActive(bool active);
void AutopilotToggle();
void AutopilotReset();
void AutopilotApply(game_controls *controls);

#endif
