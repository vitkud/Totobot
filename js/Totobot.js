// Totobot.js

(function (ext) {
	var device = null;

	var speeds = {
		SLOW: 127,
		FAST: 255
	};

	ext.resetAll = function () {
	};

	ext.moveForward = function (duration, speed) {
		// TODO: device.send(...)
	};
	
	ext.moveBackward = function (duration, speed) {
		// TODO: device.send(...)
	};

	ext.turnLeft = function (duration, speed) {
		// TODO: device.send(...)
	};

	ext.turnRight = function (duration, speed) {
		// TODO: device.send(...)
	};

	function processData(bytes) {
		trace(bytes);
	}

	// Extension API interactions
	var potentialDevices = [];
	ext._deviceConnected = function (dev) {
		potentialDevices.push(dev);

		if (!device) {
			tryNextDevice();
		}
	}

	function tryNextDevice() {
		// If potentialDevices is empty, device will be undefined.
		// That will get us back here next time a device is connected.
		device = potentialDevices.shift();
		if (device) {
			device.open({ stopBits: 0, bitRate: 115200, ctsFlowControl: 0 }, deviceOpened);
		}
	}

	function deviceOpened(dev) {
		if (!dev) {
			// Opening the port failed.
			tryNextDevice();
			return;
		}
		device.set_receive_handler('Totobot', function (data) {
			processData(data);
		});
	};

	ext._deviceRemoved = function (dev) {
		if (device != dev) return;
		device = null;
	};

	ext._shutdown = function () {
		if (device) device.close();
		device = null;
	};

	ext._getStatus = function () {
		if (!device) return { status: 1, msg: 'Totobot disconnected' };
		return { status: 2, msg: 'Totobot connected' };
	}

	var descriptor = {};
	ScratchExtensions.register('Totobot', descriptor, ext, { type: 'serial' });
})({});
