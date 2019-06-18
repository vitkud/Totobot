// Totobot.js

(function (ext) {
	var device = null;

	var speeds = {
		SLOW: 127,
		FAST: 255
	};

	var motorPorts = {
		LEFT: 0,
		RIGHT: 1
	}

	ext.resetAll = function () {
	};

	ext.setCorrection = function (corr) {
		// TODO: device.send(...)
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

	ext.runMotor = function (port, speed) {
		if (typeof port == "string") {
			port = motorPorts[port];
		}
		runPackage(90, port + 1, short2array(speed));
    };

	ext.setEyeEffect = function (eye, effect) {
		// TODO: device.send(...)
	};

	function sendPackage(argList, type) {
		var bytes = [0xff, 0x55, 0, 0, type];
		for (var i = 0; i < argList.length; ++i) {
			var val = argList[i];
			if (val.constructor == "[class Array]") {
				bytes = bytes.concat(val);
			} else {
				bytes.push(val);
			}
		}
		bytes[2] = bytes.length - 3;
		device.send(bytes);
	}
	
	function runPackage() {
		sendPackage(arguments, 2);
	}
	function getPackage() {
		var nextID = arguments[0];
		Array.prototype.shift.call(arguments);
		sendPackage(arguments, 1);
	}

	var inputArray = [];
	var _isParseStart = false;
	var _isParseStartIndex = 0;
	function processData(bytes) {
		var len = bytes.length;
		if (_rxBuf.length > 30) {
			_rxBuf = [];
		}
		for (var index = 0; index < bytes.length; index++) {
			var c = bytes[index];
			_rxBuf.push(c);
			if (_rxBuf.length >= 2) {
				if (_rxBuf[_rxBuf.length - 1] == 0x55 && _rxBuf[_rxBuf.length - 2] == 0xff) {
					_isParseStart = true;
					_isParseStartIndex = _rxBuf.length - 2;
				}
				if (_rxBuf[_rxBuf.length - 1] == 0xa && _rxBuf[_rxBuf.length - 2] == 0xd && _isParseStart) {
					_isParseStart = false;

					var position = _isParseStartIndex + 2;
					var extId = _rxBuf[position];
					position++;
					var type = _rxBuf[position];
					position++;
					//1 byte 2 float 3 short 4 len+string 5 double
					var value;
					switch (type) {
						case 1: {
							value = _rxBuf[position];
							position++;
						}
							break;
						case 2: {
							value = readFloat(_rxBuf, position);
							position += 4;
							if (value < -255 || value > 1023) {
								value = 0;
							}
						}
							break;
						case 3: {
							value = readInt(_rxBuf, position, 2);
							position += 2;
						}
							break;
						case 4: {
							var l = _rxBuf[position];
							position++;
							value = readString(_rxBuf, position, l);
						}
							break;
						case 5: {
							value = readDouble(_rxBuf, position);
							position += 4;
						}
							break;
						case 6:
							value = readInt(_rxBuf, position, 4);
							position += 4;
							break;
					}
					if (type <= 6) {
						responseValue(extId, value);
					} else {
						responseValue();
					}
					_rxBuf = [];
				}
			}
		}
	}
	function readFloat(arr, position) {
		var f = [arr[position], arr[position + 1], arr[position + 2], arr[position + 3]];
		return parseFloat(f);
	}
	function readInt(arr, position, count) {
		var result = 0;
		for (var i = 0; i < count; ++i) {
			result |= arr[position + i] << (i << 3);
		}
		return result;
	}
	function readDouble(arr, position) {
		return readFloat(arr, position);
	}
	function readString(arr, position, len) {
		var value = "";
		for (var ii = 0; ii < len; ii++) {
			value += String.fromCharCode(_rxBuf[ii + position]);
		}
		return value;
	}
	function appendBuffer(buffer1, buffer2) {
		return buffer1.concat(buffer2);
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

	var watchdog = null;
	function deviceOpened(dev) {
		if (!dev) {
			// Opening the port failed.
			tryNextDevice();
			return;
		}
		device.set_receive_handler('Totobot', processData);
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
		if (watchdog) return { status: 1, msg: 'Probing for Totobot' };
		return { status: 2, msg: 'Totobot connected' };
	}

	var descriptor = {};
	ScratchExtensions.register('Totobot', descriptor, ext, { type: 'serial' });
})({});
