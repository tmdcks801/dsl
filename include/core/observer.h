#ifndef SENSOR_OBSERVER_H
#define SENSOR_OBSERVER_H

enum class Event;

class Observer {
 public:
  virtual void Notify(Event event);

  virtual ~Observer() = default;
};

#endif
