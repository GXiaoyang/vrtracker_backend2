#include "vr_driver_manager_cursor.h"
#include "log.h"
#include "vr_cursor_common.h"
#include "openvr_string.h"
#include "gsl.h"

VRDriverManagerCursor::VRDriverManagerCursor(CursorContext *context)
	:
	m_context(context),
	state_ref(m_context->get_state()->driver_manager_node),
	iter_ref(m_context->get_iterators()->driver_manager_node)
{
}

void VRDriverManagerCursor::SynchronizeChildVectors()
{
	iter_ref.drivers.resize(state_ref.drivers.size());
}

uint32_t VRDriverManagerCursor::GetDriverCount() const
{
	LOG_ENTRY("CursorGetDriverCount");

	const_cast<VRDriverManagerCursor*>(this)->SynchronizeChildVectors();
	uint32_t rc = (uint32_t)iter_ref.drivers.size();

	LOG_EXIT_RC(rc, "CursorGetDriverCount");
}

uint32_t VRDriverManagerCursor::GetDriverName(vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize)
{
	LOG_ENTRY("CursorGetDriverName");

	uint32_t rc = 0;
	SynchronizeChildVectors();
	if (nDriver < iter_ref.drivers.size())
	{
		CURSOR_SYNC_STATE(driver_name, drivers[nDriver].driver_name);
		util_vector_to_return_buf_rc(&driver_name->val, pchValue, unBufferSize, &rc);
	}

	LOG_EXIT_RC(rc, "CursorGetDriverName");
}
