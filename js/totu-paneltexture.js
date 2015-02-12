var TOTU = TOTU || {};

TOTU.PanelTexture = function PanelTexture(parent,panel,side) {
	var self = this;

	this.panel = panel;
	this.side = side;
	this.parent = parent;

	this.init = function init() {
		this.setSize();
		this.setupCanvas();
		this.paintBackground();
		this.setupScreens();
		this.setupView();
	}

	// Set up a hidden canvas to draw the texture
	this.setupCanvas = function setupCanvas() {
		this.canvas = $('<canvas width="' + this.canvasWidth + '" height="' + this.canvasHeight + '" class="texture"></canvas>')[0];
		$('#vcon').append(this.canvas);
		this.ctx = this.canvas.getContext('2d');
		this.texture = new THREE.Texture(this.canvas);
		this.texture.minFilter = THREE.LinearFilter;
	}

	this.paintBackground = function paintBackground() {
		if (TOTU.PanelTexture.textureReady) {
			var textureImage = TOTU.PanelTexture.textureImage;

			// Tile on the wood texture
			for (var x=0; x<this.canvasWidth; x+=textureImage.width) {
				for (var y=0; y<this.canvasHeight; y+=textureImage.height) {
					this.ctx.drawImage(textureImage,x,y);
				}
			}
		}
		else {
			this.needsBackground = true;
		}
	}

	this.setSize = function setSize() {
		var config = TOTU.exploreConfig;

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
			this.screens.push(new TOTU.Screen(this, i));
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
		// Paint the background if it wasn't painted yet
		if (self.needsBackground) {
			self.needsBackground = false;
			self.paintBackground();
		}

		// If camera is in view triangle, update the screens
		var camera = self.parent.parent.camera;
		var cx = camera.parent.parent.position.x;
		var cy = camera.parent.parent.position.z;

		if (THREE.Util.inTriangle(cx,cy,self.view)) {
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
	
	this.init();
}

TOTU.PanelTexture.textureReady = false;
TOTU.PanelTexture.textureImage = new Image();
TOTU.PanelTexture.textureImage.onload = function() {
	TOTU.PanelTexture.textureReady = true;
}
TOTU.PanelTexture.textureImage.src = "/img/texture-wood.jpg";

