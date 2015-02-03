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

	var xaxis = new THREE.Vector3( 1, 0, 0 );
	var yaxis = new THREE.Vector3( 0, 1, 0 );
	function Panel(scene,radius,rx) {
		var geometry = new THREE.BoxGeometry(4,8,4);
		var m1 = new THREE.MeshLambertMaterial({ color: 0x0000FF }); // top and bottom
		var m2 = new THREE.MeshLambertMaterial({ color: 0x00FF00 }); // inner faces
		var m3 = new THREE.MeshLambertMaterial({ color: 0xFF0000 }); // outward faces
		var material = new THREE.MeshFaceMaterial([m3, m2, m1, m1, m3, m2]);

		var mesh = new THREE.Mesh(geometry, material);
		mesh.rotateOnAxis(yaxis, rx);
		mesh.translateZ(radius);
		mesh.translateX(radius);

		scene.add(mesh);
		return mesh;
	};
	
	function World(scene) {
		var radius = 128;
		var geometry = new THREE.SphereGeometry(radius, 32, 32, 0, 2*Math.PI, 0, Math.PI/2);
		var map = THREE.ImageUtils.loadTexture( "/img/collage_lb_image_page15_0_1.png" );
        var material = new THREE.MeshLambertMaterial( { map: map, color: 0x111111, fog: false } );
		material.side = THREE.BackSide;

		var mesh = new THREE.Mesh(geometry, material);
		mesh.translateY(-4);

		scene.add(mesh);
		return mesh;
	};

	function Ground(scene) {
		var radius = 128;
		var geometry = new THREE.CircleGeometry(radius, 32);
		var map = THREE.ImageUtils.loadTexture( "/img/seamless_dirt_ground_texture_by_hhh316-d4fon0w.jpg" );
        var material = new THREE.MeshLambertMaterial( { map: map, color: 0x010101, fog: false } );
		
		var mesh = new THREE.Mesh(geometry, material);
		mesh.translateY(-4);
		mesh.rotateOnAxis(xaxis, -Math.PI/2);

		scene.add(mesh);
		return mesh;
	};


	exports.init = function() {
		if ($('#virtual').length) {
			var scene = new THREE.Scene();
			var camera = new THREE.PerspectiveCamera( 75, $('#virtual').width() / $('#virtual').height(), 0.1, 1000 );
			var renderer = new THREE.WebGLRenderer({ canvas: $('#virtual')[0] });
			var controls = new THREE.TrackballControls( camera );

			var pLight = new THREE.PointLight(0xFFFFFF,5);
			pLight.position.set(30, 30, 25);
			scene.add(pLight);

			//pointLight = new THREE.PointLight(0xFFFFFF,10,100);
			//pointLight.position.set(-10, 10, -25);
			//scene.add(pointLight);

            var ambLight = new THREE.AmbientLight( 0x444444 );
            scene.add(ambLight);

			controls.rotateSpeed = 1.0;
			controls.zoomSpeed = 1.2;
			controls.panSpeed = 0.8;

			controls.noZoom = false;
			controls.noPan = false;

			controls.staticMoving = true;
			controls.dynamicDampingFactor = 0.3;

			controls.keys = [ 65, 83, 68 ];

			camera.position.z = 8;

			var panels = [];
			var count = 8;
			for (var i = 0; i < count; i++) {
			  panels[i] = new Panel(scene, 18, 2*i*Math.PI/count);
			}
			new Ground(scene);			
			new World(scene);

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