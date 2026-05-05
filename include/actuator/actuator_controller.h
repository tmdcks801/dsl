#ifndef ACTUATOR_CONTROLLER_H
#define ACTUATOR_CONTROLLER_H

class ActuatorController {
 public:
  virtual ~ActuatorController() = default;

  void SetOperation() noexcept;

protected:
  [[nodiscard]] virtual bool CheckStatus()
};

#endif
