THREE.GameControls = function(c,e) {
	var self = this;
	var moveForward = false;
	var moveBackward = false;
	var moveLeft = false;
	var moveRight = false;
	var velocity = new THREE.Vector3();

	var controls;
	var prevTime = performance.now();
	var camera = c;
	var element = e;

	var maxPitch = Math.PI ;


	var pitch = new THREE.Object3D();
	var yaw = new THREE.Object3D();
	pitch.add(camera);
	yaw.add(pitch);

	function pointerlockchange(e) {
		console.log("POINTER LOCK CHANGED");
		console.log(e);
		if (document.pointerLockElement || document.mozPointerLockElement || document.webkitPointerLockElement) {
			document.addEventListener('mousemove', mousemove, false);
		}
		else {
			document.removeEventListener('mousemove', mousemove);	
		}
	}

	function pointerlockerror(e) {
		console.log("POINTER LOCK FAILED");
	}

	function keydown(e) {
		switch (e.keyCode) {
			case 38:
			case 87:
				moveForward = true;
				break;
			case 37:
			case 65:
				moveLeft = true;
				break;
			case 40:
			case 83:
				moveBackward = true;
				break;
			case 39:
			case 68:
				moveRight = true;
				break;
			case 32:
				if (canJump) velocity.y += 30;
				canJump = false;
				break;
		}
	}

	function keyup(e) {
		switch (e.keyCode) {
			case 38:
			case 87:
				moveForward = false;
				break;
			case 37:
			case 65:
				moveLeft = false;
				break;
			case 40:
			case 83:
				moveBackward = false;
				break;
			case 39:
			case 68:
				moveRight = false;
				break;
		}
	}

	function mousemove(e) {
		var movementX = event.movementX || event.mozMovementX || event.webkitMovementX || 0;
		var movementY = event.movementY || event.mozMovementY || event.webkitMovementY || 0;

		yaw.rotation.y -= movementX * 0.002;
		pitch.rotation.x -= movementY * 0.002;

		pitch.rotation.x = Math.max(-maxPitch, Math.min(maxPitch, pitch.rotation.x));
	}

	document.addEventListener('keydown', keydown, false);
	document.addEventListener('keyup', keyup, false);
	

	var havePointerLock = 'pointerLockElement' in document || 'mozPointerLockElement' in document || 'webkitPointerLockElement' in document;

	if (havePointerLock) {
		document.addEventListener('pointerlockchange', pointerlockchange, false);
		document.addEventListener('mozpointerlockchange', pointerlockchange, false);
		document.addEventListener('webkitpointerlockchange', pointerlockchange, false);
		document.addEventListener('pointerlockerror', pointerlockerror, false);
		document.addEventListener('mozpointerlockerror', pointerlockerror, false);
		document.addEventListener('webkitpointerlockerror', pointerlockerror, false);

		element.requestPointerLock = element.requestPointerLock || element.mozRequestPointerLock || element.webkitRequestPointerLock;
		document.exitPointerLock = document.exitPointerLock || document.mozExitPointerLock || document.webkitExitPointerLock;

		element.addEventListener("click", function() { element.requestPointerLock() });
		
	}
	else {
		console.log("NO POINTER LOCK");

	}

	this.getObject = function() {
		return yaw;
	}

	this.update = function() {
		var time = performance.now();
		var delta = (time-prevTime)/1000;

		velocity.x -= velocity.x*10.0*delta;
		velocity.z -= velocity.z*10.0*delta;
		velocity.y -= 9.8 * 10.0 * delta;

		if (moveForward) velocity.z -= 400.0*delta;
		if (moveBackward) velocity.z += 400.0*delta;
		if (moveLeft) velocity.x -= 400.0*delta;
		if (moveRight) velocity.x += 400.0*delta;

		yaw.translateX(velocity.x*delta);
		yaw.translateY(velocity.y*delta);
		yaw.translateZ(velocity.z*delta);

		if (yaw.position.y<1) {
			velocity.y = 0;
			yaw.position.y = 1;
			canJump = true;
		}

		// TODO Parameterize for general use
		if (yaw.position.z<-100) {
			yaw.position.z = -100;
			velocity.z = 0;
		}
		else if (yaw.position.z>100) {
			yaw.position.z = 100;
			velocity.z = 0;
		}

		if (yaw.position.x<-100) {
			yaw.position.x = -100;
			velocity.x = 0;
		}
		else if (yaw.position.x>100) {
			yaw.position.x = 100;
			velocity.x = 0;
		}

		prevTime = time;
	}
};