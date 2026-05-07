#ifndef EVENT_H
#define EVENT_H

#include <cstdint>

enum class Event : uint32_t{
  kTimerExpired = 0x0000'0000,
  kFrontObstacle = 0x0000'0001,
  kLeftObstacle = 0x0000'0002,
  kRightObstacle = 0x0000'0004,
  kHighDust = 0x0000'0008,
  kSystemShutdown = 0x0000'0010,

  kCleanerFault = 0xFFFF'FF00,

  kLMotorFault = 0xFFFF'FF01,
  kRMotorFault = 0xFFFF'FF02,

  kFileOpenFault = 0xFFFF'FF04,
  kDataSizeFault = 0xFFFF'FF08,
  kDataMinusFault = 0xFFFF'FF10,
};

constexpr bool operator==(Event lhs, uint32_t rhs) noexcept {
  return static_cast<uint32_t>(lhs) == rhs;
}

// 비트 OR (이벤트 합치기)
constexpr Event operator|(Event lhs, uint32_t rhs) noexcept {
  return static_cast<Event>(static_cast<uint32_t>(lhs) |
                            rhs);
}

// 비트 AND (특정 이벤트 포함 확인)
constexpr Event operator&(Event lhs, uint32_t rhs) noexcept {
  return static_cast<Event>(static_cast<uint32_t>(lhs) &
                            rhs);
}

// NOT (특정 이벤트 끄기)
constexpr Event operator~(Event e) noexcept {
  return static_cast<Event>(~static_cast<uint32_t>(e));
}

#endif
