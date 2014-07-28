/**
 * @file threading.h
 * @brief Threadig models
 */

/*
 * Copyright (c) 2014 George Sudarkoff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once
#ifndef DOMUS_THREADING_H
#define DOMUS_THREADING_H

#include <mutex>

namespace domus {

  class SingleThreaded {
    public:
      struct Lock {
        Lock() {}
        explicit Lock(SingleThreaded*) {}
        explicit Lock(SingleThreaded&) {}
      };
  };

  class ObjectLevelLockable {
    private:
      mutable std::mutex mts_;

    public:
      ObjectLevelLockable() : mts_() {}
      ObjectLevelLockable(ObjectLevelLockable const&) : mts_() {}
      ~ObjectLevelLockable() {}

      class Lock;
      friend class Lock;

      class Lock {
        public:
          Lock() = delete;
          Lock(Lock const&) = delete;
          Lock& operator=(const Lock&) = delete;

          explicit Lock(ObjectLevelLockable const& host) : host_(host)
          {
            host_.mts_.lock();
          }

          explicit Lock(ObjectLevelLockable const* host) : host_(*host)
          {
            host_.mts_.lock();
          }

          ~Lock()
          {
            host_.mts_.unlock();
          }

        private:
          const ObjectLevelLockable& host_;
      };
  };

} // namespace domus

#endif // DOMUS_THREADING_H

