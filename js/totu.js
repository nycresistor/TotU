var TOTU = (function() {
	var state = "off";
	var exports;
	var video;
	var app;
	var ctx;
	var timer;
	var actions = {};
	var photos = [];

	function init() {
		navigator.getUserMedia = navigator.getUserMedia ||
			navigator.webkitGetUserMedia ||
			navigator.mozGetUserMedia ||
			navigator.msGetUserMedia;

		video = $('video')[0];
		app = $('#app')[0];
		ctx = app.getContext('2d');

		actions['photo'] = photo;

		$('.btn').click(fireAction);
	}

	function fireAction() {
		var action = this.getAttribute("data-action");
		actions[action]();
	}

	function photo() {
		if (state == "off") {
			state = "preview";
			preview();

		}
		else if (state == "preview") {
			state = "record"
			record();
		}
		else if (state == "postview") {
			clearInterval(timer);
			state = "preview";
			preview();
		}
	}

	function preview() {
		if (video.readyState == 0) {
			navigator.getUserMedia({video:true}, 
				function(stream) {
					$('#btn-photo').attr('src','/img/btn-photo.png');
					video.src = window.URL.createObjectURL(stream);
					timer = setInterval(function() { 
						ctx.drawImage(video,0,0,app.width,app.height); 
					}, 33);
				},
				function(err) { console.log(err) }
			);
		}
		else {
			timer = setInterval(function() { 
				ctx.drawImage(video,0,0,app.width,app.height); 
			}, 33);
		}
	}

	function record() {
		$('#btn-bar').hide();
		$('#countdown-bar').show();
		photos = [];

		var count = 8;
		var click = $('#click')[0];
		var beep = $('#beep')[0];

		var ctimer = setInterval(function() {
			count--;
			
			if (count) {

				if (count > 4) {
					console.log("playing beep");
					beep.currentTime = 0;
					beep.play();
				}
				else {
					console.log("playing click")
					click.currentTime = 0;
					click.play();
					var img = new Image();
					img.src = app.toDataURL('image/jpeg');
					photos.push(img);
				}

				$('#countdown-' + (7-count)).removeClass('active');
				$('#countdown-' + (8-count)).addClass('active');
			}
			else {
				$('#countdown-7').removeClass('active');
				clearInterval(ctimer);
				$('#btn-bar').show();
				$('#countdown-bar').hide();
				state = "postview";
				postview();
			}
		}, 850);
	}

	function postview() {
		var idx = 0;
		clearInterval(timer);
		console.log(photos.length);

		timer = setInterval(function() {
			ctx.drawImage(photos[idx],0,0);
			idx++;
			if (idx >= photos.length) 
				idx=0;
		}, 850);
	}

/*
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
*/
	$(window).load(init);

	return exports;
}());

//$(window).load(function() { startThreeJs(); });