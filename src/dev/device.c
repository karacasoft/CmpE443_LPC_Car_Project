#include "device.h"

uint8_t device_count = 0;

device_t devices[MAX_DEVICES] = {0};

uint32_t pin_usage_bitmap[6] = {
		0, 0, 0, 0, 0, 0xFFFFFFE0
};

// assigning 1 to components that we will never automatically close
uint8_t peripheral_usage[32] = {0, 1, 1, 1, 1, 0, 0, 1,
					    0, 1, 0, 0, 0, 0, 0, 1,
					    0, 0, 0, 1, 0, 0, 0, 0,
					    0, 0, 1, 0, 0, 0, 0, 0};

void add_dependency(device_info_t *device, device_info_t *dependency) {
	device_t *dev = GET_DEVICE(device);
	dev->dependency_list[dev->dependency_count++] = dependency;
}

uint8_t register_device(device_info_t *device) {
	device_t *dev = devices + device_count++;

	dev->depending_device_count = 0;
	dev->started_by_user = 0;
	dev->info = device;

	device->system_device_index = device_count;

	return 1;
}

void start_device(device_info_t *device) {
	device_t *dev = GET_DEVICE(device);
	uint8_t i;
	for(i = 0; i < dev->dependency_count; i++) {
		devices[dev->dependency_list[i]->system_device_index].depending_device_count++;
		dev->dependency_list[i]->start();
	}

	devices[device->system_device_index].started_by_user = 1;

	device->start();
}

void stop_device(device_info_t *device) {
	if(!devices[device->system_device_index].started_by_user) {
		// ERROR: device not started by user. Cannot stop
		return;
	}
	device_t *dev = GET_DEVICE(device);
	uint8_t i;
	for(i = 0; i < dev->dependency_count; i++) {
		if(!GET_DEVICE(dev->dependency_list[i])->started_by_user) {
			if(!(--GET_DEVICE(dev->dependency_list[i])->depending_device_count)) {
				dev->dependency_list[i]->end();
			}
		}
	}
	device->end();
}

void pin_error_state() {
	// get stuck in an endless loop
	while(1);
}
