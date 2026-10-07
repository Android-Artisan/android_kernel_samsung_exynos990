/* SPDX-License-Identifier: GPL-2.0 */

/*
 * (C) COPYRIGHT 2021 Samsung Electronics Inc. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 */

#include <soc/samsung/cal-if.h>
#include <soc/samsung/g3d_local_dvfs.h>
#include "../../../../soc/samsung/cal-if/exynos9830/cmucal-node.h"

#include <gpexbe_devicetree.h>

#include <gpexbe_clock.h>
#include <gpex_utils.h>
#include <gpex_debug.h>

extern int cal_clk_setrate(unsigned int vclkid, unsigned long rate);
extern void cal_dfs_set_volt_margin(unsigned int id, int volt);

struct _clock_backend_info {
	int boot_clock;
	int max_clock_limit;
};

static struct _clock_backend_info pm_info;
static unsigned int cal_id;
static int g3d_local_cached_rate;

int gpexbe_clock_get_level_num(void)
{
	if (g3d_local_is_g3d_id(cal_id))
		return g3d_local_get_level_num();

	return cal_dfs_get_lv_num(cal_id);
}

int gpexbe_clock_get_rate_asv_table(struct freq_volt *fv_array, int level_num)
{
	int i;
	int ret = 0;
	struct dvfs_rate_volt rate_volt[48];

	if (g3d_local_is_g3d_id(cal_id)) {
		int local_lv_num = g3d_local_get_level_num();

		if (level_num > local_lv_num)
			level_num = local_lv_num;

		for (i = 0; i < level_num; i++) {
			fv_array[i].freq = g3d_local_get_freq(i);
			fv_array[i].volt = g3d_local_get_volt(i);
		}

		return level_num;
	}

	ret = cal_dfs_get_rate_asv_table(cal_id, rate_volt);

	if (!ret) {
		/* TODO: print error. Also remove this size limit by using dynamic alloc */
		return ret;
	}

	for (i = 0; i < level_num; i++) {
		fv_array[i].freq = rate_volt[i].rate;
		fv_array[i].volt = rate_volt[i].volt;
	}

	return ret;
}

int gpexbe_clock_get_boot_freq(void)
{
	return pm_info.boot_clock;
}

int gpexbe_clock_get_max_freq(void)
{
	if (g3d_local_is_g3d_id(cal_id))
		return G3D_LOCAL_MAX_CLOCK_KHZ;

	return pm_info.max_clock_limit;
}

static int gpexbe_clock_set_rate_direct_oc(int clk)
{
	int ret;
	unsigned long pll_hz = g3d_local_get_pll_hz(clk);
	unsigned long actual_pll_hz;
	int margin_uv = g3d_local_get_margin_uv(clk);

	if (!pll_hz)
		return -EINVAL;

	/* 975/949/936 MHz: ACPM base + voltage margin + direct PLL_G3D. */
	ret = cal_dfs_set_rate(cal_id, G3D_LOCAL_OC_BASE_CLOCK_KHZ);
	if (ret) {
		GPU_LOG(MALI_EXYNOS_ERROR,
			"G3D OC: ACPM base %d KHz failed before %d KHz (ret=%d)\n",
			G3D_LOCAL_OC_BASE_CLOCK_KHZ, clk, ret);
		return ret;
	}

	cal_dfs_set_volt_margin(cal_id, margin_uv);

	ret = cal_clk_setrate(PLL_G3D, pll_hz);
	if (ret) {
		GPU_LOG(MALI_EXYNOS_ERROR,
			"G3D OC: direct PLL_G3D set to %lu Hz for %d KHz failed (ret=%d)\n",
			pll_hz, clk, ret);
		return ret;
	}

	actual_pll_hz = cal_clk_getrate(PLL_G3D);
	if (!actual_pll_hz || (actual_pll_hz + 500) / 1000 != clk) {
		GPU_LOG(MALI_EXYNOS_ERROR,
			"G3D OC: requested PLL %lu Hz, read back %lu Hz\n",
			pll_hz, actual_pll_hz);
		return -ERANGE;
	}

	actual_pll_hz = cal_clk_getrate(PLL_G3D);
	if (!actual_pll_hz || (actual_pll_hz + 500) / 1000 != clk) {
		GPU_LOG(MALI_EXYNOS_ERROR,
			"G3D OC: requested PLL %lu Hz, read back %lu Hz\n",
			pll_hz, actual_pll_hz);
		return -ERANGE;
	}

	g3d_local_cached_rate = clk;
	GPU_LOG(MALI_EXYNOS_INFO,
		"G3D OC: forced PLL_G3D to %lu Hz, reporting %d KHz\n",
		pll_hz, clk);

	return 0;
}
int gpexbe_clock_set_rate(int clk)
{
	int ret = 0;

	gpex_debug_new_record(HIST_CLOCK);
	gpex_debug_record_prev_data(HIST_CLOCK, gpexbe_clock_get_rate());

	if (g3d_local_is_g3d_id(cal_id) && g3d_local_is_oc_freq(clk))
		ret = gpexbe_clock_set_rate_direct_oc(clk);
	else
		ret = cal_dfs_set_rate(cal_id, clk);

	if (!ret && g3d_local_is_g3d_id(cal_id))
		g3d_local_cached_rate = clk;
	else if (ret && g3d_local_is_g3d_id(cal_id))
		GPU_LOG(MALI_EXYNOS_ERROR, "CAL/G3D failed to set local clock %d KHz (ret=%d)\n",
			clk, ret);

	gpex_debug_record_time(HIST_CLOCK);
	gpex_debug_record_code(HIST_CLOCK, ret);
	gpex_debug_record_new_data(HIST_CLOCK, clk);

	if (ret)
		gpex_debug_incr_error_cnt(HIST_CLOCK);

	return ret;
}

int gpexbe_clock_get_rate(void)
{
	if (g3d_local_is_g3d_id(cal_id) && g3d_local_is_oc_freq(g3d_local_cached_rate))
		return g3d_local_cached_rate;

	return cal_dfs_get_rate(cal_id);
}

int gpexbe_clock_init(void)
{
	cal_id = gpexbe_devicetree_get_int(g3d_cmu_cal_id);

	if (!cal_id) {
		/* TODO: print error cal id not found */
		return -1;
	}

	if (g3d_local_is_g3d_id(cal_id)) {
		pm_info.boot_clock = G3D_LOCAL_BOOT_CLOCK_KHZ;
		pm_info.max_clock_limit = G3D_LOCAL_MAX_CLOCK_KHZ;
		g3d_local_cached_rate = pm_info.boot_clock;
	} else {
		pm_info.boot_clock = cal_dfs_get_boot_freq(cal_id);
		pm_info.max_clock_limit = (int)cal_dfs_get_max_freq(cal_id);
	}

	gpex_utils_get_exynos_context()->pm_info = &pm_info;

	return 0;
}

void gpexbe_clock_term(void)
{
	cal_id = 0;
	pm_info.boot_clock = 0;
	pm_info.max_clock_limit = 0;
	g3d_local_cached_rate = 0;
}
