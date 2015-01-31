var TOTUEMU = (function() {
	var exports = {};

	// Classes
	function Screen(nx,ny,nz) {
		var x = nx;
		var y = ny;
		var z = nz;


		this.paint = function paint() {

		}
	};

	function Panel(scene,nx,ny,nz) {
		var geometry = new THREE.BoxGeometry(4,8,4);
		var material = new THREE.MeshLambertMaterial({ color: 0x0000FF });
		
		this.mesh = new THREE.Mesh(geometry, material);

		scene.add(this.mesh);
	};
	
	exports.init = function() {
		if ($('#virtual').length) {
			var scene = new THREE.Scene();
			var camera = new THREE.PerspectiveCamera( 75, $('#virtual').width() / $('#virtual').height(), 0.1, 1000 );
			var renderer = new THREE.WebGLRenderer({ canvas: $('#virtual')[0] });
			var controls = new THREE.TrackballControls( camera );
			var pointLight = new THREE.PointLight(0xFFFFFF,10,100);
			pointLight.position.set(10, 10, 25);
			scene.add(pointLight);

			pointLight = new THREE.PointLight(0xFFFFFF,10,100);
			pointLight.position.set(-10, 10, -25);
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

			var panel = new Panel(scene,0,0,0);

			function render() {
				requestAnimationFrame(render);
				controls.update();
				renderer.render(scene, camera);
			}
		
			render();
		}
	}

	return exports;
}());

$(window).load(TOTUEMU.init);