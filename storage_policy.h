/**
 * @file storage_policy.h
 * @brief Various implementations of the StoragePolicy for the Observer pattern.
 *
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
#ifndef DOMUS_STORAGE_POLICY_H
#define DOMUS_STORAGE_POLICY_H

#include <functional>
#include <utility>
#include <map>
#include <vector>

namespace domus {

  /**
   * Store observers using standard containers (std::map and std::vector).
   */
  template <
    typename Event,
    typename Observer
  >
  class StdStoragePolicy {
    public:
      StdStoragePolicy() : store_() {}

      StdStoragePolicy(StdStoragePolicy const&) = default;
      StdStoragePolicy(StdStoragePolicy&&) = default;

      StdStoragePolicy& operator=(StdStoragePolicy const&) = default;
      StdStoragePolicy& operator=(StdStoragePolicy&&) = default;

      virtual ~StdStoragePolicy() { }

      void Insert(Event&& event, Observer&& value) {
        store_[std::move(event)].push_back(std::forward<Observer>(value));
      }

      void Insert(Event const& event, Observer&& value) {
        store_[event].push_back(std::forward<Observer>(value));
      }

      void Erase(Event const& event, Observer const& value) {
        auto v = store_[event];
        auto pos = v.find(std::begin(v), std::end(v), value);
        if (pos != std::end(v)) {
          store_.erase(pos);
        }
      }

      std::vector<Observer>& at(Event const& event) {
        return store_.at(event);
      }

    private:
      std::map<Event, std::vector<Observer> > store_;
  };


} // namespace domus

#endif // DOMUS_STORAGE_POLICY_H

