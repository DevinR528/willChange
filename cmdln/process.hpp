
// vim: set tabstop=4 smarttab
#pragma once

struct cmdln_flags;

bool cmdln_process(struct cmdln_flags*& flags, int argc, const char** argvs);