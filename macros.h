#pragma once

#define TODO(msg) \
    static_assert(false, "TODO: " msg); exit(1)
