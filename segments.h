#ifndef SEGMENTS_H
#define SEGMENTS_H

#include "segment_dir.h"
#include "segment_git.h"

char* segment_names[] = {
	"dir",
	"git"
};

#ifdef DEFINE_GLOBALS
SEGMENT (*available_segments[sizeof(segment_names)])(void) = {
	segment_dir,
	segment_git
};
#endif

#endif