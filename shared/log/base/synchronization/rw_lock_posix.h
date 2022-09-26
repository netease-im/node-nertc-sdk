#pragma once

#include <pthread.h>

#include "rw_lock_wrapper.h"

class RWLockPosix : public RWLockWrapper {
 public:
  static RWLockPosix* Create();
  ~RWLockPosix() override;

  void AcquireLockExclusive() override;
  void ReleaseLockExclusive() override;

  void AcquireLockShared() override;
  void ReleaseLockShared() override;

 private:
  RWLockPosix();
  bool Init();

  pthread_rwlock_t lock_;
};
