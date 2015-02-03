var TOTUEMU = (function() {
	var config = {
		screen: { width: 320, height: 240, paddingLeft: 160, paddingTop: 120 },
		panel: { paddingBottom: 500, paddingTop: 50, paddingLeft: 0, paddingRight: 0 }
	};

	var exports = {};
	var textureImage;
	var texture;
	var firebase;

	function Screen(paneltexture, n) {
		var self = this;

		this.subno = Math.floor(Math.random()*512);
		this.pantex = paneltexture;
		this.n = 0;

		this.y = Math.floor(n / 4) * config.screen.height + 
			config.screen.paddingTop * (Math.floor(n / 4) + 1) +
			config.panel.paddingTop;

		this.x = (n % 4) * config.screen.width + 
			config.screen.paddingLeft * ((n % 4 )+ 1) +
			config.panel.paddingLeft;

		this.paint = function paint() {
			if (TOTUEMU.submissions.length) {
				var sub = self.subno % TOTUEMU.submissions.length;
				var photos = TOTUEMU.submissions[sub].photos;

				self.pantex.ctx.drawImage(photos[self.n], self.x, self.y, config.screen.width, config.screen.height);
				self.n++;
				if (self.n>=photos.length) self.n=0;

			}
			else {
				self.pantex.ctx.fillStyle = "black";
				self.pantex.ctx.fillRect(self.x, self.y, config.screen.width, config.screen.height);
			}

			self.pantex.texture.needsUpdate = true;
		}
	}

	function PanelTexture(panel,side) {
		var canvasWidth = (config.screen.width + config.screen.paddingLeft) * 4 + 
			config.screen.paddingLeft + config.panel.paddingLeft + config.panel.paddingRight;
		var canvasHeight = (config.screen.height + config.screen.paddingTop) * 8 + 
			config.screen.paddingTop + config.panel.paddingTop + config.panel.paddingBottom;

		this.canvas = $('<canvas width="' + canvasWidth + '" height="' + canvasHeight + '" class="texture"></canvas>')[0];
		$('#page-top').append(this.canvas);
		this.ctx = this.canvas.getContext('2d');

		for (var x=0; x<canvasWidth; x+=textureImage.width) {
			for (var y=0; y<canvasHeight; y+=textureImage.height) {
				this.ctx.drawImage(textureImage,x,y);
			}
		}

		this.texture = new THREE.Texture(this.canvas);
		this.texture.needsUpdate = true;

		this.screens = [];
		for (var i=0; i<32; i++) {
			this.screens.push(new Screen(this, i));
			this.screens[i].paint();
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
		texture = THREE.ImageUtils.loadTexture('/img/texture-wood.jpg');
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
			textureImage = new Image();
			textureImage.onload = function() {
				initDb();
				init3d();
			}
			textureImage.src = "/img/texture-wood.jpg";
		}
	}

	return exports;
}());

$(window).load(TOTUEMU.init);