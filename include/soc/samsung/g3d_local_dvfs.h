/* SPDX-License-Identifier: GPL-2.0 */
/* Local G3D DVFS table for Exynos9830 Mali-G77: 975MHz, 949MHz, 936MHz. */
#ifndef __SOC_SAMSUNG_G3D_LOCAL_DVFS_H__
#define __SOC_SAMSUNG_G3D_LOCAL_DVFS_H__

#include <linux/types.h>

#define G3D_LOCAL_ACPM_IDX              10
#define G3D_LOCAL_MAX_CLOCK_KHZ         975000
#define G3D_LOCAL_MIN_CLOCK_KHZ         156000
#define G3D_LOCAL_BOOT_CLOCK_KHZ        260000
#define G3D_LOCAL_OC_BASE_CLOCK_KHZ     897000
#define G3D_LOCAL_897_VOLT_UV            737500

#define G3D_LOCAL_975_CLOCK_KHZ         975000
#define G3D_LOCAL_975_PLL_HZ            975000000UL
#define G3D_LOCAL_975_VOLT_UV           793750
#define G3D_LOCAL_975_VOLT_MARGIN_UV    (G3D_LOCAL_975_VOLT_UV - G3D_LOCAL_897_VOLT_UV)

#define G3D_LOCAL_949_CLOCK_KHZ         949000
#define G3D_LOCAL_949_PLL_HZ            949000000UL
#define G3D_LOCAL_949_VOLT_UV           781250
#define G3D_LOCAL_949_VOLT_MARGIN_UV    (G3D_LOCAL_949_VOLT_UV - G3D_LOCAL_897_VOLT_UV)

#define G3D_LOCAL_936_CLOCK_KHZ         936000
#define G3D_LOCAL_936_PLL_HZ            936000000UL
#define G3D_LOCAL_936_VOLT_UV           762500
#define G3D_LOCAL_936_VOLT_MARGIN_UV    (G3D_LOCAL_936_VOLT_UV - G3D_LOCAL_897_VOLT_UV)

static const unsigned int g3d_local_freq_table[] = {
	975000,
	949000,
	936000,
	897000,
	832000,
	800000,
	702000,
	572000,
	455000,
	377000,
	260000,
	156000,
};

static const unsigned int g3d_local_volt_table[] = {
	G3D_LOCAL_975_VOLT_UV,
	G3D_LOCAL_949_VOLT_UV,
	G3D_LOCAL_936_VOLT_UV,
	G3D_LOCAL_897_VOLT_UV,
	900000,
	887500,
	850000,
	812500,
	787500,
	762500,
	737500,
	700000,
};

static inline bool g3d_local_is_g3d_id(unsigned int cal_id)
{
	return (cal_id & 0xFFFF) == G3D_LOCAL_ACPM_IDX;
}

static inline bool g3d_local_is_oc_freq(unsigned long khz)
{
	return khz == G3D_LOCAL_975_CLOCK_KHZ ||
	       khz == G3D_LOCAL_949_CLOCK_KHZ ||
	       khz == G3D_LOCAL_936_CLOCK_KHZ;
}

static inline unsigned long g3d_local_get_pll_hz(unsigned long khz)
{
	switch (khz) {
	case G3D_LOCAL_975_CLOCK_KHZ:
		return G3D_LOCAL_975_PLL_HZ;
	case G3D_LOCAL_949_CLOCK_KHZ:
		return G3D_LOCAL_949_PLL_HZ;
	case G3D_LOCAL_936_CLOCK_KHZ:
		return G3D_LOCAL_936_PLL_HZ;
	default:
		return 0;
	}
}

static inline int g3d_local_get_margin_uv(unsigned long khz)
{
	switch (khz) {
	case G3D_LOCAL_975_CLOCK_KHZ:
		return G3D_LOCAL_975_VOLT_MARGIN_UV;
	case G3D_LOCAL_949_CLOCK_KHZ:
		return G3D_LOCAL_949_VOLT_MARGIN_UV;
	case G3D_LOCAL_936_CLOCK_KHZ:
		return G3D_LOCAL_936_VOLT_MARGIN_UV;
	default:
		return 0;
	}
}

static inline int g3d_local_get_level_num(void)
{
	return sizeof(g3d_local_freq_table) / sizeof(g3d_local_freq_table[0]);
}

static inline unsigned int g3d_local_get_freq(int idx)
{
	return g3d_local_freq_table[idx];
}

static inline unsigned int g3d_local_get_volt(int idx)
{
	return g3d_local_volt_table[idx];
}

#endif /* __SOC_SAMSUNG_G3D_LOCAL_DVFS_H__ */
