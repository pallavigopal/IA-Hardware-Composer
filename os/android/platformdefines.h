/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#ifndef OS_ANDROID_PLATFORMDEFINES_H_
#define OS_ANDROID_PLATFORMDEFINES_H_

#ifndef LOG_TAG
#define LOG_TAG "iahwcomposer"
#endif

#ifndef ATRACE_TAG
#define ATRACE_TAG ATRACE_TAG_GRAPHICS
#endif

#include <utils/Trace.h>
#include <cutils/log.h>
#include <hardware/hardware.h>
#include <hardware/hwcomposer.h>
#include <ui/GraphicBuffer.h>
#include "platformcommondefines.h"
#include <cros_gralloc_handle.h>
#define DRV_I915 1
#include <i915_private_android_types.h>

#ifdef _cplusplus
extern "C" {
#endif

struct gralloc_handle {
  buffer_handle_t handle_ = NULL;
  native_handle_t* imported_handle_ = NULL;
  uint32_t gem_handle_ = 0;
  HwcBuffer meta_data_;
  uint64_t gralloc1_buffer_descriptor_t_ = 0;
  bool hwc_buffer_ = false;
};

typedef struct gralloc_handle* HWCNativeHandle;
typedef struct cros_gralloc_handle HWCNativeBuffer;
#define GETNATIVEBUFFER(handle) (*(cros_gralloc_handle*)(handle->handle_))


struct BufferHash {
  size_t operator()(HWCNativeBuffer const& buffer) const {
    const native_handle_t & p = buffer.base;
    std::size_t seed = 0;
    for (int i = 0; i < p.numFds + p.numInts; i++) {
      hash_combine_hwc(seed, (const size_t)p.data[i]);
    }
    return seed;
  }
};

struct BufferEqual {
  bool operator()(const HWCNativeBuffer& buffer1, const HWCNativeBuffer& buffer2) const {
    const native_handle_t &p1 = buffer1.base;
    const native_handle_t &p2 = buffer2.base;
    bool equal = (p1.numFds == p2.numFds) && (p1.numInts ==  p2.numInts);
    if (equal) {
      for (int i = 0; i < p1.numFds + p1.numInts; i++) {
        equal = equal && (p1.data[i] == p2.data[i]);
        if (!equal)
          break;
      }
    }
    return equal;
  }
};


#define VTRACE(fmt, ...) ALOGV("%s: " fmt, __func__, ##__VA_ARGS__)
#define DTRACE(fmt, ...) ALOGD("%s: " fmt, __func__, ##__VA_ARGS__)
#define ITRACE(fmt, ...) ALOGI(fmt, ##__VA_ARGS__)
#define WTRACE(fmt, ...) ALOGW("%s: " fmt, __func__, ##__VA_ARGS__)
#define ETRACE(fmt, ...) ALOGE("%s: " fmt, __func__, ##__VA_ARGS__)
#define STRACE() ATRACE_CALL()
// _cplusplus
#ifdef _cplusplus
}
#endif

#endif  // OS_ANDROID_PLATFORMDEFINES_H_
