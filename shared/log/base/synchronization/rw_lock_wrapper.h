#pragma once

class RWLockWrapper {
 public:
  static RWLockWrapper* CreateRWLock();
  virtual ~RWLockWrapper() = default;

  virtual void AcquireLockExclusive() = 0;
  virtual void ReleaseLockExclusive() = 0;

  virtual void AcquireLockShared() = 0;
  virtual void ReleaseLockShared() = 0;
};

// RAII extensions of the RW lock. Prevents Acquire/Release missmatches and
// provides more compact locking syntax.
class ReadLockScoped {
 public:
  explicit ReadLockScoped(RWLockWrapper& rw_lock) : rw_lock_(rw_lock) {
    rw_lock_.AcquireLockShared();
  }

  ~ReadLockScoped() { rw_lock_.ReleaseLockShared(); }

 private:
  RWLockWrapper& rw_lock_;
};

class WriteLockScoped {
 public:
  explicit WriteLockScoped(RWLockWrapper& rw_lock) : rw_lock_(rw_lock) {
    rw_lock_.AcquireLockExclusive();
  }

  ~WriteLockScoped() { rw_lock_.ReleaseLockExclusive(); }

 private:
  RWLockWrapper& rw_lock_;
};
