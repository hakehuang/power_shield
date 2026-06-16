/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <app/drivers/blink.h>

#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#define SAMPLE_INTERVAL_MS  1000U
#define BLINK_PERIOD_MS     500U
#define BLINK_FAST_PERIOD_MS 100U

int main(void)
{
	int ret;
	const struct device *sensor, *blink;
	struct sensor_value val;

	printk("Power Shield Sample Application %s\n", APP_VERSION_STRING);

	sensor = DEVICE_DT_GET(DT_NODELABEL(example_sensor));
	if (!device_is_ready(sensor)) {
		LOG_ERR("Sensor not ready");
		return 0;
	}

	blink = DEVICE_DT_GET(DT_NODELABEL(blink_led));
	if (!device_is_ready(blink)) {
		LOG_ERR("Blink LED not ready");
		return 0;
	}

	ret = blink_set_period_ms(blink, BLINK_PERIOD_MS);
	if (ret < 0) {
		LOG_ERR("Could not set LED blink period (%d)", ret);
		return 0;
	}

	printk("Power shield initialized, sampling sensor every %u ms\n",
	       SAMPLE_INTERVAL_MS);

	struct sensor_value last_val = {0};

	while (1) {
		ret = sensor_sample_fetch(sensor);
		if (ret < 0) {
			LOG_ERR("Could not fetch sample (%d)", ret);
			return 0;
		}

		ret = sensor_channel_get(sensor, SENSOR_CHAN_PROX, &val);
		if (ret < 0) {
			LOG_ERR("Could not get sample (%d)", ret);
			return 0;
		}

		if (val.val1 != last_val.val1) {
			if (val.val1) {
				LOG_INF("Proximity detected, increasing blink rate");
				ret = blink_set_period_ms(blink, BLINK_FAST_PERIOD_MS);
			} else {
				LOG_INF("Proximity lost, restoring normal blink rate");
				ret = blink_set_period_ms(blink, BLINK_PERIOD_MS);
			}

			if (ret < 0) {
				LOG_ERR("Could not update LED blink period (%d)", ret);
			}

			last_val = val;
		}

		/* Sleep between samples to reduce power consumption */
		k_sleep(K_MSEC(SAMPLE_INTERVAL_MS));
	}

	return 0;
}
