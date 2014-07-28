/**
 * @file observer.hpp
 * @brief Implementation of the Observer design pattern.
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
#ifndef DOMUS_OBSERVER_H
#define DOMUS_OBSERVER_H

#include "storage_policy.h"

#include <functional>

namespace domus {

  template
  <
    typename Event,
    typename Message = Event,
    typename Observer = std::function<void(Message const&)>,
    typename StoragePolicy = StdStoragePolicy<Event, Observer>
  >
  class Subject {
    public:
      Subject() : observers_() {}

      Subject(Subject const&) = default;
      Subject(Subject&&) = default;

      Subject& operator=(Subject const&) = default;
      Subject& operator=(Subject&&) = default;

      virtual ~Subject() { }

      void Attach(Event const& event, Observer&& observer)
      {
        observers_.Insert(event, std::forward<Observer>(observer));
      }

      void Detach(Event const& event, Observer& observer)
      {
        observers_.Erase(event, observer);
      }

      void Notify(Event const& event)
      {
        for (auto const& observer : observers_.at(event)) {
          observer(event);
        }
      }

      void Notify(Event const& event, Message const& message)
      {
        for (auto const& observer : observers_.at(event)) {
          observer(message);
        }
      }

    private:
      StoragePolicy observers_;
  };

} // namespace domus

#endif // DOMUS_OBSERVER_H

