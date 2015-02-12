var TOTU = TOTU || {};

TOTU.Panel = function Panel(scene,n) {
	this.parent = scene;
	this.x = Math.cos((n/8)*(Math.PI*2))*18;
	this.y = Math.sin((n/8)*(Math.PI*2))*18;
	this.a = -Math.PI/4 * n;

	// Load the background texture
	var texture = THREE.ImageUtils.loadTexture('/img/texture-wood.jpg');
	texture.minFilter = THREE.LinearFilter;	
	
	// Geometry
	var geometry = new THREE.BoxGeometry(4,8,4);
	this.activeTextures = [  new TOTU.PanelTexture(this,n,0), new TOTU.PanelTexture(this,n,1) ];

	// Set up face by face materials
	var materials  = [
		new THREE.MeshLambertMaterial({ map: this.activeTextures[0].texture }),
		new THREE.MeshLambertMaterial({ map: texture }),
		new THREE.MeshLambertMaterial({ map: texture }),
		new THREE.MeshLambertMaterial({ map: texture }),
		new THREE.MeshLambertMaterial({ map: this.activeTextures[1].texture }),
		new THREE.MeshLambertMaterial({ map: texture })
	];
	var material = new THREE.MeshFaceMaterial(materials);

	// Create, orient and position the mesh
	this.mesh = new THREE.Mesh(geometry, material);
	this.mesh.position.x = this.x;
	this.mesh.position.y = 0;
	this.mesh.position.z = this.y;
	this.mesh.rotateOnAxis(new THREE.Vector3(0,1,0), this.a + Math.PI/4);
	scene.add(this.mesh);

	// Setup bounding box
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
