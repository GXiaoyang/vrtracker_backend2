#pragma once
#include <openvr.h>
#include <math.h>

inline bool softcompare_is_similar(float a, float b, float epsilon)
{
	float delta;
	if (a > b)
		delta = a - b;
	else
		delta = b - a;

	if (delta > -epsilon && delta < epsilon)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool softcompare_is_similar(
	const vr::HmdColor_t &a,
	const vr::HmdColor_t &b,

	float epsilon = 0.0001f)
{
	bool similar = true;
	if (fabs(a.r - b.r) > epsilon ||
		fabs(a.g - b.g) > epsilon ||
		fabs(a.b - b.b) > epsilon ||
		fabs(a.a - b.a) > epsilon)
	{
		similar = false;
	}
	return similar;
}

inline bool softcompare_is_similar(
	const vr::DistortionCoordinates_t *a,
	const vr::DistortionCoordinates_t *b,
	float epsilon)
{
	bool similar = true;
	if (
		!softcompare_is_similar(a->rfRed[0], b->rfRed[0], epsilon) ||
		!softcompare_is_similar(a->rfRed[1], b->rfRed[1], epsilon) ||
		!softcompare_is_similar(a->rfGreen[0], b->rfGreen[0], epsilon) ||
		!softcompare_is_similar(a->rfGreen[1], b->rfGreen[1], epsilon) ||
		!softcompare_is_similar(a->rfBlue[0], b->rfBlue[0], epsilon) ||
		!softcompare_is_similar(a->rfBlue[1], b->rfBlue[1], epsilon))
	{
		similar = false;
	}
	return similar;
}



inline bool softcompare_is_similar(
	const vr::HmdColor_t *a,
	const vr::HmdColor_t *b,
	int count,
	float epsilon = 0.0001f)
{
	bool similar = true;
	for (int i = 0; i < count; i++)
	{
		if (!softcompare_is_similar(a[i], b[i], epsilon))
		{ 
			similar = false;
			break;
		}
	}
	return similar;
}

inline bool softcompare_is_similar(
	vr::HmdMatrix34_t &a,
	vr::HmdMatrix34_t &b,
	float epsilon = 0.0001f)
{
	bool similar = true;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (fabs(a.m[i][j] - b.m[i][j]) > epsilon)
			{
				similar = false;
				goto done;
			}
		}
	}
done:
	return similar;
}



// returns true if similar
inline bool softcompare_is_similar(
	const vr::TrackedDevicePose_t &a,
	const vr::TrackedDevicePose_t &b,
	float epsilon)
{
	bool similar = true;
	if (a.bPoseIsValid != b.bPoseIsValid)
	{
		similar = false;
	}
	if (a.bDeviceIsConnected != b.bDeviceIsConnected)
	{
		similar = false;
	}
	if (a.eTrackingResult != b.eTrackingResult)
	{
		similar = false;
	}

	if (a.bPoseIsValid != b.bPoseIsValid)
	{
		for (int i = 0; i < 3 && similar; i++)
		{
			similar = softcompare_is_similar(a.vAngularVelocity.v[i], b.vAngularVelocity.v[i], epsilon);
			similar = softcompare_is_similar(a.vVelocity.v[i], b.vVelocity.v[i], epsilon);
			for (int j = 0; j < 4 && similar; j++)
			{
				similar = softcompare_is_similar(a.mDeviceToAbsoluteTracking.m[i][j],
					b.mDeviceToAbsoluteTracking.m[i][j], epsilon);
			}
		}
	}
	return similar;
}

inline bool softcompare_is_similar(const vr::Compositor_FrameTiming &a, const vr::Compositor_FrameTiming &b)
{
	bool similar = true;

	if (a.m_nSize != b.m_nSize)
	{
		similar = false;
	}
	else if (abs((int)a.m_nFrameIndex - (int)b.m_nFrameIndex) > 100)
	{
		similar = false;
	}
	else if (a.m_nReprojectionFlags != b.m_nReprojectionFlags)
	{
		similar = false;
	}
	else if (!softcompare_is_similar(a.m_HmdPose, b.m_HmdPose, 0.0001f))
	{
		similar = false;
	}
	return similar;
}

inline bool softcompare_is_similar(
	const vr::TrackedDevicePose_t *a_poses,
	const vr::TrackedDevicePose_t *b_poses,
	int num_poses,
	float epsilon = 0.0001f)
{
	bool similar = true;
	for (int i = 0; i < num_poses; i++)
	{
		if (!softcompare_is_similar(a_poses[i], b_poses[i], epsilon))
		{
			similar = false;
			break;
		}
	}
	return similar;
}

inline bool softcompare_is_similar(
	const vr::Compositor_CumulativeStats &a,
	const vr::Compositor_CumulativeStats &b)
{
	bool similar = true;

	similar = similar && (a.m_nPid == b.m_nPid);
	similar = similar && (abs((int)a.m_nNumFramePresents			- (int)b.m_nNumFramePresents ) < 5);
	similar = similar && (abs((int)a.m_nNumDroppedFrames			- (int)b.m_nNumDroppedFrames ) < 5);
	similar = similar && (abs((int)a.m_nNumReprojectedFrames		- (int)b.m_nNumReprojectedFrames ) < 5);
	similar = similar && (abs((int)a.m_nNumFramePresentsOnStartup	- (int)b.m_nNumFramePresentsOnStartup) < 5);
	similar = similar && (abs((int)a.m_nNumDroppedFramesOnStartup	- (int)b.m_nNumDroppedFramesOnStartup) < 5);
	similar = similar && (abs((int)a.m_nNumReprojectedFramesOnStartup-(int)b.m_nNumReprojectedFramesOnStartup) < 5);
	similar = similar && (abs((int)a.m_nNumLoading					- (int)b.m_nNumLoading) < 5);
	similar = similar && (abs((int)a.m_nNumFramePresentsLoading		- (int)b.m_nNumFramePresentsLoading) < 5);
	similar = similar && (abs((int)a.m_nNumDroppedFramesLoading		- (int)b.m_nNumDroppedFramesLoading) < 5);
	similar = similar && (abs((int)a.m_nNumReprojectedFramesLoading	- (int)b.m_nNumReprojectedFramesLoading) < 5);
	similar = similar && (abs((int)a.m_nNumTimedOut					- (int)b.m_nNumTimedOut) < 5);
	similar = similar && (abs((int)a.m_nNumFramePresentsTimedOut	- (int)b.m_nNumFramePresentsTimedOut) < 5);
	similar = similar && (abs((int)a.m_nNumDroppedFramesTimedOut	- (int)b.m_nNumDroppedFramesTimedOut) < 5);
	similar = similar && (abs((int)a.m_nNumReprojectedFramesTimedOut -(int)b.m_nNumReprojectedFramesTimedOut) < 5);

	return similar;
}


// heuristic
// return 0 for identical or a bigger number
inline int softcompare_controllerstates(const vr::VRControllerState_t *a, const vr::VRControllerState_t *b)
{
	int score = 0;
	if (a->unPacketNum != b->unPacketNum)
	{
		score++; // 1 point. basically a tie breaker if the buttons match
	}

	if (a->ulButtonPressed != b->ulButtonPressed)
	{
		score += 3;
	}

	if (a->ulButtonTouched != b->ulButtonTouched)
	{
		score += 3;
	}

	// max score of 10
	for (int axis = 0; axis < vr::k_unControllerStateAxisCount; axis++)
	{
		if (fabs(a->rAxis[axis].y - b->rAxis[axis].y) > 0.001f)
		{
			score++;
		}
		if (fabs(a->rAxis[axis].x - b->rAxis[axis].x) > 0.001f)
		{
			score++;
		}
	}
	return score;
}
