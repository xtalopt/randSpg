/**********************************************************************
  rng.h - Provides a function to generate random doubles between a min
          and a max value

  Copyright (C) 2016 by Patrick S. Avery

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

 ***********************************************************************/

#ifndef RNG_H
#define RNG_H

#include <random>

// C++11 way of generating random numbers in a thread-safe manner...
// Creating a new distribution each time is supposedly very fast...
static inline double getRandDouble(double min, double max)
{
  // Unfortunately, MinGW does not make use of these modern random number
  // generators, so we have to resort to an old fashioned method (which may
  // not be as good...)
#ifdef __MINGW32__
  return (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) *
         (max - min) + min;
#else
  // These random number generators are probably better.
  std::mt19937 generator(std::random_device{}());
  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(generator);
#endif
}

static inline int getRandInt(int min, int max)
{
  // Unfortunately, MinGW does not make use of these modern random number
  // generators, so we have to resort to an old fashioned method (which may
  // not be as good...)
#ifdef __MINGW32__
  return rand() % (max + 1 - min) + min;
#else
  // These random number generators are probably better.
  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
#endif
}

#endif
