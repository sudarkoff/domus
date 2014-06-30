/**
 * @file domus.cpp
 * @brief Domus main
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

#include "observer.h"
#include "calendar.h"

#include <iostream>

void BirthdayReminder(std::string const& msg)
{
  std::cout << msg << " is my birthday!" << std::endl;
}

int main()
{
  domus::CalendarDevice cal;
  cal.Attach("March 31", BirthdayReminder);
  cal.Notify("March 31");

  domus::Subject<std::string> bar;
  bar.Attach("RED", [](std::string const& msg){ std::cout << msg << " is my favorite color!" << std::endl; });
  bar.Notify("RED");

  return 0;
}

