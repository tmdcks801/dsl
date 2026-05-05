#ifndef EVENT_H
#define EVENT_H

enum class Event {
  kFrontObstacle,
  kLeftObstacle,
  kRightObstacle,
  kHighDust,
  kTimerExpired,
  kHWFault,
  kSystemShutdown
};

#endif
