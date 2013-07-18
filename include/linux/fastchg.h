/*
 * Author: Chad Froebel <chadfroebel@gmail.com>
 *
 * Ported to SGS3 and extended : Jean-Pierre Rasquin <yank555.lu@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


#ifndef _LINUX_FASTCHG_H
#define _LINUX_FASTCHG_H

#define FAST_CHARGE_VERSION "Forced Fast Charge by Yank555.lu v1.4\n"

extern int force_fast_charge;

#define FAST_CHARGE_DISABLED 0	/* default */
#define FAST_CHARGE_FORCE_AC 1
#define FAST_CHARGE_FORCE_CUSTOM_MA 2

extern int ac_charge_level;

#define AC_CHARGE_1000 1000	/* default */
#define AC_CHARGE_1100 1100
#define AC_CHARGE_1200 1200
#define AC_CHARGE_1300 1300
#define AC_CHARGE_1400 1400
#define AC_CHARGE_1500 1500

extern int usb_charge_level;

#define USB_CHARGE_475 475	/* default */
#define USB_CHARGE_600 600
#define USB_CHARGE_700 700
#define USB_CHARGE_800 800
#define USB_CHARGE_900 900
#define USB_CHARGE_1000 1000

extern int wireless_charge_level;

#define WIRELESS_CHARGE_475 475	/* default */
#define WIRELESS_CHARGE_600 600
#define WIRELESS_CHARGE_700 700
#define WIRELESS_CHARGE_800 800
#define WIRELESS_CHARGE_900 900
#define WIRELESS_CHARGE_1000 1000

#define MAX_CHARGE_LEVEL 2100	/* Even with failsafe disabled, more is not allowed */

extern int failsafe;
#define FAIL_SAFE_ENABLED 1	/* default */
#define FAIL_SAFE_DISABLED 0

#endif
