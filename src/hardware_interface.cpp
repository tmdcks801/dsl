#include <hardware_interface.h>

HardwareInterface::~HardwareInterface() {
  if (file_system_) {
    file_system_->close(device_id_);
  }
}
