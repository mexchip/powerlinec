#ifndef SEGMENTS_H
#define SEGMENTS_H

#include "segment_dir.h"
#include "segment_git.h"

char* segment_names[] = {
	"dir",
	"git"
};

#ifdef DEFINE_GLOBALS
int (*available_segments[sizeof(segment_names)])(SEGMENT*) = {
	segment_dir,
	segment_git
};
#endif

#endif