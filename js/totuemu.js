var TOTUEMU = (function() {
	var exports = {};
	var texture;
	var firebase;

	function Screen(paneltexture, n) {
		this.subno = Math.floor(Math.random()*512);
		this.y = Math.floor(n / 4) * 240;
		this.x = (n % 4) * 320
		this.n = 0;
		this.pantex = paneltexture;
		var self = this;

		this.paint = function paint() {
			if (TOTUEMU.submissions.length) {
				var sub = self.subno % TOTUEMU.submissions.length;
				var photos = TOTUEMU.submissions[sub].photos;

				self.pantex.ctx.drawImage(photos[self.n], self.x, self.y);
				self.n++;
				if (self.n>=photos.length) self.n=0;

				self.pantex.texture.needsUpdate = true;
			}
		}
	}

	function PanelTexture(panel,side) {
		this.canvas = $('<canvas width="1280" height="1920" class="texture"></canvas>')[0];
		$('#page-top').append(this.canvas);
		this.ctx = this.canvas.getContext('2d');

		this.ctx.fillStyle = side ? "red" : "green";
		this.ctx.fillRect(0,0,1280,1920);

		this.texture = new THREE.Texture(this.canvas);
		this.texture.needsUpdate = true;

		this.screens = [];
		for (var i=0; i<32; i++) {
			this.screens.push(new Screen(this, i));
		}

		var self = this;
		window.setInterval(function() {
			for (var i=0; i<32; i++) {
				self.screens[i].paint();
			}
		}, 750);
	}

	function Panel(scene,n) {
		var geometry = new THREE.BoxGeometry(4,8,4);
		this.activeTextures = [  new PanelTexture(n,0), new PanelTexture(n,1) ];

		var materials  = [
			new THREE.MeshLambertMaterial({ map: this.activeTextures[0].texture }),
			new THREE.MeshLambertMaterial({ map: texture }),
			new THREE.MeshLambertMaterial({ map: texture }),
			new THREE.MeshLambertMaterial({ map: texture }),
			new THREE.MeshLambertMaterial({ map: this.activeTextures[1].texture }),
			new THREE.MeshLambertMaterial({ map: texture })
		];

		var material = new THREE.MeshFaceMaterial(materials);

		this.mesh = new THREE.Mesh(geometry, material);
		this.mesh.position.x = 0;
		this.mesh.position.y = 0;
		this.mesh.position.z = 0;

		this.mesh.rotateOnAxis(new THREE.Vector3(0,1,0), -Math.PI/4);

		scene.add(this.mesh);
	};
	
	function init3d() {
		texture = THREE.ImageUtils.loadTexture('/img/texture-wood.jpg')
		var scene = new THREE.Scene();
		var camera = new THREE.PerspectiveCamera( 75, $('#virtual').width() / $('#virtual').height(), 0.1, 1000 );
		var renderer = new THREE.WebGLRenderer({ canvas: $('#virtual')[0] });
		var controls = new THREE.TrackballControls( camera );
		var pointLight = new THREE.PointLight(0xFFFFFF,5,50);
		pointLight.position.set(10, 10, 25);
		scene.add(pointLight);

		
		controls.rotateSpeed = 1.0;
		controls.zoomSpeed = 1.2;
		controls.panSpeed = 0.8;

		controls.noZoom = false;
		controls.noPan = false;

		controls.staticMoving = true;
		controls.dynamicDampingFactor = 0.3;

		controls.keys = [ 65, 83, 68 ];

		camera.position.z = 8;
		camera.position.y = 5;

		var panel = new Panel(scene,0);

		function render() {
			requestAnimationFrame(render);
			controls.update();
			renderer.render(scene, camera);
		}
	
		render();
	}

	function appendSubmission(snapshot) {
		var sub = snapshot.val();

		for (var i=0; i<sub.photos.length; i++) {
			var img = new Image();
			img.src = sub.photos[i];
			sub.photos[i] = img;

		}

		exports.submissions.push(sub);
		console.log(exports.submissions.length);
	}

	function initDb() {
		var firebase = new Firebase('https://totu.firebaseio.com/');
		exports.submissions = [];
		firebase.child('submissions').limitToLast(512).on('child_added', appendSubmission);
	}

	exports.init = function() {
		if ($('#virtual').length) {
			initDb();
			init3d();
		}
	}

	return exports;
}());

$(window).load(TOTUEMU.init);