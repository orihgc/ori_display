/* Generated by wayland-scanner 1.21.0 */

#ifndef OBRIC_LAUNCH_SETTING_CLIENT_PROTOCOL_H
#define OBRIC_LAUNCH_SETTING_CLIENT_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @page page_obric_launch_setting The obric_launch_setting protocol
 * @section page_ifaces_obric_launch_setting Interfaces
 * - @subpage page_iface_obric_launch_setting - 
 * @section page_copyright_obric_launch_setting Copyright
 * <pre>
 * copy right reserved
 * </pre>
 */
struct obric_launch_setting;

#ifndef OBRIC_LAUNCH_SETTING_INTERFACE
#define OBRIC_LAUNCH_SETTING_INTERFACE
/**
 * @page page_iface_obric_launch_setting obric_launch_setting
 * @section page_iface_obric_launch_setting_api API
 * See @ref iface_obric_launch_setting.
 */
/**
 * @defgroup iface_obric_launch_setting The obric_launch_setting interface
 */
extern const struct wl_interface obric_launch_setting_interface;
#endif

#ifndef OBRIC_LAUNCH_SETTING_LAUNCH_SRC_TYPE_ENUM
#define OBRIC_LAUNCH_SETTING_LAUNCH_SRC_TYPE_ENUM
enum obric_launch_setting_launch_src_type {
	OBRIC_LAUNCH_SETTING_LAUNCH_SRC_TYPE_DOCK = 1,
	OBRIC_LAUNCH_SETTING_LAUNCH_SRC_TYPE_MENUBAR = 2,
};
#endif /* OBRIC_LAUNCH_SETTING_LAUNCH_SRC_TYPE_ENUM */

#define OBRIC_LAUNCH_SETTING_TRY_LAUNCH_FROM 0


/**
 * @ingroup iface_obric_launch_setting
 */
#define OBRIC_LAUNCH_SETTING_TRY_LAUNCH_FROM_SINCE_VERSION 1

/** @ingroup iface_obric_launch_setting */
static inline void
obric_launch_setting_set_user_data(struct obric_launch_setting *obric_launch_setting, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) obric_launch_setting, user_data);
}

/** @ingroup iface_obric_launch_setting */
static inline void *
obric_launch_setting_get_user_data(struct obric_launch_setting *obric_launch_setting)
{
	return wl_proxy_get_user_data((struct wl_proxy *) obric_launch_setting);
}

static inline uint32_t
obric_launch_setting_get_version(struct obric_launch_setting *obric_launch_setting)
{
	return wl_proxy_get_version((struct wl_proxy *) obric_launch_setting);
}

/** @ingroup iface_obric_launch_setting */
static inline void
obric_launch_setting_destroy(struct obric_launch_setting *obric_launch_setting)
{
	wl_proxy_destroy((struct wl_proxy *) obric_launch_setting);
}

/**
 * @ingroup iface_obric_launch_setting
 */
static inline void
obric_launch_setting_try_launch_from(struct obric_launch_setting *obric_launch_setting, uint32_t launch_src_type, uint32_t window_id, const char *app_id)
{
	wl_proxy_marshal_flags((struct wl_proxy *) obric_launch_setting,
			 OBRIC_LAUNCH_SETTING_TRY_LAUNCH_FROM, NULL, wl_proxy_get_version((struct wl_proxy *) obric_launch_setting), 0, launch_src_type, window_id, app_id);
}

#ifdef  __cplusplus
}
#endif

#endif
