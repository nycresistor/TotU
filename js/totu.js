function startVideo() {
	navigator.getUserMedia = navigator.getUserMedia ||
		navigator.webkitGetUserMedia ||
		navigator.mozGetUserMedia ||
		navigator.msGetUserMedia;
 
	navigator.getUserMedia({video:true}, 
		function(stream) {
			$('video').attr('src',window.URL.createObjectURL(stream));
		},
		function(err) { console.log(err) }
	);
}


function startThreeJs() {
	var video = $('video')[0];
	var scene = new THREE.Scene();
	var camera = new THREE.PerspectiveCamera( 75, $('#threejs').width() / $('#threejs').height(), 0.1, 1000 );
	var renderer = new THREE.WebGLRenderer({ canvas: $('#threejs')[0] });
	var controls = new THREE.TrackballControls( camera );

	controls.rotateSpeed = 1.0;
	controls.zoomSpeed = 1.2;
	controls.panSpeed = 0.8;

	controls.noZoom = false;
	controls.noPan = false;

	controls.staticMoving = true;
	controls.dynamicDampingFactor = 0.3;

	controls.keys = [ 65, 83, 68 ];

	var geometry = new THREE.BoxGeometry(1,1,1);
	var texture = new THREE.Texture($('#texture')[0]);
	var textureContext = $('#texture')[0].getContext('2d');
	texture.minFilter = THREE.LinearFilter;
	texture.magFilter = THREE.LinearFilter;
	var material = new THREE.MeshBasicMaterial( { map: texture, overdraw: 0.5 } );
	var cube = new THREE.Mesh( geometry, material );
	scene.add( cube );

	camera.position.z = 5;


	function render() {
		requestAnimationFrame(render);

		if (video.readyState === video.HAVE_ENOUGH_DATA) {
			textureContext.drawImage(video, 0, 0, 320, 240);
			texture.needsUpdate = true;
		}
		controls.update();

		renderer.render(scene, camera);
	}
	render();
}

$('#startVideo').click(function() { startVideo(); startThreeJs(); });

startThreeJs();