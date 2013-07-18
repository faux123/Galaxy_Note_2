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

/*
 * Forced Fast Charge - SysFS interface :
 * --------------------------------------
 *
 * /sys/kernel/fast_charge/force_fast_charge (rw)
 *
 *   0 - disabled (default)
 *   1 - substitute AC to USB
 *   2 - use custom mA configured through sysfs interface (see below)
 *
 * /sys/kernel/fast_charge/ac_charge_level (rw)
 *
 *   rate at which to charge when on AC (1.0A to 1.5A)
 *
 * /sys/kernel/fast_charge/usb_charge_level (r/w)
 *
 *   rate at which to charge when on USB (0.475A to 1.0A)
 *
 * /sys/kernel/fast_charge/wireless_charge_level (r/w)
 *
 *   rate at which to charge when on wireless (0.475A to 1.0A)
 *
 * /sys/kernel/fast_charge/failsafe (rw)
 *
 *   0 - disabled - allow anything up to 2.1A to be used as AC / USB custom current
 *   1 - enabled  - behaviour as described above (default)
 *
 * /sys/kernel/fast_charge/version (ro)
 *
 *   display fast charge version information
 *
 */

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fastchg.h>

int force_fast_charge;

/* sysfs interface for "force_fast_charge" */
static ssize_t force_fast_charge_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	switch (force_fast_charge) {
		case FAST_CHARGE_DISABLED:		return sprintf(buf, "0 - Disabled (default)\n");
		case FAST_CHARGE_FORCE_AC:		return sprintf(buf, "1 - Use stock AC level on USB\n");
		case FAST_CHARGE_FORCE_CUSTOM_MA:	return sprintf(buf, "2 - Use custom mA on AC (%dmA) and USB (%dmA)\n",ac_charge_level,usb_charge_level);
		default:				return sprintf(buf, "something went wrong\n");
	}
}

static ssize_t force_fast_charge_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int new_force_fast_charge;

	sscanf(buf, "%du", &new_force_fast_charge);

	switch(new_force_fast_charge) {
		case FAST_CHARGE_DISABLED:
		case FAST_CHARGE_FORCE_AC:
		case FAST_CHARGE_FORCE_CUSTOM_MA:		force_fast_charge = new_force_fast_charge;
								return count;
		default:					return count;
	}
}

static struct kobj_attribute force_fast_charge_attribute =
__ATTR(force_fast_charge, 0666, force_fast_charge_show, force_fast_charge_store);

static struct attribute *force_fast_charge_attrs[] = {
&force_fast_charge_attribute.attr,
NULL,
};

static struct attribute_group force_fast_charge_attr_group = {
.attrs = force_fast_charge_attrs,
};

/* sysfs interface for "ac_charge_level" */
static ssize_t ac_charge_level_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	switch (ac_charge_level) {
		case AC_CHARGE_1000:		return sprintf(buf, "[1000]  1100  1200  1300  1400  1500\n");
		case AC_CHARGE_1100:		return sprintf(buf, "1000  [1100]  1200  1300  1400  1500\n");
		case AC_CHARGE_1200:		return sprintf(buf, "1000  1100  [1200]  1300  1400  1500\n");
		case AC_CHARGE_1300:		return sprintf(buf, "1000  1100  1200  [1300]  1400  1500\n");
		case AC_CHARGE_1400:		return sprintf(buf, "1000  1100  1200  1300  [1400]  1500\n");
		case AC_CHARGE_1500:		return sprintf(buf, "1000  1100  1200  1300  1400  [1500]\n");
		default:			return sprintf(buf, "Custom : %dmA\n",ac_charge_level);
	}
}

static ssize_t ac_charge_level_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int new_ac_charge_level;

	sscanf(buf, "%du", &new_ac_charge_level);

	if (failsafe == FAIL_SAFE_DISABLED && new_ac_charge_level <= MAX_CHARGE_LEVEL) {

		ac_charge_level = new_ac_charge_level;
		return count;

	}

	else {

		switch (new_ac_charge_level) {
			case AC_CHARGE_1000:
			case AC_CHARGE_1100:
			case AC_CHARGE_1200:
			case AC_CHARGE_1300:
			case AC_CHARGE_1400:
			case AC_CHARGE_1500:		ac_charge_level = new_ac_charge_level;
							return count;
			default:			return count;

		}

	}

}

static struct kobj_attribute ac_charge_level_attribute =
__ATTR(ac_charge_level, 0666, ac_charge_level_show, ac_charge_level_store);

static struct attribute *ac_charge_level_attrs[] = {
&ac_charge_level_attribute.attr,
NULL,
};

static struct attribute_group ac_charge_level_attr_group = {
.attrs = ac_charge_level_attrs,
};

/* sysfs interface for "usb_charge_level" */
static ssize_t usb_charge_level_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	switch (usb_charge_level) {
		case USB_CHARGE_475:		return sprintf(buf, "[475]  600  700  800  900  1000\n");
		case USB_CHARGE_600:		return sprintf(buf, "475  [600]  700  800  900  1000\n");
		case USB_CHARGE_700:		return sprintf(buf, "475  600  [700]  800  900  1000\n");
		case USB_CHARGE_800:		return sprintf(buf, "475  600  700  [800]  900  1000\n");
		case USB_CHARGE_900:		return sprintf(buf, "475  600  700  800  [900]  1000\n");
		case USB_CHARGE_1000:		return sprintf(buf, "475  600  700  800  900  [1000]\n");
		default:			return sprintf(buf, "Custom : %dmA\n",usb_charge_level);
	}
}

static ssize_t usb_charge_level_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int new_usb_charge_level;

	sscanf(buf, "%du", &new_usb_charge_level);

	if (failsafe == FAIL_SAFE_DISABLED && new_usb_charge_level <= MAX_CHARGE_LEVEL) {

		usb_charge_level = new_usb_charge_level;
		return count;

	}

	else {

		switch (new_usb_charge_level) {
			case USB_CHARGE_475:
			case USB_CHARGE_600:
			case USB_CHARGE_700:
			case USB_CHARGE_800:
			case USB_CHARGE_900:
			case USB_CHARGE_1000:		usb_charge_level = new_usb_charge_level;
							return count;
			default:			return count;
		}

	}

}

static struct kobj_attribute usb_charge_level_attribute =
__ATTR(usb_charge_level, 0666, usb_charge_level_show, usb_charge_level_store);

static struct attribute *usb_charge_level_attrs[] = {
&usb_charge_level_attribute.attr,
NULL,
};

static struct attribute_group usb_charge_level_attr_group = {
.attrs = usb_charge_level_attrs,
};

/* sysfs interface for "wireless_charge_level" */
static ssize_t wireless_charge_level_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	switch (wireless_charge_level) {
		case WIRELESS_CHARGE_475:	return sprintf(buf, "[475]  600  700  800  900  1000\n");
		case WIRELESS_CHARGE_600:	return sprintf(buf, "475  [600]  700  800  900  1000\n");
		case WIRELESS_CHARGE_700:	return sprintf(buf, "475  600  [700]  800  900  1000\n");
		case WIRELESS_CHARGE_800:	return sprintf(buf, "475  600  700  [800]  900  1000\n");
		case WIRELESS_CHARGE_900:	return sprintf(buf, "475  600  700  800  [900]  1000\n");
		case WIRELESS_CHARGE_1000:	return sprintf(buf, "475  600  700  800  900  [1000]\n");
		default:			return sprintf(buf, "Custom : %dmA\n",wireless_charge_level);
	}
}

static ssize_t wireless_charge_level_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int new_wireless_charge_level;

	sscanf(buf, "%du", &new_wireless_charge_level);

	if (failsafe == FAIL_SAFE_DISABLED && new_wireless_charge_level <= MAX_CHARGE_LEVEL) {

		wireless_charge_level = new_wireless_charge_level;
		return count;

	}

	else {

		switch (new_wireless_charge_level) {
			case WIRELESS_CHARGE_475:
			case WIRELESS_CHARGE_600:
			case WIRELESS_CHARGE_700:
			case WIRELESS_CHARGE_800:
			case WIRELESS_CHARGE_900:
			case WIRELESS_CHARGE_1000:	wireless_charge_level = new_wireless_charge_level;
							return count;
			default:			return count;
		}

	}

}

static struct kobj_attribute wireless_charge_level_attribute =
__ATTR(wireless_charge_level, 0666, wireless_charge_level_show, wireless_charge_level_store);

static struct attribute *wireless_charge_level_attrs[] = {
&wireless_charge_level_attribute.attr,
NULL,
};

static struct attribute_group wireless_charge_level_attr_group = {
.attrs = wireless_charge_level_attrs,
};

/* sysfs interface for "failsafe" */

int failsafe = FAIL_SAFE_ENABLED;

static ssize_t failsafe_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	switch (failsafe) {
		case FAIL_SAFE_DISABLED:	return sprintf(buf, "0 - Failsafe disabled - please be careful !\n");
		case FAIL_SAFE_ENABLED:		return sprintf(buf, "1 - Failsafe active (default)\n");
		default:			return sprintf(buf, "something went wrong\n");
	}
}

static ssize_t failsafe_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

	int new_failsafe;

	sscanf(buf, "%du", &new_failsafe);

	switch (new_failsafe) {
		case FAIL_SAFE_ENABLED:		usb_charge_level = USB_CHARGE_475;
						ac_charge_level = AC_CHARGE_1000;
						failsafe = new_failsafe;
						return count;
		case FAIL_SAFE_DISABLED:	failsafe = new_failsafe;
						return count;
		default:			return count;
	}

}

static struct kobj_attribute failsafe_attribute =
__ATTR(failsafe, 0666, failsafe_show, failsafe_store);

static struct attribute *failsafe_attrs[] = {
&failsafe_attribute.attr,
NULL,
};

static struct attribute_group failsafe_attr_group = {
.attrs = failsafe_attrs,
};

/* sysfs interface for "version" */
static ssize_t version_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, FAST_CHARGE_VERSION);
}

static ssize_t version_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
/* no user change allowed */
return count;
}

static struct kobj_attribute version_attribute =
__ATTR(version, 0444, version_show, version_store);

static struct attribute *version_attrs[] = {
&version_attribute.attr,
NULL,
};

static struct attribute_group version_attr_group = {
.attrs = version_attrs,
};


/* Initialize fast charge sysfs folder */
static struct kobject *force_fast_charge_kobj;

int force_fast_charge_init(void)
{
	int force_fast_charge_retval;
	int ac_charge_level_retval;
	int usb_charge_level_retval;
	int wireless_charge_level_retval;
	int failsafe_retval;
	int version_retval;

	force_fast_charge = FAST_CHARGE_DISABLED; /* Forced fast charge disabled by default */

        force_fast_charge_kobj = kobject_create_and_add("fast_charge", kernel_kobj);
        if (!force_fast_charge_kobj) {
                return -ENOMEM;
        }
        force_fast_charge_retval = sysfs_create_group(force_fast_charge_kobj, &force_fast_charge_attr_group);
        ac_charge_level_retval = sysfs_create_group(force_fast_charge_kobj, &ac_charge_level_attr_group);
        usb_charge_level_retval = sysfs_create_group(force_fast_charge_kobj, &usb_charge_level_attr_group);
        wireless_charge_level_retval = sysfs_create_group(force_fast_charge_kobj, &wireless_charge_level_attr_group);
        failsafe_retval = sysfs_create_group(force_fast_charge_kobj, &failsafe_attr_group);
        version_retval = sysfs_create_group(force_fast_charge_kobj, &version_attr_group);
        if (force_fast_charge_retval && ac_charge_level_retval && usb_charge_level_retval && wireless_charge_level_retval && failsafe_retval && version_retval)
                kobject_put(force_fast_charge_kobj);
        return (force_fast_charge_retval && ac_charge_level_retval && usb_charge_level_retval && wireless_charge_level_retval && failsafe_retval && version_retval);
}
/* end sysfs interface */

void force_fast_charge_exit(void)
{
	kobject_put(force_fast_charge_kobj);
}

module_init(force_fast_charge_init);
module_exit(force_fast_charge_exit);
