var TOTU = TOTU || {};

TOTU.exploreConfig = {
	scaledown: 1,
	screen: { width: 320, height: 240, paddingLeft: 160, paddingTop: 120 },
	panel: { paddingBottom: 500, paddingTop: 50, paddingLeft: 0, paddingRight: 0 }
};

TOTU.explore = (function() {
	var exports = {};
	var firebase;
	var stats;
	var raycaster;
							
	function boundsCheckHandler(controls,position) {
		for (var i=0; i<exports.panels.length; i++) {
			var box = exports.panels[i].bounds.box;

			if (position.x >= box.min.x && position.x <= box.max.x &&
				position.z >= box.min.z && position.z <= box.max.z) {

				return false;
			}

		}
		return true;
	}

	function panelTimerHandler() {
		var n = 0;
		for (var i=0; i<exports.panels.length; i++) {
			n += exports.panels[i].paint();
		}
	}

	function scaleTimerHandler() {
		var config = TOTU.exploreConfig;
		var fps = parseInt($('#stats').text().replace(/^(\d+).*$/,'$1'));

		if (fps < 50 && config.scaledown < 11) {
			config.scaledown++;
			console.log("Increasing scaledown to " + config.scaledown);
			for (var i=0; i<exports.panels.length; i++) {
				exports.panels[i].updateResolution();
			}
		}
	}

	function renderHandler() {
		var scene = exports.scene;

		requestAnimationFrame(renderHandler);
		scene.controls.update();

		scene.renderer.render(scene, scene.camera);
		stats.update();
	}

	function moveHandler() {
		var panels = exports.panels;
		var controls = exports.scene.controls;

		var v = new THREE.Vector3(0,0,-1);
		raycaster.set(controls.getPosition(), controls.getDirection(v));

		for (var i=0; i<panels.length; i++) {
			var intersections = raycaster.intersectObject(panels[i].mesh);
			
			if (intersections && intersections.length) {
				raycaster.helper.position.set(0,0,0);
                raycaster.helper.lookAt(intersections[0].face.normal);
                raycaster.helper.position.copy(intersections[0].point);

                // So point is in world space, would need to translate and rotate to face 
			}
		}
	}

	function initScene() {
		var scene = exports.scene = new THREE.Scene();
		exports.scene.camera = new THREE.PerspectiveCamera(75, $('#virtual').width() / $('#virtual').height(), 0.1, 1000);
		exports.scene.renderer = new THREE.WebGLRenderer({ canvas: $('#virtual')[0] });

		// TODO lighting needs some work
		var pointLight = new THREE.PointLight(0xFFFFFF,5,50);
		pointLight.position.set(20, 25, 20);
		scene.add(pointLight);

		pointLight = new THREE.PointLight(0xFFFFFF,5,50);
		pointLight.position.set(-20, 25, -20);
		scene.add(pointLight);

		scene.add(new THREE.AmbientLight(0xAAAAAA));		
	}
	
	function initControls() {
		var controls = exports.scene.controls = new THREE.GameControls(exports.scene.camera, $('#virtual')[0]);
		
		var rig = controls.getObject();
		exports.scene.add(rig);
		rig.position.z = 36;

		controls.onboundscheck(boundsCheckHandler);	
		controls.onmove(moveHandler);
	}

	function initGround() {
		var groundTexture = THREE.ImageUtils.loadTexture('/img/texture-ground.jpg');
		groundTexture.wrapS = THREE.RepeatWrapping;
		groundTexture.wrapT = THREE.RepeatWrapping;
		groundTexture.repeat.set(512,512);
		var groundGeometry = new THREE.PlaneBufferGeometry(10000,10000);
		var groundMaterial =  new THREE.MeshLambertMaterial({ map: groundTexture });
		var ground = new THREE.Mesh(groundGeometry,groundMaterial);
		ground.rotation.x = -Math.PI/2;
		ground.position.y = -4;
		exports.scene.add(ground);
	}

	function initSky() {
		var urls = ['/img/texture-sky-front.jpg', '/img/texture-sky-back.jpg',
			'/img/texture-sky-top.jpg', '/img/texture-sky-down.jpg',
			'/img/texture-sky-left.jpg', '/img/texture-sky-right.jpg'];
		var textureCube = THREE.ImageUtils.loadTextureCube(urls);
		textureCube.format = THREE.RGBFormat;
		var shader = THREE.ShaderLib["cube"];
		shader.uniforms['tCube'].value = textureCube;		

		var material = new THREE.ShaderMaterial({
    		fragmentShader: shader.fragmentShader,
    		vertexShader: shader.vertexShader,
    		uniforms: shader.uniforms,
    		depthWrite: false,
    		side: THREE.BackSide
		});		
		skyboxMesh = new THREE.Mesh(new THREE.BoxGeometry(1000, 1000, 1000, 1, 1, 1, null, true), material);
		exports.scene.add(skyboxMesh);
	}

	function initPanels() {
		exports.panels = [];
		for (var i=0; i<8; i++) {
			var panel = new TOTU.Panel(exports.scene,i);
			exports.panels.push(panel);
		}
	}

	function initRaycaster() {
		raycaster = new THREE.Raycaster();
		var rchgeom = new THREE.CylinderGeometry(0,0.5,1);
		rchgeom.applyMatrix(new THREE.Matrix4().makeRotationX(Math.PI/2));
		var rchmat = new THREE.MeshLambertMaterial({ color: 0xEB1515, ambient: 0xEB1515, wireframe: false });
		raycaster.helper = new THREE.Mesh(rchgeom, rchmat);
		exports.scene.add(raycaster.helper);
	}

	function initTimers() {
		window.setInterval(panelTimerHandler,750);
		window.setInterval(scaleTimerHandler, 5000);		
	}

	function init3d() {
		initScene();
		initControls();
		initGround();
		initSky();
		initPanels();
		initRaycaster();
		initTimers();

		renderHandler();
	}

	function appendSubmission(snapshot) {
		var sub = snapshot.val();

		for (var i=0; i<sub.photos.length; i++) {
			var img = new Image();
			img.src = sub.photos[i];
			sub.photos[i] = img;

		}

		exports.submissions.push(sub);
	}

	function initDb() {
		var firebase = new Firebase('https://totu.firebaseio.com/');
		exports.submissions = [];
		firebase.child('submissions').limitToLast(512).on('child_added', appendSubmission);
	}

	function initStats() {
		stats = new Stats();
		stats.domElement.style.position = 'absolute';
		stats.domElement.style.top = '0px';
		stats.domElement.style.left = '0px';
		stats.domElement.style.zIndex = 10000;
		$('#vcon').append(stats.domElement);
	}

	exports.init = function() {
		if ($('#virtual').length) {
			initDb();
			initStats();
			init3d();
		}
	}

	return exports;
}());

$(window).load(TOTU.explore.init);