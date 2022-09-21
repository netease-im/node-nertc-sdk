#pragma once

#include <Windows.h>

#include "rw_lock_wrapper.h"

class RWLockWin : public RWLockWrapper {
 public:
  static RWLockWin* Create();

  void AcquireLockExclusive() override;
  void ReleaseLockExclusive() override;

  void AcquireLockShared() override;
  void ReleaseLockShared() override;

 private:
  RWLockWin();

  SRWLOCK lock_;
};
