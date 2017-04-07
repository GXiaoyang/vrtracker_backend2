#include <openvr_broker.h>

#pragma once

#if 0
// implements a gimped openvr interface
class OuterInterface
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual bool IsHmdPresent() = 0;
	virtual bool IsRuntimeInstalled() = 0;
	virtual const char *RuntimePath() = 0;
	virtual void** GetAPIInterfaceHandles() = 0;
};


class VRStream
{
	void unpause();
	void pause();
	void seek_to_time();
	void seek_to_now();
	void save_to_file(const char *filename);
	OuterInterface *GetOuterInterface();
	virtual void** GetAPIInterfaceHandles() = 0;
};

class LiveStream : public VRStream
{

};

class RecordedStream : public VRStream 
{
	bool load_from_file(const char *filename);
};
#endif
