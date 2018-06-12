#ifndef SEGMENTS_H
#define SEGMENTS_H

#include "segment_dir.h"
#include "segment_git.h"
#include "segment_virtualenv.h"

char* segment_names[] = {
	"dir",
	"git",
	"virtualenv"
};

#ifdef DEFINE_GLOBALS
int (*available_segments[sizeof(segment_names)])(SEGMENT*) = {
	segment_dir,
	segment_git,
	segment_virtualenv
};
#endif

#endif