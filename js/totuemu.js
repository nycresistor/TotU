var TOTUEMU = (function() {
	var config = {
		scaledown: 1,
		screen: { width: 320, height: 240, paddingLeft: 160, paddingTop: 120 },
		panel: { paddingBottom: 500, paddingTop: 50, paddingLeft: 0, paddingRight: 0 }
	};
	var exports = {};
	var textureImage;
	var texture;
	var firebase;
	var camera;
	var scene;
	var panels;
	var stats;
						
	function inTriangle (px,py,t) {
		var v0 = [t[4]-t[0],t[5]-t[1]];
		var v1 = [t[2]-t[0],t[3]-t[1]];
		var v2 = [px-t[0],py-t[1]];

		var dot00 = (v0[0]*v0[0]) + (v0[1]*v0[1]);
		var dot01 = (v0[0]*v1[0]) + (v0[1]*v1[1]);
		var dot02 = (v0[0]*v2[0]) + (v0[1]*v2[1]);
		var dot11 = (v1[0]*v1[0]) + (v1[1]*v1[1]);
		var dot12 = (v1[0]*v2[0]) + (v1[1]*v2[1]);

		var invDenom = 1/ (dot00 * dot11 - dot01 * dot01);

		var u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		var v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		return ((u >= 0) && (v >= 0) && (u + v < 1));
	}
	
	function Screen(paneltexture, n) {
		var self = this;

		this.subno = Math.floor(Math.random()*512);
		this.pantex = paneltexture;
		this.n = 0;

		this.setSize = function setSize() {
			this.width = config.screen.width / config.scaledown;
			this.height = config.screen.height / config.scaledown;
			this.y = Math.floor(n / 4) * this.height + 
				config.screen.paddingTop / config.scaledown * (Math.floor(n / 4) + 1) +
				config.panel.paddingTop / config.scaledown;

			this.x = ((n % 4) * config.screen.width + 
				config.screen.paddingLeft * ((n % 4 )+ 1) +
				config.panel.paddingLeft) / config.scaledown;
		}

		this.setSize();
		
		this.paint = function paint() {
			if (TOTUEMU.submissions.length) {
				var sub = self.subno % TOTUEMU.submissions.length;
				var photos = TOTUEMU.submissions[sub].photos;

				self.pantex.ctx.drawImage(photos[self.n], self.x, self.y, self.width, self.height);
				self.n++;
				if (self.n>=photos.length) self.n=0;

			}
			else {
				self.pantex.ctx.fillStyle = "black";
				self.pantex.ctx.fillRect(self.x, self.y, self.width, self.height);
			}

			return true;
		}

		this.updateResolution = function() {
			self.setSize();
		}
	}

	function PanelTexture(parent,panel,side) {
		var self = this;

		this.panel = panel;
		this.side = side;
		this.parent = parent;

		// Set up a hidden canvas to draw the texture
		this.setupCanvas = function setupCanvas() {
			this.canvas = $('<canvas width="' + this.canvasWidth + '" height="' + this.canvasHeight + '" class="texture"></canvas>')[0];
			$('#vcon').append(this.canvas);
			this.ctx = this.canvas.getContext('2d');
			this.texture = new THREE.Texture(this.canvas);
		}

		this.paintBackground = function paintBackground() {
			// Tile on the wood texture
			for (var x=0; x<this.canvasWidth; x+=textureImage.width) {
				for (var y=0; y<this.canvasHeight; y+=textureImage.height) {
					this.ctx.drawImage(textureImage,x,y);
				}
			}
		}

		this.setSize = function setSize() {
			this.canvasWidth = ((config.screen.width + config.screen.paddingLeft) * 4 + 
				config.screen.paddingLeft + config.panel.paddingLeft + config.panel.paddingRight) / 
				config.scaledown;
			this.canvasHeight = ((config.screen.height + config.screen.paddingTop) * 8 + 
				config.screen.paddingTop + config.panel.paddingTop + config.panel.paddingBottom) /
				config.scaledown;
		}

		this.resizeCanvas = function resizeCanvas() {
			this.setSize();
			this.canvas.width = this.canvasWidth;
			this.canvas.height = this.canvasHeight;
		}


		this.paintScreens = function paintScreens() {
			for (var i=0; i<32; i++) {
				this.screens[i].paint();
			}
			
			this.texture.needsUpdate = true;
		}

		this.setupScreens = function setupScreens() {
			// Set up the screens and paint the first pass
			this.screens = [];
			for (var i=0; i<32; i++) {
				this.screens.push(new Screen(this, i));
			}

			this.paintScreens();
		}

		this.setupView = function setupView() {
			// Triangle from which the camera can see the panel
			var a = parent.a + Math.PI/2*panel
			var a1 = a + (side === 1 ? Math.PI*0.6 : Math.PI/6);
			var a2 = a - (side === 1 ? Math.PI/6 : Math.PI*0.6);

			this.view = [
				parent.x,parent.y,
				parent.x+Math.cos(a1)*200,parent.y+Math.sin(a1)*200,
				parent.x+Math.cos(a2)*200,parent.y+Math.sin(a2)*200
			];
		}

		this.paint = function() {
			// If camera is in view triangle, update the screens
			var cx = camera.parent.parent.position.x;
			var cy = camera.parent.parent.position.z;

			if (inTriangle(cx,cy,self.view)) {
				for (var i=0; i<32; i++) {
					self.screens[i].paint();
				}
				return 1;
			}

			return 0;
		}

		this.updateResolution = function() {
			self.resizeCanvas();
			self.paintBackground();
			for (var i=0; i<32; i++) {
				self.screens[i].updateResolution();
			}
			self.paintScreens();
		}

		
		this.setSize();
		this.setupCanvas();
		this.paintBackground();
		this.setupScreens();
		this.setupView();
	}

	function Panel(scene,n) {
		this.x = Math.cos((n/8)*(Math.PI*2))*18;
		this.y = Math.sin((n/8)*(Math.PI*2))*18;
		this.a = -Math.PI/4 * n;

		var geometry = new THREE.BoxGeometry(4,8,4);
		this.activeTextures = [  new PanelTexture(this,n,0), new PanelTexture(this,n,1) ];

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

		this.mesh.position.x = this.x;
		this.mesh.position.y = 0;
		this.mesh.position.z = this.y;

		this.mesh.rotateOnAxis(new THREE.Vector3(0,1,0), this.a + Math.PI/4);

		scene.add(this.mesh);

		this.bounds = new THREE.BoundingBoxHelper(this.mesh,0xff0000);
		this.bounds.update();

		this.paint = function() {
			var n = 0;

			for (var i=0; i<this.activeTextures.length; i++) {
				var painted = this.activeTextures[i].paint();
				this.activeTextures[i].texture.needsUpdate = (painted > 0);
				n += painted;
			}

			return n;
		}

		this.updateResolution = function() {
			for (var i=0; i<this.activeTextures.length; i++) {
				this.activeTextures[i].updateResolution();
			}
		}
	};

	function checkBounds(position) {
		for (var i=0; i<panels.length; i++) {
			var box = panels[i].bounds.box;

			if (position.x >= box.min.x && position.x <= box.max.x &&
				position.z >= box.min.z && position.z <= box.max.z) {

				return false;
			}

		}
		return true;
	}
	
	// TODO Break up
	function init3d() {
		texture = THREE.ImageUtils.loadTexture('/img/texture-wood.jpg');
		scene = new THREE.Scene();
		camera = new THREE.PerspectiveCamera( 75, $('#virtual').width() / $('#virtual').height(), 0.1, 1000 );
		var renderer = new THREE.WebGLRenderer({ canvas: $('#virtual')[0] });

		// TODO lighting needs some work
		var pointLight = new THREE.PointLight(0xFFFFFF,5,50);
		pointLight.position.set(20, 25, 20);
		scene.add(pointLight);

		pointLight = new THREE.PointLight(0xFFFFFF,5,50);
		pointLight.position.set(-20, 25, -20);
		scene.add(pointLight);

		scene.add(new THREE.AmbientLight(0xAAAAAA));

		// My new controls
		var controls = new THREE.GameControls(camera, $('#virtual')[0]);
		var rig = controls.getObject();
		scene.add(rig);
		rig.position.z = 36;
		controls.setBoundsCallback(checkBounds);

		// Ground
		var groundTexture = THREE.ImageUtils.loadTexture('/img/texture-ground.jpg');
		groundTexture.wrapS = THREE.RepeatWrapping;
		groundTexture.wrapT = THREE.RepeatWrapping;
		groundTexture.repeat.set(512,512);
		var groundGeometry = new THREE.PlaneGeometry(10000,10000);
		var groundMaterial =  new THREE.MeshLambertMaterial({ map: groundTexture });
		var ground = new THREE.Mesh(groundGeometry,groundMaterial);
		ground.rotation.x = -Math.PI/2;
		ground.position.y = -4;
		scene.add(ground);

		// Skybox
		var urls = ['/img/texture-sky-front.jpg', '/img/texture-sky-back.jpg',
			'/img/texture-sky-top.jpg', '/img/texture-sky-down.jpg',
			'/img/texture-sky-left.jpg', '/img/texture-sky-right.jpg'];
		var textureCube = THREE.ImageUtils.loadTextureCube(urls);
		textureCube.format = THREE.RGBFormat;
		console.log(textureCube);
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
		scene.add(skyboxMesh);

		// Panels
		panels = [];
		for (var i=0; i<8; i++) {
			var panel = new Panel(scene,i);
			panels.push(panel);
		}

		// Debugging, can remove
		exports.panels = panels;
		exports.camera = camera;
		exports.scene = scene;
		exports.controls = controls;

		// Set up the screen update timer
		window.setInterval(
			function() {
				var n = 0;
				for (var i=0; i<panels.length; i++) {
					n += panels[i].paint();
				}
			}, 750
		);

		window.setInterval(
			function() {			
				var fps = parseInt($('#stats').text().replace(/^(\d+).*$/,'$1'));
				if (fps < 50 && config.scaledown < 11) {
					config.scaledown++;
					console.log("Increasing scaledown to " + config.scaledown);
					for (var i=0; i<panels.length; i++) {
						panels[i].updateResolution();
					}
				}
			}, 5000
		);

		function render() {
			requestAnimationFrame(render);
			controls.update();
			renderer.render(scene, camera);
			stats.update();
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
			textureImage = new Image();
			textureImage.onload = function() {
				initDb();
				initStats();
				init3d();
				
			}
			textureImage.src = "/img/texture-wood.jpg";
		}
	}

	return exports;
}());

$(window).load(TOTUEMU.init);