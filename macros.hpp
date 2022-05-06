#pragma once

#include "fmt/core.h"

#define TODO(msg) \
	FMT_ASSERT(false, "TODO: " msg); \
	exit(1)

